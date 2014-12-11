#include "Game.h"

using namespace G2;

Game* Game::mInstance_ = std::shared_ptr<Game>();

Game&
Game::getShared()
{
	if( mInstance_.get() == nullptr )
		mInstance_ = std::shared_ptr<Game>(new Game());
	return *(mInstance_.get());
}

void Game::destroy()
{
	mInstance_ = std::shared_ptr<Game>();
}

Game::Game() :
	mCurrentScene("")
{
	G2::EventDistributer::onRenderFrame.hook(this, &Game::_onRenderFrame);
}

Game::~Game()
{
	G2::EventDistributer::onRenderFrame.unhookAll(this);
}

void
Game::startSceneAsync(std::string const& name) 
{
	// generated code goes here for loading the scene
	getShared()._scheduleSceneLoading(name);
}

void
Game::_scheduleSceneLoading(std::string const& sceneName) 
{
	if(sceneName != "")
	{
		mCurrentScene = sceneName;
		mLoadScene = true;
	}
}

void
Game::_onRenderFrame(G2::FrameInfo const&)
{
	if(mLoadScene)
	{
		// TODO Unload entire ECSManager
		// TODO load mCurrentScene
	}
}