// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"

#include "Intersection.h"
#include "UberShaderParser.h"
#include "EventDistributer.h"
#include "AABB.h"

// ECS INCLUDES
#include "Entity.h"
#include "ECSManager.h"
#include "RenderSystem.h"
#include "RenderComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"

// Importer
#include "TextureImporter.h"
#include "MD5Importer.h"
#include "FBXImporter.h"
#include "EffectImporter.h"
#include "FrameInfo.h"

// TEXTURING
#include "Texture3D.h"

// Library search
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "IL.lib")
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "cg.lib")
#pragma comment(lib, "cgGL.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "libfbxsdk-md.lib")


//#pragma comment(lib, "OpenCL.lib")

#ifndef _DEBUG
	#pragma comment(lib, "OIS.lib")
#else
	#pragma comment(lib, "OIS_d.lib")
#endif
#include "AbstractWindow.h"
extern "C" {

	void G2_init();
	/** This function manages the render loop using an AbstractWindow instance
	 * @param window The AbstractWindow to manage the render loop for.
	 */
	void G2_loop(G2::AbstractWindow& window);
}