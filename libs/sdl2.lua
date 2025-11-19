project "SDL2"
    kind "StaticLib"
    language "C"
    architecture "x86_64"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}/SDL2"

    includedirs { "SDL2/include" }

    files {
        "SDL2/src/*.c",
        "SDL2/src/*/*.c",
        "SDL2/src/*.h",
        "SDL2/src/*/*.h"
    }

    filter "system:windows"
        defines { "_WINDOWS" }
        links { "user32", "gdi32", "winmm", "imm32", "version", "ole32", "oleaut32", "setupapi" }

    filter "system:linux"
        links { "pthread", "dl" }
