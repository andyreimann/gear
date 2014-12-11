#include "Launcher.h"

using namespace G2;

Launcher::Launcher(G2::SDL::Window& window)
	: mWindow(window),
	mGameRuntime("GameRuntime.dll")
{
	// read resources.cfg
	// load additional libraries via late binding

	// load start scene

	mGameRuntime.loadScene("MyScene");
	
	if(mGameRuntime.postLoadScene != nullptr)
	{
		mGameRuntime.postLoadScene("MyScene");
	}
	/*
		Things we need to solve:
			- Provide a central function or event accessible inside the GameRuntime.dll 
			  which can initiate the loading of a different scene and thus an unloading of the current scene.

			  inside of any script of the GameRuntime.dll, it should be possible to do:
			  		G2::Game::startSceneAsync("..."); // loads the scene before the next frame starts

			  In Fact:
			  	- GEAR Launcher uses function pointers to load the very first scene
			  	- Code in GameRuntime.dll uses G2::Game::startSceneAsync("...") to load new scenes
	*/
}

Launcher::~Launcher()
{
}
