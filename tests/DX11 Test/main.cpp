#include "Scene.h"
#include <G2/GEAR.h>
#include <G2/Logger.h>

#include <G2GL/GlGfxDevice.h>
#include <G2SdlContext/SDLWindow.h>
#include "DX11Window.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

//#define USE_DX11 1

#ifdef _WIN32
	// in SDLmain.lib is an SDL_main entry point
	// we have to disable it
	// @see http://www.spieleprogrammierer.de/phpBB2/viewtopic.php?t=8526
	#undef main
#endif 

int main(int argc, char *argv[])
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF));
	
#ifndef USE_DX11
	#include <G2GL/GlGfxDevice.h>
	#pragma comment (lib, "GEARSDLContext.lib")

	G2_loadGfxDevice("GEARGL.dll"); 
	G2::SDL::Window window("DX11 Test - Renderer -> OpenGL",1600,768,false);
	G2_gfxDevice()->init(nullptr); 
#else

	G2_loadGfxDevice("GEARDX11.dll"); 
	DX11Window window("DX11 Test - Renderer -> D3D 11",1600,768,false); 
#endif



	G2_init();
	{
		Scene testScene(&window); 

		G2_loop(window);
	}
	G2_shutdown();

	return 0;
}
