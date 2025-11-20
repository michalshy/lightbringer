project "SDL2main"
	kind          "StaticLib"
	language      "C"

	systemversion "latest"

	flags {
		          "NoRuntimeChecks",      -- Only used on Visual Studio.
		          "NoBufferSecurityCheck"
	}

	vectorextensions "SSE"                -- Necessary to run x32.

	location         "Intermediate/ProjectFiles/%{_ACTION}"

	targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}/SDL2main"

	includedirs     {"sdl2/include"}

	files           {"sdl2/src/main/windows/*.c"}	

	systemversion    "latest"
	defines {
					 "_WINDOWS",
					 "WIN32"
	}

	filter "configurations:Debug"
		defines {
			         "_DEBUG"
		}
		runtime      "Debug"
		symbols      "On"

	filter "configurations:Release"
		defines {
			         "NDEBUG"
		}
		runtime      "Release"
		optimize     "Speed"


	filter "configurations:Development"    -- These are the configurations I tend to
		defines {                          -- use in my projects, but I have added 
			         "NDEBUG"              -- the default ones anyway.
		}
		runtime      "Release"
		optimize     "On"

	filter "configurations:Ship"
		defines {
			         "NDEBUG"
		}
		runtime      "Release"
		optimize     "Speed"