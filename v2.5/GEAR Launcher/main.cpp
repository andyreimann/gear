#include "Launcher.h"
// hide console
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#ifdef _WIN32
	// in SDLmain.lib is an SDL_main entry point
	// we have to disable it
	// @see http://www.spieleprogrammierer.de/phpBB2/viewtopic.php?t=8526
	#undef main
#endif 

int main(int argc, char *argv[])
{
	G2_loadGfxDevice("GEARGL.dll");

	G2::SDL::Window window("GEAR Launcher",1024,768,false);

	G2_gfxDevice()->init(nullptr); 
	G2_init();
	{
		G2::Launcher launcher(window);

		G2_loop(window); 
	}
	G2_shutdown();

	return 0;
}
