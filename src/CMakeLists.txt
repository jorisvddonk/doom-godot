# SPDX-License-Identifier: Unlicense

file(GLOB_RECURSE doom_src_files CONFIGURE_DEPENDS ../extern/puredoom/src/DOOM/*.c)

target_sources( ${PROJECT_NAME}
    PRIVATE
        Doom.h
        Doom.cpp
        ../extern/puredoom/PureDOOM.h
        GDExtensionTemplate.h
        GDExtensionTemplate.cpp
        RegisterExtension.cpp
        ${doom_src_files}
)

target_compile_definitions( ${PROJECT_NAME} PRIVATE DOOM_IMPLEMENT_MALLOC DOOM_IMPLEMENT_FILE_IO DOOM_IMPLEMENT_GETTIME DOOM_IMPLEMENT_GETENV)

target_include_directories( ${PROJECT_NAME}
    PRIVATE
        "src"
)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm.lib)
endif (WIN32)