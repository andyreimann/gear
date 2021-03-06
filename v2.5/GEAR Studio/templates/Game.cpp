#include "Game.h"

using namespace G2;

std::shared_ptr<Game> Game::mInstance_ = std::shared_ptr<Game>();

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
	mCurrentScene(""),
	mProjectRoot("../../..")
{
	G2::EventDistributer::onRenderFrame.hook(this, &Game::_onRenderFrame);
}

Game::~Game()
{
	G2::EventDistributer::onRenderFrame.unHookAll(this);
}

void
Game::startSceneAsync(std::string const& name) 
{
	getShared()._scheduleSceneLoading(name);
}

bool
Game::hasEntity(std::string const& name) const
{
	return mManagedEntities.count(name) == 1;
}

G2::Entity*
Game::getEntity(std::string const& name) 
{
	auto it = mManagedEntities.find(name);
	return it != std::end(mManagedEntities) ? &it->second : nullptr;
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
		// destroy all currently loaded entities and all components, which are attached to them
		// Note that there should not be a need to actually clean up any System class.
		mManagedEntities = std::unordered_map<std::string,G2::Entity>();
		// load mCurrentScene
		_auto_generated_loadCurrentScene();
	}
}