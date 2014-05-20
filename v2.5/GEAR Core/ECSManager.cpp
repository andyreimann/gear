// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "ECSManager.h"

using namespace G2;

ECSManager* ECSManager::mInstance_ = NULL;

ECSManager&
ECSManager::getShared()
{
	if( !mInstance_ )
		mInstance_ = new ECSManager();
	return *mInstance_;
}

void ECSManager::destroy()
{
	if(mInstance_)
		delete mInstance_;
}

ECSManager::~ECSManager()
{
	for(int i = 0; i < mRegisteredSystems.size(); ++i) 
	{
		delete mRegisteredSystems[i];
	}
}

void
G2::ECSManager::runOnMainThread(std::string const& name, FrameInfo const& frameInfo) 
{
	mMainThreadUpdateEvent(name, frameInfo);
}

void
G2::ECSManager::runOnSideThread(std::string const& name, FrameInfo const& frameInfo) 
{
	mSideThreadUpdateEvent(name, frameInfo);
}

void
G2::ECSManager::deleteComponentsForEntity(unsigned int entityId) 
{
	for(size_t i = 0; i < mRegisteredSystems.size(); ++i) 
	{
		mRegisteredSystems[i]->deleteComponentsForEntity(entityId);
	}
}