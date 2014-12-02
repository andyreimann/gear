// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "TransformSystem.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "Logger.h"

using namespace G2;

void
TransformSystem::runPhase(std::string const& name, FrameInfo const& frameInfo) 
{
	if(name == "update") 
	{
		auto* renderSystem = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>();
		for(auto i = 0; i < components.size(); ++i) 
		{
			TransformComponent& comp = components[i];

			// step a may assigned spline animation

			// recalculate the local space matrix if not already done this frame
			comp.updateWorldSpaceMatrix(frameInfo.frame);
			if (comp.updated())
			{
				// TransformComponent was updated this frame
				auto* renderComponent = renderSystem->get(comp.getEntityId());
				if(renderComponent != nullptr)
				{
					// update all world space AABBs
					for(unsigned int c = 0; c < renderComponent->getNumDrawCalls(); ++c)
					{
						DrawCall& drawCall = renderComponent->getDrawCall(c);

						drawCall.mWorldSpaceAABB = std::move(drawCall.getModelSpaceAABB().transform(comp.getWorldSpaceMatrix()));
					}
				}
			}
		}
	}
}