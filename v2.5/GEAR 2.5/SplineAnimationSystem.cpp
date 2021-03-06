#include "SplineAnimationSystem.h"
#include "SplineAnimation.h"
#include "TransformComponent.h"
#include <utility>

using namespace G2;

void
SplineAnimationSystem::runPhase(std::string const& name, FrameInfo const& frameInfo) 
{
	if (name == G2Core::ECSPhase::PREUPDATE)
	{
		auto* transformSystem = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>();
		for(auto i = 0; i < components.size(); ++i) 
		{
			SplineAnimation& comp = components[i];
			if(comp.isPaused())
			{
				continue;
			}
			transformSystem->lock();
			auto* transformComponent = transformSystem->get(comp.getEntityId());
			if(transformComponent == nullptr)
			{
				// create new one
				transformSystem->unlock();
				transformSystem->create(comp.getEntityId());
				transformSystem->lock();
				transformComponent = transformSystem->get(comp.getEntityId());
			}

			// do animation
			comp.animate(frameInfo.timeSinceLastFrame, transformComponent);

			transformSystem->unlock();
		}
	}
}