
solution "##PROJECTNAME##"
  platforms { "x64", "x32" }
  configurations { "Debug", "Release" }
  location ""
  targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
  libdirs { "lib/%{cfg.platform}/%{cfg.buildcfg}" }
  includedirs { "include", "." }
  defines { "_CRT_SECURE_NO_WARNINGS" }

  configuration "Debug"
    buildoptions { "/MP8", "/MDd" }  
    flags{ "Symbols", "NoMinimalRebuild" }
    optimize "Off"

  configuration "Release"
    buildoptions { "/MP8" }  
    flags { "NoFramePointer", "NoMinimalRebuild" }
    optimize "Speed"
  
  configuration {} -- Clear configuration

------------------- GEAR Core -----------------------------------------------------

  project "##PROJECTNAME## Core"
    language "C++"
    kind "SharedLib"
    location "src"
    targetname "GameRuntime"
    files { "src/**.h","src/**.cpp","generated-src/**.h","generated-src/**.cpp" }

    defines { "DLL_EXPORTS" }

    configuration {} -- Clear configuration