// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "FBXAnimationData.h"

using namespace G2;

FBXAnimationData::FBXAnimationData() 
	: cacheStart(FBXSDK_TIME_INFINITE),
	cacheStop(FBXSDK_TIME_MINUS_INFINITE),
	frameRate(24.0),
	invFrameRate(1.0 / 24.0)
{}

FBXAnimationData::FBXAnimationData(FbxTime const& cacheStart, 
								   FbxTime const& cacheStop, 
								   FbxArray<FbxString*> const& animationStackNameArray, 
								   double const& frameRate) 
	: cacheStart(cacheStart),
	cacheStop(cacheStop),
	animStackNameArray(animationStackNameArray),
	frameRate(frameRate),
	invFrameRate(1.0 / frameRate)
{ } 

void
FBXAnimationData::setFrameRate(double const& frameRate_) 
{
	frameRate = frameRate_;
	invFrameRate = 1.0 / frameRate;
}