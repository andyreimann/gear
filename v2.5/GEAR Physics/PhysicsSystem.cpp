#include "PhysicsSystem.h"
#include "PhysicsComponent.h"

#include <G2/Logger.h>

#include <thread>

using namespace G2::Physics;


PhysicsSystem::PhysicsSystem() 
{
	mCollisionConfiguration = new btDefaultCollisionConfiguration();

}

void
PhysicsSystem::runPhase(std::string const& name, G2::FrameInfo const& frameInfo)
{
	if(name == "update") 
	{
		for (int i = 0; i < components.size() ; ++i) 
		{
			PhysicsComponent& comp = components[i];
			G2::logger << "Process PhysicsComponent " << comp.getEntityId() << " in thread " << std::this_thread::get_id() << G2::endl;
		}
	}
}

bool
PhysicsSystem::runsOnMainThread() 
{
	return false; // make it running on the side thread
}