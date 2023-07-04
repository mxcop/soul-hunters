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

    -- Include directories containing only '.h' files.
    includedirs {
        "../include/",
        "../vendor/glfw/include/",
        "../vendor/glad/include/",
        "../vendor/glm/",
    }
    
    -- Project source files. (.cpp & .h)
    files { "../src/*.cpp", "../src/*.h", "../src/**/*.cpp", "../src/**/*.h" }

    -- Library links.
    links { 
        "GLFW",
        "GLAD",
        "GLM",
    }

    filter "system:linux"
        links { "dl", "pthread", "GL", "X11" }

        defines { "_X11" }

    filter "system:windows"
        links { "OpenGL32" } -- OpenGL

        defines { "_WINDOWS" }

    -- Library includes.
    include "../vendor/glfw.lua"
    include "../vendor/glad.lua"
    include "../vendor/glm.lua"