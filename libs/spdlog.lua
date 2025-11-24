project "spdlog"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir "../bin/%{cfg.buildcfg}"
    objdir "../obj/%{cfg.buildcfg}"

    -- Correct include paths (relative to THIS file)
    includedirs {
        "spdlog/include",
        "spdlog/include/spdlog",
        "spdlog/include/spdlog/fmt/bundled"
    }

    -- Correct source files (relative to THIS file)
    files {
        "spdlog/include/spdlog/**.h",
        "spdlog/src/**.cpp"
    }

    defines { "SPDLOG_COMPILED_LIB" }

    filter "system:linux"
        pic "On"

    filter "system:windows"
        defines { "_WINDOWS" }
        links { "user32", "gdi32", "winmm", "imm32", "version", "ole32", "oleaut32", "setupapi" }

    filter "action:vs*"
        buildoptions { "/utf-8" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
