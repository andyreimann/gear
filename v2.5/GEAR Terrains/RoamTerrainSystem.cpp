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
		
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for(auto i = 0; i < components.size(); ++i) 
		{
			RoamTerrain& terrain = components[i];
			if(terrain.mNumRecalculations == 0 || frameInfo.frame%1 == 0)
			{
				auto* renderComponent = renderSystem->get(terrain.getEntityId());

				glm::vec4 cameraDir = camera->getInverseCameraRotation() * glm::vec4(0.f,0.f,-1.f,0.f);
				cameraDir.y = 0.f;
				cameraDir = glm::normalize(cameraDir);

				_reset(&terrain, glm::vec3(camPos), camera->getFovY(), std::acos(glm::dot(cameraDir, glm::vec4(0.f,0.f,-1.f,0.f) / (glm::length(cameraDir) * glm::length(glm::vec4(0.f,0.f,-1.f,0.f))))));
				_tesselate(&terrain, glm::vec3(camPos));
				_draw(&terrain);

			}
		}
	}
}

void
RoamTerrainSystem::_reset(RoamTerrain* terrain, glm::vec3 const& cameraPosition, float fovY, float cameraYawAngle) 
{
	terrain->_reset(cameraPosition, fovY*2.f, cameraYawAngle * 180.f / (float)M_PI);
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

void
RoamTerrainSystem::_uploadMatrices(std::shared_ptr<G2::Shader> shader, glm::mat4 const& cameraSpaceMatrix, glm::mat4 const& modelMatrix) 
{
	shader->setProperty(std::string("matrices.model_matrix"), modelMatrix);
	shader->setProperty(std::string("matrices.view_matrix"), cameraSpaceMatrix);
	shader->setProperty(std::string("matrices.modelview_matrix"), cameraSpaceMatrix * modelMatrix);
	shader->setProperty(std::string("matrices.normal_matrix"), glm::mat3(cameraSpaceMatrix));
}