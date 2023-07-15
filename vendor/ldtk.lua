project "LDTK"
	kind "StaticLib"
	language "C++"
	architecture "x86_64"

	targetdir "../build/bin/ldtk/%{cfg.buildcfg}"
	objdir "../build/obj/ldtk/%{cfg.buildcfg}"
	
	includedirs { "ldtk-loader/include/" }

	files
	{
		"ldtk-loader/src/*.cpp",
        "ldtk-loader/src/*.hpp"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
