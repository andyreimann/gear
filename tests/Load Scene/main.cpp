#include "TestScene.h"

#include <G2/SDLWindow.h>
#include <G2/GEAR.h>
#include <G2/Logger.h>

#ifdef _WIN32
	// in SDLmain.lib is an SDL_main entypoint
	// we have to disable it
	// @see http://www.spieleprogrammierer.de/phpBB2/viewtopic.php?t=8526
	#undef main
#endif 

int main(int argc, char *argv[])
{
	
	G2::SDLWindow window("Engine Test",1600,768,false);
	
	G2_init();

	TestScene testScene(window);

	G2_loop(window);

	return 0;
}
