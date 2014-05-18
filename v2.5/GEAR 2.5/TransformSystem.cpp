// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "TransformSystem.h"
#include "TransformComponent.h"

using namespace G2;

void
TransformSystem::runPhase(std::string const& name, FrameInfo const& frameInfo) 
{
	// nothing
	if(name == "update") 
	{
		for(auto i = 0; i < components.size(); ++i) 
		{
			// recalculate the local space matrix if not already done this frame
			components[i]._updateWorldSpaceMatrix(frameInfo.frame);
		}
	}
}