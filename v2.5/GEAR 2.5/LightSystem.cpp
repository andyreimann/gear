#include "LightSystem.h"
#include "LightComponent.h"
#include "TransformSystem.h"
#include "TransformComponent.h"

using namespace G2;

void
LightSystem::runPhase(std::string const& name, FrameInfo const& frameInfo)
{
	if(name == "postUpdate") 
	{
		
		for (int i = 0; i < components.size() ; ++i) 
		{
			LightComponent& comp = components[i];

			if(!comp.isEnabled())
			{
				continue; // Light not ready
			}

			auto* transformation = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>()->get(comp.getEntityId());
			if(transformation != nullptr) 
			{
				// transform either position or orientation
				comp._updateTransformedPosition(comp._getUntransformedPosition() * transformation->getWorldSpaceMatrix());
			}
			else 
			{
				// no transformation applied
				comp._updateTransformedPosition(comp._getUntransformedPosition());
			}
		}
	}
}