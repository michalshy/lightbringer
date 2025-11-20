project "spdlog"
    kind "StaticLib"
    language "C++"
    architecture "x86_64"
    cppdialect "C++17"
    staticruntime "off"

    targetdir "bin/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"

    files {
        "spdlog/include/spdlog/**.h",
        "spdlog/src/**.cpp"
    }

    includedirs {
        "spdlog/include"
    }

	defines {
        "SPDLOG_COMPILED_LIB"
    }

	filter "action:vs*"
        buildoptions { "/utf-8" }

    filter "system:windows"
        defines { "_WINDOWS" }
        links { "user32", "gdi32", "winmm", "imm32", "version", "ole32", "oleaut32", "setupapi" }
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "full"