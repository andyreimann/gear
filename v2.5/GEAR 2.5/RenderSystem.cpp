// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "RenderSystem.h"
#include "RenderComponent.h"
#include "CameraSystem.h"
#include "CameraComponent.h"
#include "LightSystem.h"
#include "LightComponent.h"
#include "TransformSystem.h"
#include "TransformComponent.h"
#include "NameComponent.h"
#include "NameSystem.h"
#include "ECSManager.h"
#include "Shader.h"
#include "Texture.h"
#include "Logger.h"
#include "Effect.h"

#include <iostream>

using namespace G2;

void
RenderSystem::runPhase(std::string const& name, FrameInfo const& frameInfo)
{
	if(name == "render") 
	{

		// update camera movement 
		CameraComponent* camera = ECSManager::getShared().getSystem<CameraSystem,CameraComponent>()->getRenderCamera();
		
		bool cameraUpdated = camera->updateModelView();

		for (int i = 0; i < components.size() ; ++i) 
		{
			RenderComponent& comp = components[i];
			
			if(comp.vaos.size() == 0)
			{
				// no VAO attached
				continue;
			}
			
			// calc which shader to use for rendering
			std::shared_ptr<Shader> shader = getRenderShader(&comp);

			// bind shader, since also in case of a default shader, this could be a permuted shader!
			shader->bind();

			// Upload Matrices
			auto* transformation = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>()->get(comp.getEntityId());
			uploadMatrices(shader, transformation, camera);

			// Upload Lights
			int maxLights = 1;
			LightSystem* lightSystem = ECSManager::getShared().getSystem<LightSystem,LightComponent>();
			for(int l = 0; l < maxLights; ++l)
			{
				auto& lightComponent = lightSystem->components[l];
				uploadLight(shader, &lightComponent, camera);
			}

			// Bind Textures
			auto const& textures = comp.material.getTextures();
			for(auto it = textures.begin(); it != textures.end(); ++it)
			{
				if(it->second.get() != nullptr)
				{
					// tex slots are continuous numbers and samplers start at 0 -> simple calculation
					it->second->bind(TEX_SLOT1+(int)it->first);
				}
			}

			// Upload Material
			uploadMaterial(shader, &comp.material);

			// Draw all attached VAO
			for (int k = 0; k < comp.vaos.size() ; ++k) 
			{
				comp.vaos[k].draw(comp.drawMode);
			}
		}
	}
}

void
RenderSystem::uploadMatrices(std::shared_ptr<Shader>& shader, TransformComponent* transformation, CameraComponent* camera) 
{

	// TEMP UNTIL PROJECTION MATRIX UPDATES ARE TRACKED IN CAMERA!
	shader->setProperty(Property("matrices.projection_matrix"), camera->getProjectionMatrix());
	// TEMP END
	if(transformation != nullptr) 
	{
		glm::mat4 mv = camera->getCameraSpaceMatrix() * transformation->getWorldSpaceMatrix();
		shader->setProperty(Property("matrices.model_matrix"), transformation->getWorldSpaceMatrix());
		shader->setProperty(Property("matrices.view_matrix"), camera->getCameraSpaceMatrix());
		shader->setProperty(Property("matrices.modelview_matrix"), mv);
		// this can be used as normal matrix if mv has a uniform scaling! Up to now we don't care!
		shader->setProperty(Property("matrices.normal_matrix"), glm::mat3(mv));
	}
	else 
	{
		shader->setProperty(Property("matrices.model_matrix"), transformation->getWorldSpaceMatrix());
		shader->setProperty(Property("matrices.view_matrix"), camera->getCameraSpaceMatrix());
		shader->setProperty(Property("matrices.modelview_matrix"), camera->getCameraSpaceMatrix());
		shader->setProperty(Property("matrices.normal_matrix"), glm::mat3(camera->getCameraSpaceMatrix()));
	}
}

void
RenderSystem::uploadLight(std::shared_ptr<Shader>& shader, LightComponent* light, CameraComponent* camera) 
{
	glm::vec4 pos = camera->getCameraSpaceMatrix() * light->getTransformedPosition();
	glm::vec3 shaderDir = glm::normalize(glm::vec3(pos.x,pos.y,pos.z));
	shader->setProperty(Property("light.color"), light->diffuse);
	shader->setProperty(Property("light.position"), pos);
	shader->setProperty(Property("light.direction"), shaderDir);
	shader->setProperty(Property("light.cosCutoff"), std::cosf(light->cutOffDegrees * 3.14f / 180.f)); // only needed for spotlight
	shader->setProperty(Property("light.range"), 0.f); // only needed for spotlight/point light
	shader->setProperty(Property("light.attenuation"), glm::vec4(light->attenuation,light->linearAttenuation,light->exponentialAttenuation,0.f)); // only needed for spotlight/point light
}

void
RenderSystem::uploadMaterial(std::shared_ptr<Shader>& shader, Material* material) 
{
	shader->setProperty(Property("material.ambient"), material->getAmbient());
	shader->setProperty(Property("material.diffuse"), material->getDiffuse());
	shader->setProperty(Property("material.specular"), material->getSpecular());
	shader->setProperty(Property("material.shininess"), material->getShininess());
}

std::shared_ptr<Shader>
RenderSystem::getRenderShader(RenderComponent* component) 
{
	std::shared_ptr<Shader> shader;

	/************************************************************
	 * Shader Caching											*
	 *----------------------------------------------------------*
	 * Shader decisions are cached for RenderComponents.		*
	 * The caching builds on top of a VersionTracking on the	*
	 * vao and the material. Whenever the version check			*
	 * with the versions of the material and the vao fails,		*
	 * a new Shader will be calculated and cached.				*
	 * When changing something on the UberShader assigned to	*
	 * the RenderComponent, the cache is also invalidated!		*
	 ************************************************************/
	if(component->shaderCache.isCacheValid(component->material.getVersion(), component->vaos[0].getVersion()))
	{
		shader = component->shaderCache.getShader();
	}
	else if(component->effect.get() != NULL && component->effect->hasCompiledShaders()) 
	{
		shader = component->effect->getShader(component->material,component->vaos[0]);
		// update shader cache
		component->shaderCache.setShader(shader, component->material.getVersion(), component->vaos[0].getVersion());
	}
	else
	{
		shader = defaultEffect->getShader(component->material,component->vaos[0]);
		// update shader cache
		component->shaderCache.setShader(shader, component->material.getVersion(), component->vaos[0].getVersion());
	}
	return shader;
}