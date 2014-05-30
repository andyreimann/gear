// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "RenderSystem.h"
#include "RenderComponent.h"
#include "CameraSystem.h"
#include "CameraComponent.h"
#include "LightSystem.h"
#include "LightComponent.h"
#include "TransformComponent.h"
#include "Shader.h"
#include "Texture.h"
#include "Effect.h"
#include "Logger.h"
#include "Frustum.h"

#include <G2Core/ECSManager.h>

#include <glm/ext.hpp>

using namespace G2;

#define M_PI 3.14159265358979323846

const glm::mat4 CubeMapFaceCameraRotations[6] = {
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
		// get pointer to all needed systems once per frame -> less dynamic casts!
		auto* transformSystem = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>();
		auto* cameraSystem = ECSManager::getShared().getSystem<CameraSystem,CameraComponent>();
		auto* lightSystem = ECSManager::getShared().getSystem<LightSystem,LightComponent>();

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
		
		//logger << "[RenderSystem] Pass rendering in frame " << frameInfo.frame << endl;
		renderPasses(
			camera->getProjectionMatrix(),
			cameraPosition,
			cameraSpaceMatrix, 
			inverseCameraRotation, 
			transformSystem,
			lightSystem
		);

		// do normal rendering
		//logger << "[RenderSystem] Normal rendering in frame " << frameInfo.frame << endl;
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
			// bind shader before call render()
			shader->bind();
			// regular rendering
			GLDEBUG( glViewport(0,0,camera->getViewportWidth(),camera->getViewportHeight()));
			render(camera->getProjectionMatrix(), cameraSpaceMatrix, inverseCameraRotation, &comp, shader, transformSystem, lightSystem, &camera->getFrustum());
		}
	}
}

void
RenderSystem::renderPasses(
					glm::mat4 const& cameraProjectionMatrix, 
					glm::vec3 const& cameraPosition,
					glm::mat4 const& cameraSpaceMatrix, 
					glm::mat4 const& inverseCameraRotation, 
					TransformSystem* transformSystem,
					LightSystem* lightSystem) 
{
	// render all passes -> components with passes should be cached in system!
	for (int i = 0; i < components.size() ; ++i) 
	{
		auto& comp = components[i];// check if this component has a pass attached

		// lazy init aabb if needed
		initializeAABB(&comp, transformSystem);

		if(comp.getEffect().get() != nullptr && comp.getEffect()->getPasses().size() > 0) 
		{
			for(auto it = comp.getEffect()->getPasses().begin(); it < comp.getEffect()->getPasses().end(); ++it)
			{
				it->preRender();
				glm::mat4 passProjectionMatrix = cameraProjectionMatrix;
					
				if(it->getRenderTarget().getRenderTargetType() == RenderTargetType::RT_CUBE)
				{
#ifdef GLM_FORCE_RADIANS
					passProjectionMatrix = glm::perspective(it->getFovY() * (float)M_PI / 180.f, it->getRenderTarget().getRenderTexture()->getWidth() / it->getRenderTarget().getRenderTexture()->getHeight(), it->getZNear(), it->getZFar());
#else
					passProjectionMatrix = glm::perspective(it->getFovY(), it->getRenderTarget().getRenderTexture()->getWidth() / (float)it->getRenderTarget().getRenderTexture()->getHeight(), it->getZNear(), it->getZFar());
#endif
				}

				for(int renderIter = 0; renderIter < it->getNumRenderIterations(); ++renderIter)
				{
					it->getRenderTarget().bind(renderIter);
					glm::mat4 passCameraSpaceMatrix;
					if(it->getPov() == PointOfView::MAIN_CAMERA)
					{
						if(it->getRenderTarget().getRenderTargetType() == RenderTargetType::RT_CUBE)
						{ // only use cameras position
							passCameraSpaceMatrix = glm::translate(glm::vec3(cameraSpaceMatrix * glm::vec4(0.f,0.f,0.f,1.f)));
						}
						else
						{
							passCameraSpaceMatrix = cameraSpaceMatrix;
						}
					}
					if(it->getPov() == PointOfView::MAIN_CAMERA_FLIP_Y)
					{
						glm::vec3 camPos = -cameraPosition;
						glm::vec3 viewVec = glm::vec3(glm::normalize(cameraSpaceMatrix * glm::vec4(0.f,0.f,-1.f,0.f)));
						glm::vec3 upVec = glm::vec3(glm::normalize(cameraSpaceMatrix * glm::vec4(0.f,1.f,0.f,0.f)));
						upVec.x = -upVec.x;
						upVec.z = -upVec.z;
						viewVec.y = -viewVec.y; //
						camPos.y = camPos.y - (2.f * (camPos.y - (-it->getFlipYLevel())));
						//logger << "POS " << camPos << " DIR " << viewVec << endl;
						passCameraSpaceMatrix = glm::inverse(glm::lookAt(glm::vec3(camPos),glm::vec3(camPos)+viewVec,upVec));
						//passCameraSpaceMatrix = camera->getCameraSpaceMatrix();
					}
					else
					{
						// try to get the local transformation component
						auto* transformation = transformSystem->get(comp.getEntityId());
						if(transformation != nullptr)
						{
							passCameraSpaceMatrix = glm::translate(glm::vec3(transformation->getWorldSpaceMatrix() * glm::vec4(0.f,0.f,0.f,1.f)));
						}
					}
					if(it->getRenderTarget().getRenderTargetType() == RenderTargetType::RT_CUBE)
					{
						passCameraSpaceMatrix = glm::inverse( // adjust cam space mat to current cube map face
							passCameraSpaceMatrix * // lightTrans
							inverseCameraRotation * // inverse render camera rotation
							CubeMapFaceCameraRotations[renderIter] // face rotation
						);
					}
					for (int k = 0; k < components.size() ; ++k) 
					{
						auto& innerComp = components[k];
						
						// lazy init aabb if needed
						initializeAABB(&innerComp, transformSystem);

						if(innerComp.vaos.size() == 0 || i == k)
						{
							// no VAO attached or pass component is this component
							continue;
						}
						auto passShader = getPassRenderShader(&innerComp, &(*it));
						// bind shader before call render()
						passShader->bind();
						Frustum frustum;
						frustum.setup(passProjectionMatrix * passCameraSpaceMatrix);
						// pass rendering
						render(passProjectionMatrix, passCameraSpaceMatrix, inverseCameraRotation, &innerComp, passShader, transformSystem, lightSystem, &frustum);
					}
					it->getRenderTarget().unbind();
				}
				// bind result of pass
				it->getRenderTarget().getRenderTexture()->bind(TEX_SLOT1+(int)it->getRenderTarget().getRenderTextureSampler());
				it->postRender();
			}
		}
	}
}

void
RenderSystem::render(glm::mat4 const& projectionMatrix, glm::mat4 const& cameraSpaceMatrix, glm::mat4 const& inverseCameraRotation, RenderComponent* component, std::shared_ptr<Shader>& boundShader, TransformSystem* transformSystem, LightSystem* lightSystem, Frustum const* frustum)
{

	// Upload Matrices
	auto* transformation = transformSystem->get(component->getEntityId());
	uploadMatrices(boundShader, transformation, projectionMatrix, cameraSpaceMatrix, inverseCameraRotation, component->billboarding);

	// Upload Lights
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
		if(frustum->inside(component->worldSpaceAABBs[k]))
		{
			component->vaos[k].draw(component->drawMode);
		}
		//else
		//{
		//	logger << "[RenderSystem] Culled vao " << k << " in RenderComponent " << component->getEntityId() << endl;
		//}
	}
}

void
RenderSystem::uploadMatrices(std::shared_ptr<Shader>& shader, TransformComponent* transformation, glm::mat4 const& projectionMatrix, glm::mat4 const& cameraSpaceMatrix, glm::mat4 const& inverseCameraRotation, bool billboarding) 
{

	// TEMP UNTIL PROJECTION MATRIX UPDATES ARE TRACKED IN CAMERA!
	shader->setProperty(Property("matrices.projection_matrix"), projectionMatrix);
	// TEMP END
	if(transformation != nullptr) 
	{
		glm::mat4 modelMatrix;
		glm::mat4 modelViewMatrix;
		if(!billboarding)
		{
			modelMatrix = transformation->getWorldSpaceMatrix();
			modelViewMatrix = cameraSpaceMatrix * modelMatrix;
		}
		else
		{
			modelMatrix = transformation->getWorldSpaceMatrix() * inverseCameraRotation;
			modelViewMatrix = cameraSpaceMatrix * modelMatrix;
		}
		shader->setProperty(Property("matrices.model_matrix"), modelMatrix);
		shader->setProperty(Property("matrices.view_matrix"), cameraSpaceMatrix);
		shader->setProperty(Property("matrices.modelview_matrix"), modelViewMatrix);
		if(transformation->getScale() == glm::vec3(1.f,1.f,1.f))
		{
			shader->setProperty(Property("matrices.normal_matrix"), glm::mat3(modelViewMatrix));
		}
		else
		{
			// non-uniform scaling
			shader->setProperty(Property("matrices.normal_matrix"), glm::inverseTranspose(glm::mat3(modelViewMatrix)));
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
	if(component->_getShaderCache().isCacheValid(component->material.getVersion(), component->vaos[0].getVersion()))
	{
		shader = component->_getShaderCache().getShader();
	}
	else if(component->mEffect.get() != nullptr && component->mEffect->hasCompiledShaders()) 
	{
		shader = component->getEffect()->getShader(component->material,component->vaos[0]);
		// update shader cache
		component->_getShaderCache().setShader(shader, component->material.getVersion(), component->vaos[0].getVersion());
	}
	else
	{
		shader = defaultEffect->getShader(component->material,component->vaos[0]);
		// update shader cache
		component->_getShaderCache().setShader(shader, component->material.getVersion(), component->vaos[0].getVersion());
	}
	return shader;
}

std::shared_ptr<Shader>
RenderSystem::getPassRenderShader(RenderComponent* component, Pass const* pass) const
{
	// TODO add caching for pass shader as well
	return pass->getShader(component->material,component->vaos[0]);;
}

void
RenderSystem::initializeAABB(RenderComponent* component, TransformSystem* transformSystem) 
{
	if(component->objectSpaceAABBs.size() != component->vaos.size())
	{
		component->objectSpaceAABBs.resize(component->vaos.size());
		component->worldSpaceAABBs.resize(component->vaos.size());
		//logger << "[RenderSystem] Lazy initialize object space AABBs for component " << component->getEntityId() << endl;
		for(int i = 0; i < component->vaos.size(); ++i)
		{
			VertexArrayObject& vao = component->vaos[i];
			AABB& aabb = component->objectSpaceAABBs[i];
			unsigned int componentsPerPosition = vao.getNumBytesBySemantic(Semantics::POSITION) / sizeof(float);
			if(componentsPerPosition == 3)
			{
				glm::vec3* vertices = (glm::vec3*)vao.getDataPointer(Semantics::POSITION, BufferAccessMode::READ_ONLY);
				for(unsigned int v = 0; v < vao.getNumElements(); ++v)
				{
					aabb.merge(vertices[v]);
				}
				vao.returnDataPointer(Semantics::POSITION);
			}
			else if(componentsPerPosition == 4)
			{
				glm::vec4* vertices = (glm::vec4*)vao.getDataPointer(Semantics::POSITION, BufferAccessMode::READ_ONLY);
				for(unsigned int v = 0; v < vao.getNumElements(); ++v)
				{
					aabb.merge(glm::vec3(vertices[v]));
				}
				vao.returnDataPointer(Semantics::POSITION);
			}
			transformSystem->lock();
			auto* transformComponent = transformSystem->get(component->getEntityId());
			
			if(transformComponent != nullptr)
			{
				component->worldSpaceAABBs[i] = std::move(aabb.transform(transformComponent->getWorldSpaceMatrix()));
			}
			else
			{
				component->worldSpaceAABBs[i] = aabb;
			}
			transformSystem->unlock();
		}

	}
}