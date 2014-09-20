#include "DX11Window.h"

#include <G2/GEAR.h>
#include <G2/Logger.h>

#include <G2GL/GlGfxDevice.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

int main(int argc, char *argv[])
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF));
	
	G2_loadGfxDevice("GEARDX11.dll"); 

	DX11Window window("DX11 Test",1600,768,false);

	G2_init();
	{
		
		//CeguiTest test(window);

		G2_loop(window);
	}
	G2_shutdown();

	return 0;
}
