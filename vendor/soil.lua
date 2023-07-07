project "SOIL"
    kind "StaticLib"
    
    targetdir "../build/bin/soil/%{cfg.buildcfg}"
	objdir "../build/obj/soil/%{cfg.buildcfg}"

    includedirs { "soil/include/" }

    files { "soil/src/*.c" }

    filter "action:vs*"
        buildoptions { "/TP" }
        defines { "_CRT_SECURE_NO_WARNINGS" }

    filter "action:not vs*"
        language "C"
        buildoptions { "-Wall" }

    filter "configurations:debug"
        defines { "DEBUG" }
        symbols "On"
        targetname "soil-debug"

    filter "configurations:release"
        defines { "NDEBUG" }
        optimize "On"
        targetname "soil"

    filter "system:macosx"
        defines { "GL_SILENCE_DEPRECATION" }