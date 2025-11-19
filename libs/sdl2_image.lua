project "SDL2_image"
    kind "StaticLib"
    language "C"
    architecture "x86_64"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}/SDL2_image"

    includedirs {
        "SDL2_image/include",
        "SDL2/include"
    }

    files {
        "SDL2_image/src*.c",
        "SDL2_image/src*.h"
    }

    links { "SDL2" }
