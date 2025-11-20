workspace "Lightbringer"
    configurations { "Debug", "Release" }
    startproject "Lightbringer"
    staticruntime "off"

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
        "libs/glad/include/", 
        "libs/glm/", 
        "libs/imgui/", 
        "libs/imgui/examples", 
        "libs/SDL2/include", 
        "libs/SDL2_image/include", 
        "engine/src",
        "libs/entt/single_include",
        "libs/spdlog/include"
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
    filter "system:windows"
        libdirs {
            "libs/SDL2/build/%{cfg.buildcfg}",
            "libs/SDL2_image/build/%{cfg.buildcfg}"
        }

    links { "GLM", "GLAD", "ImGui", "spdlog" }
    filter "configurations:Debug"
        links { "SDL2d", "SDL2maind", "SDL2_imaged" }

    filter "configurations:Release"
        links { "SDL2", "SDL2main", "SDL2_image" }

    filter "system:linux"
        links { "dl", "pthread" }

        defines { "_X11" }

    filter "action:vs*"
        buildoptions { "/utf-8" }

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

    filter "system:windows"
        libdirs {
            "libs/SDL2/build/%{cfg.buildcfg}",
            "libs/SDL2_image/build/%{cfg.buildcfg}"
        }
        links { "SDL2", "SDL2main" }

    filter "system:linux"
        links { "dl", "pthread" }

        defines { "_X11" }


    filter "system:windows"
        defines { "_WINDOWS" }

include "libs/glad.lua"
include "libs/glm.lua"
include "libs/imgui.lua"
include "libs/spdlog.lua"