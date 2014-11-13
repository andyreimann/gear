solution "GEAR 2.5-premake"
  platforms { "x64", "x32" }
  configurations { "Debug", "Release" }
  location ""

configuration { "debug", "x32" }
  targetdir "lib/x32/Debug"

configuration { "release", "x32" }
  targetdir "lib/x32/Release"

configuration { "debug", "x64" }
  targetdir "lib/x64/Debug"

configuration { "release", "x64" }
  targetdir "lib/x64/Release"

project "GEAR Core Premake"
  language "C++"
  kind "SharedLib"
  location "GEAR Core"
  targetname    "GEARCore"

  files { "GEAR Core/**.h","GEAR Core/**.cpp" }

  postbuildcommands { 
    "XCOPY \"*.h\" \"..\\include\\G2Core\\\" /R /I /Y", 
    "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\GEARCore.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\""
  }
  
  includedirs { "include" }
  
 -- pchheader "Pch.h"
 -- pchsource "GLFW Win/ASframework/Pch.cpp"
  
  defines { "COREDLL_EXPORTS" }

  buildoptions { "/DEBUG" }

  configuration "Debug"
     buildoptions { "/MP8" }  
     libdirs { "../lib/" }
     optimize "Off"

  configuration "Release"
     flags { "NoFramePointer" }
     libdirs { "../lib/" }
     optimize "Speed"


 -- configuration { "debug", "x32" }
 --   targetdir "lib/x32/Debug"

 -- configuration { "release", "x32" }
 --   targetdir "lib/x32/Release"


 -- configuration { "release", "x64" }
 --   targetdir "lib/x64/Release"

  configuration {} -- Clear configuration


project "GEAR 2.5 Premake"
  language "C++"
  kind "StaticLib"
  location "GEAR 2.5"
  targetname    "GEAR"

 files { "GEAR 2.5/**.h","GEAR 2.5/**.cpp" }
  
  includedirs { "include" }
  
 -- pchheader "Pch.h"
 -- pchsource "GLFW Win/ASframework/Pch.cpp"
  
  defines { "AS_FUNCDLL_EXPORT" }

 buildoptions { "/DEBUG" }
 -- flags { "Symbols", "FatalWarnings" }
 -- warnings "Extra"

  configuration "Debug"
      buildoptions { "/MP8" }  
     libdirs { "../lib/" }
     optimize "Off"

 configuration "Release"
     flags { "NoFramePointer" }
     libdirs { "../lib/" }
     optimize "Speed"

 configuration {} -- Clear configuration

 links { "GEARCore.lib" }