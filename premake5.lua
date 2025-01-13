-- @file premake5.lua

-- Workspace Settings
workspace "project-tm"
    language "C++"
    cppdialect "C++23"
    filter { "system:linux" }
        cppdialect "gnu++23"
    filter {}
    location "./generated"
    configurations { "debug", "release", "distribute" }
    filter { "configurations:debug" }
        defines { "TM_DEBUG" }
        symbols "On"
    filter { "configurations:release" }
        defines { "TM_RELEASE" }
        optimize "On"
    filter { "configurations:distribute" }
        defines { "TM_DISTRIBUTE" }
        optimize "On"
    filter { "system:linux" }
        defines { "TM_LINUX" }
    filter {}

    -- CPU Emulator Backend
    project "tmc"
        kind "StaticLib"
        location "./generated/tmc"
        targetdir "./build/bin/tmc/%{cfg.buildcfg}"
        objdir "./build/obj/tmc/%{cfg.buildcfg}"
        pchheader "./projects/tmc/include/TMC.Precompiled.hpp"
        pchsource "./projects/tmc/src/TMC.Precompiled.cpp"
        includedirs {
            "./projects/tmc/include"
        }
        files {
            "./projects/tmc/src/TMC.*.cpp"
        }

    -- CPU Assembler Tool
    project "tmm"
        kind "ConsoleApp"
        location "./generated/tmm"
        targetdir "./build/bin/tmm/%{cfg.buildcfg}"
        objdir "./build/obj/tmm/%{cfg.buildcfg}"
        pchheader "./projects/tmm/include/TMM.Precompiled.hpp"
        pchsource "./projects/tmm/src/TMM.Precompiled.cpp"
        includedirs {
            "./projects/tmc/include",
            "./projects/tmm/include"
        }
        files {
            "./projects/tmm/src/TMM.*.cpp"
        }
        libdirs {
            "./build/bin/tmc/%{cfg.buildcfg}"
        }
        links { 
            "tmc", "m"
        }
