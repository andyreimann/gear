// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "MD5AnimationComponent.h"

using namespace G2;

MD5AnimationComponent::MD5AnimationComponent() 
{ }

MD5AnimationComponent::MD5AnimationComponent(unsigned int numJointsInSkeleton) 
{
	// allocate space for the interpolated skeleton
	interpolatedSkeleton.joints.resize(numJointsInSkeleton);
}

MD5AnimationComponent::MD5AnimationComponent(MD5AnimationComponent && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

MD5AnimationComponent& MD5AnimationComponent::operator=(MD5AnimationComponent && rhs) 
{

	animationState = std::move(rhs.animationState);
	animationData = std::move(rhs.animationData);
	interpolatedSkeleton = std::move(rhs.interpolatedSkeleton);
	
	return static_cast<MD5AnimationComponent&>(BaseComponent::operator=(std::move(rhs)));
}
