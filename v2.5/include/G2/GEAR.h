// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"
#include <G2Core/G2Core.h>

#include "Intersection.h"
#include "UberShaderParser.h"
#include <G2Core/EventDistributer.h>
#include "AABB.h"

// ECS INCLUDES
#include <G2Core/Entity.h>
#include <G2Core/ECSManager.h>
#include "RenderSystem.h"
#include "RenderComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"
#include "NameComponent.h"
#include "TransformComponent.h"
#include "SplineAnimation.h"

// Importer
#include "TextureImporter.h"
#include "MD5Importer.h"
#include "FBXImporter.h"
#include "EffectImporter.h"

#include <G2Core/FrameInfo.h>

// TEXTURING
#include "Texture3D.h"

// Library search
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "freeglut.lib")
#ifdef _DEBUG
	#pragma comment(lib, "glew_d.lib")
#else
	#pragma comment(lib, "glew.lib")
#endif
#pragma comment(lib, "libfbxsdk-md.lib")

#include "AbstractWindow.h"

extern "C" {

	void G2_init();

	void G2_startSideThread();

	void G2_stopSideThread();

	/** This function manages to render one single frame using an AbstractWindow instance
	 * @param window The AbstractWindow to render one frame for.
	 * @param frameInfo The current frame information.
	 */
	void G2_singleFrame(G2::AbstractWindow& window, G2::FrameInfo& frameInfo);
	/** This function manages the render loop using an AbstractWindow instance
	 * @param window The AbstractWindow to manage the render loop for.
	 */
	void G2_loop(G2::AbstractWindow& window);
	/** This function releases all engine related resources.
	 */
	void G2_shutdown();
}