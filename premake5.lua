workspace "Lightbringer"
    configurations { "Debug", "Release" }
    startproject "Lightbringer"

    flags { "MultiProcessorCompile" }

    filter "configurations:Debug"
        defines { "DEBUG", "DEBUG_SHADER" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "Speed"
        flags { "LinkTimeOptimization" }

project "Engine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
	architecture "x86_64"
    targetdir "build/engine/%{cfg.buildcfg}"
    objdir "build/engine/%{cfg.buildcfg}"

    includedirs { 
        "libs/glad/include/", "libs/glm/", "libs/imgui/", "libs/imgui/examples", "libs/SDL2/include", "libs/SDL2_image/include", "engine/src"
    }

    files { 
        "engine/src/*.cpp",
        "engine/src/camera/*.cpp",
        "engine/src/renderer/*.cpp",
        "engine/src/resources/*.cpp",
        "engine/src/scene/*.cpp",
        "engine/src/timer/*.cpp",
        "engine/src/window/*.cpp",
        "engine/src/*.h",
        "engine/src/camera/*.h",
        "engine/src/renderer/*.h",
        "engine/src/resources/*.h",
        "engine/src/scene/*.h",
        "engine/src/timer/*.h",
        "engine/src/window/*.h",
    }

    links { "SDL2", "SDL2main", "SDL2_image", "GLM", "GLAD", "ImGui" }

    filter "system:linux"
        links { "dl", "pthread" }

        defines { "_X11" }

    filter "system:windows"
        defines { "_WINDOWS" }

project "Lightbringer"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
	architecture "x86_64"

    targetdir "build/game/bin/%{cfg.buildcfg}"
    objdir "build/game/obj/%{cfg.buildcfg}"

    includedirs { "include/", "libs/glad/include/", "libs/glm/", "libs/imgui/", "libs/imgui/examples", "libs/SDL2/include", "libs/SDL2_image/include" }
    
    files { "game/src/*.cpp" }

    links { "SDL2", "SDL2main", "SDL2_image", "GLM", "GLAD", "ImGui", "Engine" }

    filter "system:linux"
        links { "dl", "pthread" }

        defines { "_X11" }

    filter "system:windows"
        defines { "_WINDOWS" }

include "libs/sdl2.lua"
include "libs/sdl2_image.lua"
include "libs/glad.lua"
include "libs/glm.lua"
include "libs/imgui.lua"