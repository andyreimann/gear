// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include <fbxsdk.h>

namespace G2 
{
	struct FBXAnimationData 
	{
		FBXAnimationData();
		FBXAnimationData(FbxTime const& cacheStart,
						 FbxTime const& cacheStop,
						 FbxArray<FbxString*> const& animationStackNameArray,
						 double const& frameRate);

		void setFrameRate(double const& frameRate_);

		FbxTime					cacheStart;
		FbxTime					cacheStop;
		FbxArray<FbxString*>	animStackNameArray;	// The name array of the available animations
		double					frameRate;			// The inverse frame rate in seconds
		double					invFrameRate;		// The inverse frame rate in seconds
	};
};