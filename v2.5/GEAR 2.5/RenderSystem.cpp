// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "RenderSystem.h"
#include "RenderComponent.h"
#include "CameraSystem.h"
#include "CameraComponent.h"
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
			if(comp.shaderCache.isCacheValid(comp.material.getVersion(), comp.vaos[0].getVersion()))
			{
				shader = comp.shaderCache.getShader();
			}
			else if(comp.effect.get() != NULL && comp.effect->hasCompiledShaders()) 
			{
				shader = comp.effect->getShader(comp.material,comp.vaos[0]);
				// update shader cache
				comp.shaderCache.setShader(shader, comp.material.getVersion(), comp.vaos[0].getVersion());
			}
			else
			{
				shader = defaultEffect->getShader(comp.material,comp.vaos[0]);
				// update shader cache
				comp.shaderCache.setShader(shader, comp.material.getVersion(), comp.vaos[0].getVersion());
			}
			// bind shader, since also in case of a default shader, this could be a permuted shader!
			shader->bind();
			if(cameraUpdated) 
			{
				/** TODO: This normal matrix isn't the one, we need for normal transformation
				 * Normal matrix calculation: (modelview^-1)^T, actually: (view^-1)^T
				 */
				shader->setProperty(Property("matrices.normal_matrix"), camera->getNormalMatrix());
			}

			// TEMP UNTIL PROJECTION MATRIX UPDATES ARE TRACKED IN CAMERA!
			shader->setProperty(Property("matrices.projection_matrix"), camera->getProjectionMatrix());
			shader->setProperty(Property("light.dir"), glm::normalize(glm::vec3(1.f, 1.f, 1.f)));
			// TEMP END

			// upload transformation
			auto* transformation = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>()->get(comp.getEntityId());
			if(transformation != nullptr) 
			{
				shader->setProperty(Property("matrices.modelview_matrix"), camera->getCameraSpaceMatrix() * transformation->getWorldSpaceMatrix());
			}
			else 
			{
				shader->setProperty(Property("matrices.modelview_matrix"), camera->getCameraSpaceMatrix());
			}

			//DEBUG

			auto const& textures = comp.material.getTextures();

			for(auto it = textures.begin(); it != textures.end(); ++it)
			{
				if(it->second.get() != nullptr)
				{
					// tex slots are continuous numbers and samplers start at 0 -> simple calculation
					it->second->bind(TEX_SLOT1+(int)it->first);
				}
			}

			// upload material
			shader->setProperty(Property("material.ambient"), comp.material.getAmbient());
			shader->setProperty(Property("material.diffuse"), comp.material.getDiffuse());

			shader->setProperty(Property("material.specular"), comp.material.getSpecular());
			shader->setProperty(Property("G2Material.specular"), comp.material.getSpecular());
			shader->setProperty(Property("specular"), comp.material.getSpecular());

			shader->setProperty(Property("material.shininess"), comp.material.getShininess());
			// draw all attached VAO
			for (int k = 0; k < comp.vaos.size() ; ++k) 
			{
				comp.vaos[k].draw(comp.drawMode);
			}
		}
	}
}