if os.target() == "linux" then
    require "premake-plugins/export-compile-commands"
end

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

    -- Windows-only settings (do NOT affect Linux)
    filter "system:windows"
        architecture "x86_64"
        system "windows"
    
    filter "*"

project "Engine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
	architecture "x86_64"
    targetdir "build/engine/%{cfg.buildcfg}"
    objdir "build/engine/%{cfg.buildcfg}"

    includedirs { 
        "libs/entt/single_include/",
        "libs/glad/include/", 
        "libs/glm/", 
        "libs/imgui/", 
        "libs/imgui/examples", 
        "libs/SDL2/include", 
        "libs/SDL2_image/include", 
        "libs/spdlog/include",
        "engine/src",
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
    defines {
        "ENGINE_BUILD"
    }
    filter "system:windows"
        pchheader "engine_pch.h"
        pchsource "engine/src/engine_pch.cpp"
        libdirs {
            "libs/SDL2/build/%{cfg.buildcfg}",
            "libs/SDL2_image/build/%{cfg.buildcfg}"
        }
        
        links { "GLM", "ImGui", "spdlog" }
        
    filter "system:linux"
        links { "GL", "GLEW", "dl", "pthread", "SDL2", "SDL2_image" }
        defines { "HW_LINUX" }
        
    filter "system:windows"
        links { "GLAD" }
        defines { "HW_WIN" }
    

    filter { "system:windows", "configurations:Debug" }
        links { "SDL2d", "SDL2maind", "SDL2_imaged" }

    filter { "system:windows", "configurations:Release" }
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

    includedirs { 
        "libs/glad/include/", 
        "libs/glm/", 
        "libs/imgui/", 
        "libs/imgui/examples", 
        "libs/SDL2/include", 
        "libs/SDL2_image/include", 
        "engine/src", 
        "game/src",
        "libs/entt/single_include/", 
        "libs/spdlog/include"
    }

    links { "Engine", "GLM", "ImGui", "spdlog" }

    files { "game/src/**.cpp", "game/src/**.h" }
    
    filter "system:windows"
    libdirs {
        "libs/SDL2/build/%{cfg.buildcfg}",
        "libs/SDL2_image/build/%{cfg.buildcfg}"
    }

    filter "system:linux"
        links { "GL", "GLEW", "dl", "pthread", "SDL2", "SDL2_image" }
        defines { "HW_LINUX" }

    filter "system:windows"
        links { "GLAD" }
        defines { "HW_WIN" }

    filter { "system:windows", "configurations:Debug" }
        links { "SDL2d", "SDL2maind", "SDL2_imaged" }

    filter { "system:windows", "configurations:Release" }
        links { "SDL2", "SDL2main", "SDL2_image" }

    filter "system:linux"
        links { "dl", "pthread" }
        defines { "_X11" }

    filter "system:windows"
        defines { "_WINDOWS" }
    
    filter "system:windows"
        postbuildcommands {
            -- Copy game resources
            '{COPY} res "%{cfg.targetdir}/res"',

            -- Copy Engine DLL
            '{COPY} "%{wks.location}/build/engine/%{cfg.buildcfg}/Engine.dll" "%{cfg.targetdir}"',

            -- Copy SDL DLLs
            '{COPY} "%{wks.location}/libs/SDL2/build/%{cfg.buildcfg}/*.dll" "%{cfg.targetdir}"',
            '{COPY} "%{wks.location}/libs/SDL2_image/build/%{cfg.buildcfg}/*.dll" "%{cfg.targetdir}"'
        }

    filter "system:linux"
        postbuildcommands {
            '{COPY} res/ "%{cfg.targetdir}/res"'
        }

    filter "action:vs*"
        buildoptions { "/utf-8" }
    
    filter "configurations:Debug"
        symbols "On"
        optimize "Debug"
    filter "configurations:Debug"
        buildoptions { "/Oy-" }
        
include "libs/glm.lua"
include "libs/imgui.lua"
include "libs/spdlog.lua"

if os.target() == "windows" then
    include "libs/glad.lua"
end