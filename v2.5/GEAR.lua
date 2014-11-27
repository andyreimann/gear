solution "GEAR 2.5"
  platforms { "x64", "x32" }
  configurations { "Debug", "Release" }
  location ""
  targetdir "lib/%{cfg.platform}/%{cfg.buildcfg}"
  libdirs { "lib/%{cfg.platform}/%{cfg.buildcfg}" }
  includedirs { "include", "." }
  defines { "_CRT_SECURE_NO_WARNINGS" }

  configuration "Debug"
    buildoptions { "/MP8", "/MDd" }  
    flags{ "Symbols" }
    optimize "Off"

  configuration "Release"
    flags { "NoFramePointer" }
    optimize "Speed"
  
  configuration {} -- Clear configuration

------------------- GEAR Core -----------------------------------------------------

  project "GEAR Core"
    language "C++"
    kind "SharedLib"
    location "GEAR Core"
    targetname "GEARCore"
    files { "GEAR Core/**.h","GEAR Core/**.cpp" }

    postbuildcommands { 
      "XCOPY \"*.h\" \"..\\include\\G2Core\\\" /R /I /Y"
      , 
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\GEARCore.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\""
    }

    defines { "COREDLL_EXPORTS" }

    configuration {} -- Clear configuration

------------------- GEAR 2.5 -----------------------------------------------------

  project "GEAR 2.5"
    language "C++"
    kind "StaticLib"
    location "GEAR 2.5"
    targetname "GEAR"
    files { "GEAR 2.5/**.h","GEAR 2.5/**.cpp" }
    links { "GEAR Core", "OpenGL32", "glu32", "freeglut", "libfbxsdk-md.lib" }

    postbuildcommands { 
      "XCOPY \"*.h\" \"..\\include\\G2\\\" /R /I /Y", 
    }

    configuration "Debug"
      links { "glew_d" }

    configuration "Release"
      links { "glew" }

    configuration {} -- Clear configuration

------------------- GEAR Bullet Physics -----------------------------------------------------

  project "GEAR Bullet Physics"
    language "C++"
    kind "StaticLib"
    implibdir "lib/%{cfg.platform}/%{cfg.buildcfg}"
    location "GEAR Bullet Physics"
    targetname "GEARBulletPhysics"
    files { "GEAR Bullet Physics/**.h","GEAR Bullet Physics/**.cpp" }
    links { "GEAR Core","GEAR 2.5", "OpenGL32", "BulletSoftBody", "BulletDynamics", "BulletCollision", "LinearMath" }

    postbuildcommands { 
      "XCOPY \"*.h\" \"..\\include\\G2BulletPhysics\\\" /R /I /Y", 
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\pcre*.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\""
    }

    configuration {} -- Clear configuration

------------------- GEAR Newton Physics -----------------------------------------------------

  project "GEAR Newton Physics"
    language "C++"
    kind "StaticLib"
    location "GEAR Newton Physics"
    targetname "GEARNewtonPhysics"
    files { "GEAR Newton Physics/**.h","GEAR Newton Physics/**.cpp" }

    postbuildcommands { 
      "XCOPY \"*.h\" \"..\\include\\G2NewtonPhysics\\\" /R /I /Y", 
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\newton*.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\""
    }

    configuration "Debug"
      links { "GEAR Core", "GEAR 2.5", "newton_d", "dMath_d" }

    configuration "Release"
      links { "GEAR Core", "newton", "dMath" }

    configuration {} -- Clear configuration

------------------- GEAR Cameras -----------------------------------------------------

  project "GEAR Cameras"
    language "C++"
    kind "SharedLib"
    location "GEAR Cameras"
    targetname "GEARCameras"
    files { "GEAR Cameras/**.h","GEAR Cameras/**.cpp" }
    links { "GEAR Core", "GEAR 2.5", "OpenGL32", "glu32" }

    postbuildcommands { 
      "XCOPY \"*.h\" \"..\\include\\G2Cameras\\\" /R /I /Y", 
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\GEARCameras.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\""
    }
    
    defines { "CAMERASDLL_EXPORTS" }

    configuration {} -- Clear configuration

------------------- GEAR CEGUI -----------------------------------------------------

  project "GEAR CEGUI"
    language "C++"
    kind "SharedLib"
    location "GEAR CEGUI"
    targetname "GEARCegui"
    files { "GEAR CEGUI/**.h","GEAR CEGUI/**.cpp" }
    links { "GEAR Core", "GEAR 2.5", "OpenGL32", "glu32" }

    postbuildcommands { 
      "XCOPY \"*.h\" \"..\\include\\G2Cegui\\\" /R /I /Y", 
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\GEARCegui.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\"",
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\CEGUI*.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\"",
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\expat*.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\"",
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\freetype*.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\"",
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\jpeg*.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\"",
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\libpng*.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\"",
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\SILLY*.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\"",
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\zlib*.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\""
    }
    
    defines { "UICEGUIDLL_EXPORTS" }

    configuration "Debug"
      links { "glew_d", "CEGUIBase-0_d", "CEGUIOpenGLRenderer-0_d" }

    configuration "Release"
      links { "glew", "CEGUIBase-0", "CEGUIOpenGLRenderer-0" }

    configuration {} -- Clear configuration

------------------- GEAR Editor -----------------------------------------------------

  project "GEAR Editor"
    language "C++"
    kind "SharedLib"
    location "GEAR Editor"
    targetname "GEAREditor"
    files { "GEAR Editor/**.h","GEAR Editor/**.cpp" }
    links { "GEAR Core", "GEAR 2.5", "GEAR Cameras", "GEAR CEGUI", "OpenGL32", "glu32" }

    postbuildcommands { 
      "XCOPY \"*.h\" \"..\\include\\G2Editor\\\" /R /I /Y", 
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\GEAREditor.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\""
    }
    
    defines { "EDITORDLL_EXPORTS" }

    configuration "Debug"
      links { "CEGUIBase-0_d.lib", "CEGUIOpenGLRenderer-0_d.lib" }

    configuration "Release"
      links { "CEGUIBase-0.lib", "CEGUIOpenGLRenderer-0.lib" }

    configuration {} -- Clear configuration

------------------- GEAR GFX DX 11 -----------------------------------------------------

  project "GEAR GFX DX 11"
    language "C++"
    kind "SharedLib"
    location "GEAR DX 11"
    targetname "GEARDX11"
    files { "GEAR DX 11/**.h","GEAR DX 11/**.cpp" }
    links { "GEAR Core", "GEAR 2.5", "d3d11", "d3dx11", "d3dx10", "cg", "cgD3D11" }
    includedirs { "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include" }

    postbuildcommands { 
      "XCOPY \"*.h\" \"..\\include\\G2DX11\\\" /R /I /Y", 
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\GEARDX11.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\""
    }

    -- TODO add for x32 as well!
    libdirs { "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x64" }
    
    defines { "GEARDX11DLL_EXPORTS" }

    configuration {} -- Clear configuration

------------------- GEAR GFX GL 3 -----------------------------------------------------

  project "GEAR GFX GL 3"
    language "C++"
    kind "SharedLib"
    location "GEAR OpenGL"
    targetname "GEARGL"
    files { "GEAR OpenGL/**.h","GEAR OpenGL/**.cpp" }
    links { "GEAR Core", "OpenGL32", "glu32", "freeglut", "IL", "DevIL", "cg", "cgGL" }

    postbuildcommands { 
      "XCOPY \"*.h\" \"..\\include\\G2GL\\\" /R /I /Y", 
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\GEARGL.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\"",
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\glew*.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\"",
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\DevIL.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\""
    }
    
    defines { "GEAROPENGLDLL_EXPORTS" }

    configuration "Debug"
      links { "glew_d" }

    configuration "Release"
      links { "glew" }

    configuration {} -- Clear configuration

------------------- GEAR SDL Context -----------------------------------------------------

  project "GEAR SDL Context"
    language "C++"
    kind "SharedLib"
    location "GEAR SDL Context"
    targetname "GEARSdlContext"
    files { "GEAR SDL Context/**.h","GEAR SDL Context/**.cpp" }
    links { "GEAR Core", "GEAR 2.5", "SDL2", "OpenGL32", "glu32" }

    postbuildcommands { 
      "XCOPY \"*.h\" \"..\\include\\G2SdlContext\\\" /R /I /Y", 
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\GEARSdlContext.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\"",
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\SDL*.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\"",
    }
    
    defines { "SDLCONTEXTDLL_EXPORTS" }

    configuration {} -- Clear configuration

------------------- GEAR Terrains -----------------------------------------------------

  project "GEAR Terrains"
    language "C++"
    kind "SharedLib"
    location "GEAR Terrains"
    targetname "GEARTerrains"
    files { "GEAR Terrains/**.h","GEAR Terrains/**.cpp" }
    links { "GEAR Core", "GEAR 2.5", "OpenGL32", "glu32" }

    postbuildcommands { 
      "XCOPY \"*.h\" \"..\\include\\G2Terrains\\\" /R /I /Y", 
      "Copy /Y \"..\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\GEARTerrains.dll\" \"..\\..\\tests\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\""
    }
    
    defines { "TERRAINSDLL_EXPORTS" }

    configuration "Debug"
      links { "glew_d" }

    configuration "Release"
      links { "glew" }

    configuration {} -- Clear configuration
