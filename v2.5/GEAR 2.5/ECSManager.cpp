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
