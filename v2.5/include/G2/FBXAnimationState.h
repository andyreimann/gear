// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include <fbxsdk.h>

namespace G2 
{
	/** This class defines the data structure for an animation of a FBXMesh.
	 * @brief Defines the data structure for an animation of a FBXMesh.
	 * @author Andy Reimann
	 */
	struct FBXAnimationState
	{
		FBXAnimationState();
		FBXAnimationState(int poseIndex,
						  FbxAnimLayer* currentAnimLayer,
						  FbxAnimStack* currentAnimationStack,
						  FbxTime const& start,
						  FbxTime const& stop,
						  bool pause);

		int						poseIndex;						// The index of the currently used pose
		FbxAnimLayer*			currentAnimLayer;				// The pointer to the used animation layer
		FbxTime					currentTime;					// The current time of the animated pose
		FbxTime					start;							// The start time of the animated pose
		FbxTime					stop;							// The end time of the animated pose
		bool					pause;							// The animation pause flag
		bool					updateFirstFrame;				// The flag indicating whether the current pose was already animated at least one frame (to skip the base pose).
		double					timeSinceLastAnimationFrame;	// The elapsed time since the last animated frame
		FbxAnimStack *			currentAnimationStack;			// The currently used animation stack
	};
};