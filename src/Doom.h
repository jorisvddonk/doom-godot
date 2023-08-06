#pragma once

#include "godot_cpp/classes/image_texture.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "godot_cpp/classes/audio_server.hpp"

#include <godot_cpp/core/binder_common.hpp>

#include "../extern/puredoom/PureDOOM.h"

using namespace godot;

class Doom : public Node
{
    GDCLASS( Doom, Node )

public:
    // Functions.
    Ref<Image> getFramebuffer();
    void initialize();
    void update();
    void key_down(doom_key_t key);
    void key_up(doom_key_t key);
    int tick_midi();

    Doom();
protected:
    static void _bind_methods();
private:
    PackedByteArray pba;
    Ref<Image> image;
};

VARIANT_ENUM_CAST(doom_key_t);