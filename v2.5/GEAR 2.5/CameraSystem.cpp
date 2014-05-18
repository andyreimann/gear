// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "CameraSystem.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Logger.h"

#include <G2Core/ECSManager.h>

using namespace G2;

void
CameraSystem::runPhase(std::string const& name, FrameInfo const& frameInfo) 
{
	if(name == "postUpdate") 
	{
		// calculate inverse camera rotation matrix for every camera once per frame
		auto* transformSystem = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>();
		for(auto i = 0; i < components.size(); ++i) 
		{
			auto& comp = components[i];// check if this component has a pass attached
			
			auto* cameraTransformation = transformSystem->get(comp.getEntityId());
			if(cameraTransformation != nullptr)
			{
				if(cameraTransformation->updated())
				{
					// TransformSystem calculated new matrices which frame due to some changes
					comp._setInverseCameraRotation(glm::toMat4(glm::inverse(cameraTransformation->getRotation())));
				}
			}
			else
			{
				comp._setInverseCameraRotation(glm::mat4());
			}
		}
	}
}

CameraComponent*
CameraSystem::getRenderCamera() 
{
	return get(mRenderCameraEntityId);
}

void
CameraSystem::setRenderCamera(unsigned int entityId) 
{
	mRenderCameraEntityId = entityId;
}