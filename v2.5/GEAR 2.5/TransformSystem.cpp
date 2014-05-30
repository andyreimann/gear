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
	// nothing
	//if(name == "render") 
	//{
	//	G2::logger << "T in " << frameInfo.frame << G2::endl;
	//}
	if(name == "update") 
	{
		auto* renderSystem = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>();
		for(auto i = 0; i < components.size(); ++i) 
		{
			TransformComponent& comp = components[i];
			// recalculate the local space matrix if not already done this frame
			comp.updateWorldSpaceMatrix(frameInfo.frame);
			auto* renderComponent = renderSystem->get(comp.getEntityId());
			if(renderComponent != nullptr)
			{
				// update all world space AABBs
				// TODO Only when transformComponent is really updated!!!!!!!
				if(renderComponent->objectSpaceAABBs.size() > 0)
				{
					if(renderComponent->objectSpaceAABBs.size() != renderComponent->worldSpaceAABBs.size())
					{
						renderComponent->worldSpaceAABBs.resize(renderComponent->objectSpaceAABBs.size());
					}
					for(auto c = 0; c < renderComponent->objectSpaceAABBs.size(); ++c)
					{
						renderComponent->worldSpaceAABBs[c] = std::move(renderComponent->objectSpaceAABBs[c].transform(comp.getWorldSpaceMatrix()));
					}
				}
			}
		}
	}
}