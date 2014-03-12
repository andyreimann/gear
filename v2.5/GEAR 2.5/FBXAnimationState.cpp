// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "FBXAnimationState.h"

using namespace G2;

FBXAnimationState::FBXAnimationState() 
	: poseIndex(-1),
	currentAnimLayer(nullptr),
	currentTime(start),
	start(FBXSDK_TIME_INFINITE),
	stop(FBXSDK_TIME_MINUS_INFINITE),
	pause(true),
	timeSinceLastAnimationFrame(0.0),
	updateFirstFrame(true)
{ }

FBXAnimationState::FBXAnimationState(int poseIndex, 
									 FbxAnimLayer* currentAnimLayer,
									 FbxAnimStack* currentAnimationStack, 
									 FbxTime const& start, 
									 FbxTime const& stop, 
									 bool pause) 
	: poseIndex(poseIndex),
	currentAnimLayer(currentAnimLayer),
	currentAnimationStack(currentAnimationStack),
	currentTime(start),
	start(start),
	stop(stop),
	pause(pause),
	timeSinceLastAnimationFrame(0.0),
	updateFirstFrame(true)
{ }