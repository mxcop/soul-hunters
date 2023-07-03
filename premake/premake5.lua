workspace "Game"
    location ".."
    configurations { "Debug", "Release" }
    startproject "Game"

    flags { "MultiProcessorCompile" }

    filter "configurations:Debug"
        defines { "DEBUG", "DEBUG_SHADER" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "Speed"
        flags { "LinkTimeOptimization" }

project "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
	architecture "x86_64"

    location ".."
    targetdir "../build/bin/%{cfg.buildcfg}"
    objdir "../build/obj/%{cfg.buildcfg}"

    includedirs {
        "../vendor/glfw/include/",
        "../vendor/glad/include/",
        "../vendor/glm/",
        "../vendor/soil/include/",
    }
    
    files { "../src/*.cpp", "../src/*.h" }

    links { 
        "GLFW",
        "GLAD",
        "GLM",
        "SOIL",
    }

    filter "system:linux"
        links { "dl", "pthread" }

        defines { "_X11" }

    filter "system:windows"
        defines { "_WINDOWS" }

    include "../vendor/glfw.lua"
    include "../vendor/glad.lua"
    include "../vendor/glm.lua"
    include "../vendor/soil.lua"
