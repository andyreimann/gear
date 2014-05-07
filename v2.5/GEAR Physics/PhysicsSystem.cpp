#include "PhysicsSystem.h"
#include "PhysicsComponent.h"

#include <G2/Logger.h>

using namespace G2::Physics;


void
PhysicsSystem::runPhase(std::string const& name, G2::FrameInfo const& frameInfo)
{
	if(name == "render") 
	{
		
		for (int i = 0; i < components.size() ; ++i) 
		{
			PhysicsComponent& comp = components[i];
			G2::logger << "Process PhysicsComponent " << (i+1) << G2::endl;
		}
	}
}
