#include "TerrainTest.h"

#include <G2/GEAR.h>
#include <G2/Logger.h>

#include <G2GL/GlGfxDevice.h>

#ifdef _WIN32
	// in SDLmain.lib is an SDL_main entry point
	// we have to disable it
	// @see http://www.spieleprogrammierer.de/phpBB2/viewtopic.php?t=8526
	#undef main
#endif 

int main(int argc, char *argv[])
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF));
	
	G2_loadGfxDevice("GEARGL.dll");

	G2::SDL::Window window("Terrain Test",1600,768,false);

	G2_gfxDevice()->init(nullptr);
	G2_init();
	{
		
		TerrainTest test(window);

		G2_loop(window);
	}
	G2_shutdown();

	return 0;
}
