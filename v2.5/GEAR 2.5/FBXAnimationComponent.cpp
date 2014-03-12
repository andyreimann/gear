// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "FBXAnimationComponent.h"
#include "Logger.h"

using namespace G2;

FBXAnimationComponent::FBXAnimationComponent() 
	: fbxScene(nullptr)
{ }

FBXAnimationComponent::FBXAnimationComponent(FbxScene* fbxScene, FBXAnimationState const& animationState, FBXAnimationData const& animationData) 
	: fbxScene(fbxScene),
	animationState(animationState),
	animationData(animationData)
{ }

FBXAnimationComponent::FBXAnimationComponent(FBXAnimationComponent && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

FBXAnimationComponent& FBXAnimationComponent::operator=(FBXAnimationComponent && rhs) 
{
	fbxScene = rhs.fbxScene;
	animationData = std::move(rhs.animationData);
	animationState = std::move(rhs.animationState);
	rhs.fbxScene = nullptr;
	
	return static_cast<FBXAnimationComponent&>(BaseComponent::operator=(std::move(rhs)));
}

void
FBXAnimationComponent::tempSetPoseIndex(unsigned int poseIndex) 
{
    const int lAnimStackCount = animationData.animStackNameArray.GetCount();
	logger << "[FBXAnimationComponent] Change pose to " << poseIndex << endl;
    if (poseIndex >= (unsigned int)lAnimStackCount)
    {
        return;
    }
	// select the base layer from the animation stack
	FbxAnimStack * animationStack = fbxScene->FindMember<FbxAnimStack>(animationData.animStackNameArray[poseIndex]->Buffer());
	if (animationStack == NULL)
	{
		// this is a problem. The anim stack should be found in the scene!
		logger << "[FBXAnimationComponent] Error: No AnimationStack found in FBX-Scene! " << endl;
		return;
	}

	animationState.currentAnimLayer = animationStack->GetMember<FbxAnimLayer>();
	animationState.currentAnimationStack = animationStack;
	fbxScene->SetCurrentAnimationStack(animationState.currentAnimationStack);

	animationState.poseIndex = poseIndex;
	// we assume that the first animation layer connected to the animation stack is the base layer
	// (this is the assumption made in the FBXSDK)

	// init animation state
	FbxTakeInfo* lCurrentTakeInfo = fbxScene->GetTakeInfo(*(animationData.animStackNameArray[animationState.poseIndex]));
	if (lCurrentTakeInfo)
	{
		animationState.start = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
		animationState.stop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
	}
	else
	{
		// Take the time line value
		FbxTimeSpan lTimeLineTimeSpan;
		fbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);

		animationState.start = lTimeLineTimeSpan.GetStart();
		animationState.stop  = lTimeLineTimeSpan.GetStop();
	}

   // check for smallest start with cache start
   if(animationData.cacheStart < animationState.start)
	   animationState.start = animationData.cacheStart;

   // check for biggest stop with cache stop
   if(animationData.cacheStop  > animationState.stop)  
	   animationState.stop  = animationData.cacheStop;

   animationState.currentTime = animationState.start;
}