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

ECSManager::ECSManager() 
{
	std::string mainThreadPhases[4] = {"preUpdate","update","postUpdate","render"};
	setMainThreadPhases(mainThreadPhases, 4);
	std::string sideThreadPhases[1] = {"updateSideThread"};
	setSideThreadPhases(sideThreadPhases, 1);
}

ECSManager::~ECSManager()
{
	for(int i = 0; i < mRegisteredSystems.size(); ++i) 
	{
		delete mRegisteredSystems[i];
		mRegisteredSystems[i] = nullptr;
	}
}

void
ECSManager::runMainThread(FrameInfo const& frameInfo) 
{
	for(size_t i = 0; i < mMainThreadPhases.size(); ++i)
	{
		EventDistributer::onPhaseStarted(mMainThreadPhases[i], frameInfo);
		mMainThreadUpdateEvent(mMainThreadPhases[i], frameInfo);
		EventDistributer::onPhaseEnded(mMainThreadPhases[i], frameInfo);
	}
}

void
ECSManager::runSideThread(FrameInfo const& frameInfo) 
{
	for(size_t i = 0; i < mSideThreadPhases.size(); ++i)
	{
		EventDistributer::onPhaseStarted(mSideThreadPhases[i], frameInfo);
		mSideThreadUpdateEvent(mSideThreadPhases[i], frameInfo);
		EventDistributer::onPhaseEnded(mSideThreadPhases[i], frameInfo);
	}
}

void
ECSManager::deleteComponentsForEntity(unsigned int entityId) 
{
	for(size_t i = 0; i < mRegisteredSystems.size(); ++i) 
	{
		mRegisteredSystems[i]->deleteComponentsForEntity(entityId);
	}
}

bool
ECSManager::setSideThreadPhases(std::string* phases, unsigned int numPhases) 
{
	// check if the default phases are given and the ordering is correct
	unsigned int defaultsFound = 0;
	for(unsigned int i = 0; i < numPhases; ++i)
	{
		if(defaultsFound == 0 && phases[i] == "updateSideThread")
		{
			++defaultsFound;
		}
	}
	if(defaultsFound == 1)
	{
		// everything is good
		mSideThreadPhases.clear();
		for(unsigned int i = 0; i < numPhases; ++i)
		{
			mSideThreadPhases.push_back(phases[i]);
		}
	}
	return defaultsFound == 1;
}
bool
ECSManager::setMainThreadPhases(std::string* phases, unsigned int numPhases) 
{
	// check if the default phases are given and the ordering is correct
	unsigned int defaultsFound = 0;
	for(unsigned int i = 0; i < numPhases; ++i)
	{
		if(defaultsFound == 0 && phases[i] == "preUpdate")
		{
			++defaultsFound;
		}
		else if(defaultsFound == 1 && phases[i] == "update")
		{
			++defaultsFound;
		}
		else if(defaultsFound == 2 && phases[i] == "postUpdate")
		{
			++defaultsFound;
		}
		else if(defaultsFound == 3 && phases[i] == "render")
		{
			++defaultsFound;
		}
	}
	if(defaultsFound == 4)
	{
		// everything is good
		mMainThreadPhases.clear();
		for(unsigned int i = 0; i < numPhases; ++i)
		{
			mMainThreadPhases.push_back(phases[i]);
		}
	}
	return defaultsFound == 4;
}