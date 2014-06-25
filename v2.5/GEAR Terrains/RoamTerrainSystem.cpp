#include "RoamTerrainSystem.h"
#include "RoamTerrain.h"

#include <G2Core/ECSManager.h>
#include <G2/RenderComponent.h>
#include <G2/CameraComponent.h>
#include <G2/TransformComponent.h>
#include <utility>

using namespace G2::Terrain;

RoamTerrainSystem::RoamTerrainSystem() 
{
}

void
RoamTerrainSystem::runPhase(std::string const& name, G2::FrameInfo const& frameInfo) 
{
	if(name == "postUpdate")
	{
		// camera is updated, prepare for rendering of terrain

		auto* transformSystem = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>();
		auto* cameraSystem = ECSManager::getShared().getSystem<CameraSystem,CameraComponent>();
		auto* renderSystem = ECSManager::getShared().getSystem<G2::RenderSystem,G2::RenderComponent>();
		
		CameraComponent* camera = cameraSystem->getRenderCamera();
		assert(camera != nullptr);
		
		glm::mat4 cameraSpaceMatrix;
		auto* cameraTransformation = transformSystem->get(camera->getEntityId());
		glm::mat4 inverseCameraRotation;
		glm::vec3 cameraPosition;
		if(cameraTransformation != nullptr)
		{
			cameraSpaceMatrix = cameraTransformation->getWorldSpaceMatrix();
			inverseCameraRotation = camera->getInverseCameraRotation();
			cameraPosition = cameraTransformation->getPosition();
		}

		glm::vec4 camPos(0.f,0.f,0.f,1.f);
		
		camPos = glm::inverse(cameraSpaceMatrix) * camPos;
		
		for(auto i = 0; i < components.size(); ++i) 
		{
			RoamTerrain& terrain = components[i];
			auto* renderComponent = renderSystem->get(terrain.getEntityId());
			auto* transformComponent = transformSystem->get(terrain.getEntityId());
			if(transformComponent != nullptr && transformComponent->updated())
			{
				// terrain was moved this frame
				terrain._updatePatchPositions(transformComponent->getWorldSpaceMatrix());
			}

			// recalculate and render the terrain
			_reset(&terrain, &camera->getFrustum());
			_tesselate(&terrain, glm::vec3(camPos));
			_draw(&terrain);
		}
	}
}

void
RoamTerrainSystem::_reset(RoamTerrain* terrain, G2::Frustum const* cameraFrustum) 
{
	terrain->_reset(cameraFrustum);
}

void
RoamTerrainSystem::_tesselate(RoamTerrain* terrain, glm::vec3 const& cameraPosition) 
{
	terrain->_tesselate(cameraPosition);
}

void
RoamTerrainSystem::_draw(RoamTerrain* terrain) 
{
	terrain->_draw();
}