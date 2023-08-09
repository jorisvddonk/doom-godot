#include "godot_cpp/classes/image_texture.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/timer.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "godot_cpp/classes/audio_server.hpp"
#include "Doom.h"

#define DOOM_IMPLEMENTATION
#include "../extern/puredoom/PureDOOM.h"

using namespace godot;

Ref<Image> Doom::getFramebuffer()
{
    return this->image;
}

void Doom::update()
{
    //AudioServer::get_singleton()->lock();
    doom_update();
    // update the framebuffer as well
    const unsigned char* framebuffer = doom_get_framebuffer(3); // RGB
    for (uint16_t i = 0; i < 320 * 200; i++) {
        this->pba.set(i * 3, framebuffer[i * 3]);
        this->pba.set(i * 3 + 1, framebuffer[i * 3 + 1]);
        this->pba.set(i * 3 + 2, framebuffer[i * 3 + 2]);
    }
    (*this->image)->set_data(320, 200, false, Image::FORMAT_RGB8, this->pba);
    //AudioServer::get_singleton()->unlock();
}

static void doom_print_impl(const char* str)
{
    UtilityFunctions::print(str);
}

void doom_exit_impl(int code)
{
    UtilityFunctions::print("Doom exit with code ", code);
}

void Doom::initialize()
{
#ifdef WIN32
    unsigned long err;
    if (!(err = midiOutOpen(&this->handle, (UINT)-1, 0, 0, CALLBACK_NULL))) {
        has_midi = true;
        Timer *t = new Timer();
        this->add_child(t);
        t->set_paused(false);
        t->set_autostart(true);
        t->set_one_shot(false);
        double wait = 0.00714285714;
        t->set_wait_time(wait);
        t->connect("timeout", this->get("midi_process"));
        t->start();
        UtilityFunctions::print( "Midi initialized" );
    } else {
        UtilityFunctions::print( "Could not initialize MIDI subsystem" );
    }
#endif
    UtilityFunctions::print("initializing doom...");
	char* argv[] = { "doom", "-file", "DOOM.WAD", NULL };
    doom_set_print(&doom_print_impl);
    doom_set_exit(&doom_exit_impl);
	doom_init(0, argv, 0);
    UtilityFunctions::print("doom initialized");
    doom_update();
    UtilityFunctions::print("doom updated");
    UtilityFunctions::print("initializing image texture");
    const unsigned char* framebuffer = doom_get_framebuffer(3); // RGB
    this->pba = PackedByteArray();
    for (uint16_t i = 0; i < 320 * 200; i++) {
        uint8_t val = framebuffer[i];
        this->pba.append(framebuffer[i * 3]);
        this->pba.append(framebuffer[i * 3 + 1]);
        this->pba.append(framebuffer[i * 3 + 2]);
    }
    this->image = Image::create_from_data(320, 200, false, Image::FORMAT_RGB8, this->pba);
    UtilityFunctions::print("image texture initialized");
}

void Doom::key_down(doom_key_t key)
{
    doom_key_down(key);
}

void Doom::key_up(doom_key_t key)
{
    doom_key_up(key);
}

void Doom::midi_process()
{
    int midi_msg;
    while ((midi_msg = doom_tick_midi()) != 0) {
#ifdef WIN32
        unsigned long err;
        err = midiOutShortMsg(this->handle, midi_msg);
#endif
    }
}

Doom::Doom()
{
    UtilityFunctions::print( "Doom - constructor called." );
}

void Doom::_bind_methods()
{
    // Methods.
    ClassDB::bind_method( D_METHOD( "get_framebuffer" ), &Doom::getFramebuffer );
    ClassDB::bind_method( D_METHOD( "initialize" ), &Doom::initialize );
    ClassDB::bind_method( D_METHOD( "update" ), &Doom::update );
    ClassDB::bind_method( D_METHOD( "key_down" ), &Doom::key_down );
    ClassDB::bind_method( D_METHOD( "key_up" ), &Doom::key_up );
    ClassDB::bind_method( D_METHOD( "midi_process" ), &Doom::midi_process );

    // Properties
    // Signals
}
