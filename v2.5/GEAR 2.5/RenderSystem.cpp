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
#include "Effect.h"
#include "Shader.h"
#include "Logger.h"

#include <iostream>
#include <glm/ext.hpp>

using namespace G2;

#define M_PI 3.14159265358979323846

const glm::mat4 CubeMapFaceCameraRotations[6] = {
	/* G2::CUBE_MAP_POS_X */ //glm::toMat4(glm::cross(glm::cross(glm::quat(), glm::angleAxis(-90.f, glm::vec3(0.f,1.f,0.f))), glm::angleAxis(180.f, glm::vec3(0.f,0.f,1.f)))),
	/* G2::CUBE_MAP_NEG_X */ //glm::toMat4(glm::cross(glm::cross(glm::quat(), glm::angleAxis( 90.f, glm::vec3(0.f,1.f,0.f))), glm::angleAxis(180.f, glm::vec3(0.f,0.f,1.f)))),
	/* G2::CUBE_MAP_POS_Y */ //glm::toMat4(glm::cross(glm::quat(), glm::angleAxis( 90.f, glm::vec3(1.f,0.f,0.f)))),
	/* G2::CUBE_MAP_NEG_Y */ //glm::toMat4(glm::cross(glm::quat(), glm::angleAxis(-90.f, glm::vec3(1.f,0.f,0.f)))),
	/* G2::CUBE_MAP_POS_Z */ //glm::toMat4(glm::cross(glm::quat(), glm::angleAxis(180.f, glm::vec3(1.f,0.f,0.f)))),
	/* G2::CUBE_MAP_NEG_Z */ //glm::toMat4(glm::cross(glm::quat(), glm::angleAxis(180.f, glm::vec3(0.f,0.f,1.f))))
	glm::lookAt(glm::vec3(),glm::vec3() +glm::vec3(1,0,0),glm::vec3(0, -1,0)),
	glm::lookAt(glm::vec3(),glm::vec3() +glm::vec3(-1,0,0),glm::vec3(0,-1,0)),
	glm::lookAt(glm::vec3(),glm::vec3() +glm::vec3(0,-1,0),glm::vec3(0,0,-1)),
	glm::lookAt(glm::vec3(),glm::vec3() +glm::vec3(0,1,0),glm::vec3(0,0, 1)),
	glm::lookAt(glm::vec3(),glm::vec3() +glm::vec3(0,0,1),glm::vec3(0,-1,0)),
	glm::lookAt(glm::vec3(),glm::vec3() +glm::vec3(0,0,-1),glm::vec3(0,-1,0)),
};

void
RenderSystem::runPhase(std::string const& name, FrameInfo const& frameInfo)
{
	if(name == "render") 
	{
		// update camera movement 
		CameraComponent* camera = ECSManager::getShared().getSystem<CameraSystem,CameraComponent>()->getRenderCamera();
		auto* transformSystem = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>();
		bool cameraUpdated = camera->updateModelView();

		// render all passes
		for (int i = 0; i < components.size() ; ++i) 
		{
			RenderComponent& comp = components[i];// check if this component has a pass attached
			if(comp.effect.get() != nullptr && comp.effect->getPasses().size() > 0) 
			{
				for(auto it = comp.effect->getPasses().begin(); it < comp.effect->getPasses().end(); ++it)
				{
					GLDEBUG( glEnable(GL_POLYGON_OFFSET_FILL) );
					GLDEBUG( glPolygonOffset( 1.f, 100.f ) );
					// TODO get right projectionMatrix
					glm::mat4 passProjectionMatrix = camera->getProjectionMatrix();

					for(int renderIter = 0; renderIter < it->getNumRenderIterations(); ++renderIter)
					{
						it->getRenderTarget().bind(renderIter);
						// TODO get right cameraSpaceMatrix for current render iteration of pass
						glm::mat4 passCameraSpaceMatrix;
						if(it->getPov() == PointOfView::MAIN_CAMERA)
						{
							passCameraSpaceMatrix = camera->getCameraSpaceMatrix();
						}
						else
						{
							// try to get the local transformation component
							auto* transformation = transformSystem->get(comp.getEntityId());
							if(transformation != nullptr)
							{
								passCameraSpaceMatrix = transformation->getWorldSpaceMatrix();
							}
						}

						if(it->getNumRenderIterations() == 6)
						{
							GLDEBUG( glViewport(0,0,512,512));
#ifdef GLM_FORCE_RADIANS
							passProjectionMatrix = glm::perspective(90.0f * (float)M_PI / 180.f, 512.f / 512.f, 0.01f, 50.0f);
#else
							passProjectionMatrix = glm::perspective(90.0f, 512.f / 512.f, 0.01f, 50.0f);
#endif
							// cube map rendering -> adjust camera space matrix to current face
							glm::mat4 const& faceRotation = CubeMapFaceCameraRotations[renderIter];

							glm::vec4 cameraPosition = camera->getCameraSpaceMatrix() * glm::vec4(0.f,0.f,0.f,1.f);
							// passCameraSpaceMatrix.translation * faceRotation
							//passCameraSpaceMatrix = glm::inverse(passCameraSpaceMatrix * glm::inverse(camera->getCameraSpaceMatrix()) * glm::translate(passCameraPosition.x, passCameraPosition.y, passCameraPosition.z) * faceRotation);
							passCameraSpaceMatrix = glm::inverse(
								glm::inverse(camera->mRotation) * // cam translation
							//	//passCameraSpaceMatrix * // lightTrans
							//	//glm::inverse(camera->getCameraSpaceMatrix()) * // inv cam
							//	//passCameraSpaceMatrix * 
							//	//glm::inverse(camera->getCameraSpaceMatrix()) * 
								faceRotation
							);
							passCameraSpaceMatrix = glm::inverse(
								glm::inverse(camera->getCameraSpaceMatrix()) * // cam translation
								glm::translate(cameraPosition.x, cameraPosition.y, cameraPosition.z) *
								//passCameraSpaceMatrix * // lightTrans
								//glm::inverse(camera->getCameraSpaceMatrix()) * // inv cam
								//passCameraSpaceMatrix * 
								//glm::inverse(camera->getCameraSpaceMatrix()) * 
								faceRotation
							);
							;
						}
						//if(renderIter == 1)
						//logger << "[Pass#" << renderIter << "] Cam-Space-Mat:\n" << passCameraSpaceMatrix << endl;

						for (int k = 0; k < components.size() ; ++k) 
						{
							RenderComponent& innerComp = components[k];
			
							if(innerComp.vaos.size() == 0 || i == k)
							{
								// no VAO attached or pass component is this component
								continue;
							}
							std::shared_ptr<Shader> passShader = getPassRenderShader(&innerComp, &(*it));
							// bind shader before cann render()
							passShader->bind();
							// pass rendering
							render(passProjectionMatrix, passCameraSpaceMatrix, &innerComp, passShader);
						}
						//logger << "Rendered " << components.size() << " components to rendertarget" << endl;
						it->getRenderTarget().unbind();
					}
					// bind result of pass
					it->getRenderTarget().getRenderTexture()->bind(TEX_SLOT1+(int)it->getRenderTarget().getRenderTextureSampler());
					GLDEBUG( glDisable(GL_POLYGON_OFFSET_FILL) );
				}
			}
		}

		// do normal rendering
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

			// bind shader before cann render()
			shader->bind();
			// regular rendering
			GLDEBUG( glViewport(0,0,camera->getViewportWidth(),camera->getViewportHeight()));
			render(camera->getProjectionMatrix(), camera->getCameraSpaceMatrix(), &comp, shader);
		}
	}
}

void
RenderSystem::render(glm::mat4 const& projectionMatrix, glm::mat4 const& cameraSpaceMatrix, RenderComponent* component, std::shared_ptr<Shader>& boundShader)
{
	// Upload Matrices
	auto* transformation = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>()->get(component->getEntityId());
	uploadMatrices(boundShader, transformation, projectionMatrix, cameraSpaceMatrix);

	// Upload Lights
	LightSystem* lightSystem = ECSManager::getShared().getSystem<LightSystem,LightComponent>();
	int maxLights = std::min(8,(int)lightSystem->components.size());
	int numActive = 0;
	for(int l = 0; l < maxLights; ++l)
	{
		auto& lightComponent = lightSystem->components[l];
		if(!lightComponent.isEnabled())
		{
			continue;
		}
		uploadLight(boundShader, &lightComponent, cameraSpaceMatrix, numActive++);
	}
	boundShader->setProperty(Property("G2ActiveLights"), numActive);

	// Bind Textures
	auto const& textures = component->material.getTextures();
	for(auto it = textures.begin(); it != textures.end(); ++it)
	{
		if(it->second.get() != nullptr)
		{
			// tex slots are continuous numbers and samplers start at 0 -> simple calculation
			it->second->bind(TEX_SLOT1+(int)it->first);
		}
	}

	// Upload Material
	uploadMaterial(boundShader, &component->material);

	// Draw all attached VAO
	for (int k = 0; k < component->vaos.size() ; ++k) 
	{
		component->vaos[k].draw(component->drawMode);
	}
}



void
RenderSystem::uploadMatrices(std::shared_ptr<Shader>& shader, TransformComponent* transformation, glm::mat4 const& projectionMatrix, glm::mat4 const& cameraSpaceMatrix) 
{

	// TEMP UNTIL PROJECTION MATRIX UPDATES ARE TRACKED IN CAMERA!
	shader->setProperty(Property("matrices.projection_matrix"), projectionMatrix);
	// TEMP END
	if(transformation != nullptr) 
	{
		glm::mat4 mv = cameraSpaceMatrix * transformation->getWorldSpaceMatrix();
		shader->setProperty(Property("matrices.model_matrix"), transformation->getWorldSpaceMatrix());
		shader->setProperty(Property("matrices.view_matrix"), cameraSpaceMatrix);
		shader->setProperty(Property("matrices.modelview_matrix"), mv);
		if(transformation->getScale() == glm::vec3(1.f,1.f,1.f))
		{
			shader->setProperty(Property("matrices.normal_matrix"), glm::mat3(mv));
		}
		else
		{
			// non-uniform scaling
			shader->setProperty(Property("matrices.normal_matrix"), glm::inverseTranspose(glm::mat3(mv)));
		}
	}
	else 
	{
		shader->setProperty(Property("matrices.model_matrix"), glm::mat4());
		shader->setProperty(Property("matrices.view_matrix"), cameraSpaceMatrix);
		shader->setProperty(Property("matrices.modelview_matrix"), cameraSpaceMatrix);
		shader->setProperty(Property("matrices.normal_matrix"), glm::mat3(cameraSpaceMatrix));
	}
}

void
RenderSystem::uploadLight(std::shared_ptr<Shader>& shader, LightComponent* light, glm::mat4 const& cameraSpaceMatrix, int index) 
{
	glm::vec4 lightPos;
	glm::vec3 lightDir;
	if(light->getType() != LightType::DIRECTIONAL)
	{
		lightPos = cameraSpaceMatrix * light->getTransformedPosition();
	}
	if(light->getType() != LightType::POSITIONAL)
	{
		lightDir = glm::normalize(glm::mat3(cameraSpaceMatrix) * light->getTransformedDirection());
	}

	std::stringstream baseStr;
	baseStr << "light[" << index << "]";
	std::string base = baseStr.str();
	// set coloring
	shader->setProperty(Property(base + ".ambient"), light->ambient);
	shader->setProperty(Property(base + ".diffuse"), light->diffuse);
	shader->setProperty(Property(base + ".specular"), light->specular);
	// set positional
	shader->setProperty(Property(base + ".position"), lightPos);
	shader->setProperty(Property(base + ".direction"), lightDir);
	shader->setProperty(Property(base + ".range"), 0.f); // only needed for spotlight/point light
	shader->setProperty(Property(base + ".attenuation"), glm::vec4(light->attenuation,light->linearAttenuation,light->exponentialAttenuation,0.f)); // only needed for spotlight/point light
	// set special
	shader->setProperty(Property(base + ".cosCutoff"), std::cosf(light->cutOffDegrees * (float)M_PI / 180.f)); // only needed for spotlight
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
	else if(component->effect.get() != nullptr && component->effect->hasCompiledShaders()) 
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

std::shared_ptr<Shader>
RenderSystem::getPassRenderShader(RenderComponent* component, Pass const* pass) const
{
	// TODO add caching for pass shader as well
	return pass->getShader(component->material,component->vaos[0]);;
}