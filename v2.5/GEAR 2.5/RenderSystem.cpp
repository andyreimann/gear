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
#include "Texture2D.h"
#include "Effect.h"
#include "Logger.h"
#include "Frustum.h"
#include "LightEffectState.h"
#include "MultipleRenderTarget.h"

#include <G2Core/ECSManager.h>
#include <G2Core/EventDistributer.h>

#include <glm/ext.hpp>
#include <algorithm>

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

RenderSystem::RenderSystem() :
	mPostProcessingRenderTargets(2),
	mClearColor(0.25f, 0.25f, 0.25f, 1.f)
{
	mRenderType = RenderType::FORWARD_RENDERING;

	EventDistributer::onViewportResize.hook(this, &RenderSystem::_onViewportResize);

	// setup full screen quad in NDC
	mFullScreenQuad.resizeElementCount(4);
	glm::vec3 geometry[4];
	geometry[0] = glm::vec3(-1.f,-1.f,0.f);
	geometry[1] = glm::vec3( 1.f,-1.f,0.f);
	geometry[2] = glm::vec3( 1.f, 1.f,0.f);
	geometry[3] = glm::vec3(-1.f, 1.f,0.f);
	mFullScreenQuad.writeData(G2::Semantics::POSITION, geometry);
	glm::vec2 texCoord[4];
	texCoord[0] = glm::vec2(0.f,0.f);
	texCoord[1] = glm::vec2(1.f,0.f);
	texCoord[2] = glm::vec2(1.f,1.f);
	texCoord[3] = glm::vec2(0.f,1.f);
	mFullScreenQuad.writeData(G2::Semantics::TEXCOORD_0, texCoord);
}

RenderSystem::~RenderSystem() 
{
	components.clear();
	EventDistributer::onViewportResize.unHook(this, &RenderSystem::_onViewportResize);
}

void
RenderSystem::runPhase(std::string const& name, FrameInfo const& frameInfo)
{
	if(name == "postUpdate") 
	{
		// calculate only those AABBs, which are scheduled for recalculation
		// TODO Animated meshes will not schedule a recalculation so far! Should be configurable!
		auto* transformSystem = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>();
		for (auto it = mRecalcAABBEntityIds.begin(); it != mRecalcAABBEntityIds.end() ; ++it) 
		{
			RenderComponent* comp = get(*it);
			if(comp != nullptr)
			{
				// recalculate all model space AABBs
				// it also synchronizes 
				this->_recalculateModelSpaceAABB(comp, transformSystem);

				for (unsigned int k = 0; k < comp->getNumDrawCalls() ; ++k) 
				{
					// TODO Just merging it will may lead to a much bigger AABB for the world as needed over time
					mWorldAABB.merge(comp->getDrawCall(k).getWorldSpaceAABB());
				}
			}
		}
		mRecalcAABBEntityIds.clear();
	}
	else if(name == "render") 
	{
		//G2::logger << "R in " << frameInfo.frame << G2::endl;
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
		_renderPasses(
			camera,
			cameraPosition,
			cameraSpaceMatrix, 
			inverseCameraRotation, 
			transformSystem,
			lightSystem
		);

		if(mRenderType == RenderType::FORWARD_RENDERING)
		{
			_renderForward(camera, cameraSpaceMatrix, inverseCameraRotation, transformSystem, lightSystem);
		}
		else if(mRenderType == RenderType::DEFERRED_SHADING)
		{
			_renderDeferred(camera, cameraSpaceMatrix, inverseCameraRotation, transformSystem, lightSystem);
		}
	}
}

void
RenderSystem::_renderForward(
				CameraComponent* mainCamera,
				glm::mat4 const& cameraSpaceMatrix,
				glm::mat4 const& inverseCameraRotation,
				TransformSystem* transformSystem,
				LightSystem* lightSystem
)
{
	// do normal rendering
	if(mPostProcessingEffects.size() > 0)
	{
		// capture frame in first render target
		mCurrentPostProcessingRenderTargetIndex = 0;
		mPostProcessingRenderTargets[mCurrentPostProcessingRenderTargetIndex]->bind(0);
	}
	
	GLDEBUG( glViewport(0,0,mainCamera->getViewportWidth(),mainCamera->getViewportHeight()));
	
	// TODO cache inverse camera space matrix!
	glm::vec4 cameraPosition = glm::inverse(cameraSpaceMatrix) * glm::vec4(0.f,0.f,0.f,1.f);

	_renderAllComponents(&mainCamera->getFrustum(), mainCamera->getProjectionMatrix(), cameraSpaceMatrix, inverseCameraRotation, cameraPosition, transformSystem, lightSystem);
					
	if(mPostProcessingEffects.size() > 0)
	{
		// capture frame in first render target
		mPostProcessingRenderTargets[mCurrentPostProcessingRenderTargetIndex]->unbind();
	}
	for(int i = 0; i < mPostProcessingEffects.size(); ++i)
	{
		mPostProcessingEffects[i]->getShader()->bind();

		_uploadMatrices(mPostProcessingEffects[i]->getShader(), nullptr, glm::mat4(), glm::mat4(), glm::mat4(), false);

		mCurrentPostProcessingRenderTargetIndex = 1 - mCurrentPostProcessingRenderTargetIndex;
		if(i < mPostProcessingEffects.size() - 1)
		{
			// bind next ping pong render target for capturing
			mPostProcessingRenderTargets[mCurrentPostProcessingRenderTargetIndex]->bind(0);
		}
		// bind last scene rendering texture
		mPostProcessingRenderTargets[1-mCurrentPostProcessingRenderTargetIndex]->getRenderTexture()->bind(TEX_SLOT1+(int)mPostProcessingRenderTargets[1-mCurrentPostProcessingRenderTargetIndex]->getRenderTextureSampler());
		// draw post processing
		mFullScreenQuad.bind();
		mFullScreenQuad.draw(GL_QUADS, 0);
		mFullScreenQuad.unbind();
		// unbind render target
		mPostProcessingRenderTargets[mCurrentPostProcessingRenderTargetIndex]->unbind();
	}
}

void
RenderSystem::_renderDeferred(
				CameraComponent* mainCamera,
				glm::mat4 const& cameraSpaceMatrix,
				glm::mat4 const& inverseCameraRotation,
				TransformSystem* transformSystem,
				LightSystem* lightSystem
)
{
	// render to G-Buffer
	mDeferredShadingPass = DeferredShadingPass::ATTRIBUTES_PASS;
	mDeferredShadingTarget->bind();
	//mGBufferEffect->bind();
	GLDEBUG( glViewport(0,0,mainCamera->getViewportWidth(),mainCamera->getViewportHeight()));
	
	// TODO cache inverse camera space matrix!
	glm::vec4 cameraPosition = glm::inverse(cameraSpaceMatrix) * glm::vec4(0.f,0.f,0.f,1.f);

	_renderAllComponents(&mainCamera->getFrustum(), mainCamera->getProjectionMatrix(), cameraSpaceMatrix, inverseCameraRotation, cameraPosition, transformSystem, lightSystem);
			
	mDeferredShadingTarget->unbind();

	mDeferredShadingPass = DeferredShadingPass::SHADING_PASS;
	// TODO render to full screen quad -> possible user defined shading shader
	// Need to find solution of how to get a Shader from an Effect without a material
			
	mDeferredShadingTarget->getRenderTexture(BufferAttachment::COLOR_0)->bind(TEX_SLOT1);	// diffuse
	mDeferredShadingTarget->getRenderTexture(BufferAttachment::COLOR_1)->bind(TEX_SLOT1+1);	// normals
	mDeferredShadingTarget->getRenderTexture(BufferAttachment::COLOR_2)->bind(TEX_SLOT1+2);	// positions
			
	// combine everything back using one surface shader
	// here the POST PROCESSING can take place:  Glow, Distortion, Edge-Smoothing, Fog, ...
	//mShadingEffect->bind();
	mFullScreenQuad.draw(GL_QUADS, 0);
}

void
RenderSystem::_renderPasses(
					CameraComponent* mainCamera,
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

		if(comp.getEffect().get() != nullptr && comp.getEffect()->getPasses().size() > 0) 
		{
			for(auto it = comp.getEffect()->getPasses().begin(); it < comp.getEffect()->getPasses().end(); ++it)
			{
				it->preRender();
				glm::mat4 passProjectionMatrix = mainCamera->getProjectionMatrix();
					
				if(it->getRenderTarget().getRenderTargetType() == RenderTargetType::RT_CUBE)
				{
#ifdef GLM_FORCE_RADIANS
					passProjectionMatrix = glm::perspective(it->getFovY() * (float)M_PI / 180.f, it->getRenderTarget().getRenderTexture()->getWidth() / it->getRenderTarget().getRenderTexture()->getHeight(), it->getZNear(), it->getZFar());
#else
					passProjectionMatrix = glm::perspective(it->getFovY(), it->getRenderTarget().getRenderTexture()->getWidth() / (float)it->getRenderTarget().getRenderTexture()->getHeight(), it->getZNear(), it->getZFar());
#endif
				}
				// if a lightsource is attached, we ask it to perform any steps needed
				// before a pass can be rendered
				auto* lightComponent = lightSystem->get(comp.getEntityId());
				if(lightComponent != nullptr)
				{
					lightComponent->_prePassRendering(&(*it), mainCamera);
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
					else/* if(it->getPov() == PointOfView::LOCAL)*/
					{
						// try to get the local transformation component
						auto* transformation = transformSystem->get(comp.getEntityId());
						if(transformation != nullptr)
						{
							// WAS HERE
							// only use translation part
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
					if(lightComponent != nullptr)
					{
						// passProjectionMatrix and passCameraSpaceMatrix may be modified here!
						lightComponent->_prePassIterationRendering(&(*it), renderIter, mainCamera, cameraSpaceMatrix, passProjectionMatrix, passCameraSpaceMatrix, mWorldAABB);
					}
					// setup the frustum to use while rendering the pass
					Frustum frustum;
					frustum.setup(passProjectionMatrix * passCameraSpaceMatrix);

					_renderAllComponents(&frustum, passProjectionMatrix, passCameraSpaceMatrix, inverseCameraRotation, glm::vec4(cameraPosition, 1.f), transformSystem, lightSystem, &(*it), comp.getEntityId());
					
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
RenderSystem::_renderAllComponents(
				Frustum const* frustum,
				glm::mat4 const& projectionMatrix,
				glm::mat4 const& cameraSpaceMatrix,
				glm::mat4 const& inverseCameraRotation,
				glm::vec4 const& cameraPosition,
				TransformSystem* transformSystem,
				LightSystem* lightSystem,
				G2::Pass const* pass,
				unsigned int entityIdToSkip
)
{
	// render for each RenderStatesGroup
	for (int i = 0; i < mRenderSortedComponents.size() ; ++i) 
	{
		std::shared_ptr<RenderStatesGroup> componentGroup = mRenderSortedComponents[i];
		// send states to renderer
		componentGroup->getRenderStates().applyStates(pass != nullptr);
		auto const& renderComponentIds = componentGroup->getEntityIds();
		for (int k = 0; k < renderComponentIds.size() ; ++k) 
		{
			RenderComponent* comp = get(renderComponentIds[k]);
			if(comp->getEntityId() == entityIdToSkip || comp->material.isTransparent())
			{
				continue;
			}
			bool unculledFound = _performFrustumCulling(comp, frustum);
			if(unculledFound)
			{
				// calc which shader to use for rendering
				std::shared_ptr<Shader> shader;
				if(pass != nullptr && pass->hasShader())
				{
					shader = _getPassRenderShader(comp, pass);
				}
				else
				{
					shader = _getRenderShader(comp);
				}
				// bind shader before call render()
				shader->bind();
				// regular rendering
				_render(projectionMatrix, cameraSpaceMatrix, inverseCameraRotation, comp, shader, transformSystem, lightSystem);
			}
		}
	}
	
	// perform Z-Sorting for transparent vertex array objects
	std::sort(std::begin(mZSortedTransparentEntityIdsToDrawCall), std::end(mZSortedTransparentEntityIdsToDrawCall), ZSorter(this, cameraPosition));
	
	// render all transparent vertex array objects Z-ordered
	for (int i = 0; i < mZSortedTransparentEntityIdsToDrawCall.size() ; ++i) 
	{
		// TODO No frustum culling so far for transparent objects!
		RenderComponent* comp = get(mZSortedTransparentEntityIdsToDrawCall[i].first);
		if(comp->getEntityId() == entityIdToSkip)
		{
			continue;
		}
		// calc which shader to use for rendering
		std::shared_ptr<Shader> shader;
		if(pass != nullptr && pass->hasShader())
		{
			shader = _getPassRenderShader(comp, pass);
		}
		else 
		{
			shader = _getRenderShader(comp);
		}
		// send states to renderer
		comp->mRenderStatesGroup->getRenderStates().applyStates(pass != nullptr);
		// bind shader before call render()
		shader->bind();
		// Enable blending

		GLDEBUG( glDisable(GL_CULL_FACE) );
		GLDEBUG( glDepthMask(false) );
		_render(projectionMatrix, cameraSpaceMatrix, inverseCameraRotation, comp, shader, transformSystem, lightSystem, mZSortedTransparentEntityIdsToDrawCall[i].second);
		GLDEBUG( glDepthMask(true) );
		GLDEBUG( glEnable(GL_CULL_FACE) );
	}
}

void
RenderSystem::_render(glm::mat4 const& projectionMatrix, glm::mat4 const& cameraSpaceMatrix, glm::mat4 const& inverseCameraRotation, RenderComponent* component, std::shared_ptr<Shader>& boundShader, TransformSystem* transformSystem, LightSystem* lightSystem, int drawCallToDraw)
{
	// Upload Matrices
	auto* transformation = transformSystem->get(component->getEntityId());
	_uploadMatrices(boundShader, transformation, projectionMatrix, cameraSpaceMatrix, inverseCameraRotation, component->billboarding);

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
		_uploadLight(boundShader, &lightComponent, cameraSpaceMatrix, numActive++);
	}
	boundShader->setProperty(std::move(std::string("G2ActiveLights")), numActive);

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
	_uploadMaterial(boundShader, &component->material);

	// Draw all attached VAO
	if(drawCallToDraw == -1)
	{ // draw all
		for (unsigned int k = 0; k < component->getNumDrawCalls(); ++k) 
		{
			DrawCall& drawCall = component->getDrawCall(k);
			if(drawCall.isFrustumCulled() || !drawCall.isEnabled())
			{
				continue;
			}
			// get the referenced VertexArrayObject of this DrawCall
			auto& vao = component->mVaos[drawCall.getVaoIndex()];
			vao.bind();
			if(drawCall.getIaoIndex() != -1)
			{
				// DrawCall references an IndexArrayObject as well
				auto& iao = component->mIaos[drawCall.getIaoIndex()];
				iao.bind();
				iao.draw(drawCall.getDrawMode());
				iao.unbind();
			}
			else
			{
				vao.draw(drawCall.getDrawMode());
			}
			vao.unbind();
		}
	}
	else
	{ // only draw given drawCall
		DrawCall& drawCall = component->getDrawCall(drawCallToDraw);
		
		if(drawCall.isFrustumCulled() || !drawCall.isEnabled())
		{
			return;
		}
		// get the referenced VertexArrayObject of this DrawCall
		auto& vao = component->mVaos[drawCall.getVaoIndex()];
		vao.bind();
		if(drawCall.getIaoIndex() != -1)
		{
			// DrawCall references an IndexArrayObject as well
			auto& iao = component->mIaos[drawCall.getIaoIndex()];
			iao.bind();
			iao.draw(drawCall.getDrawMode());
			iao.unbind();
		}
		else
		{
			vao.draw(drawCall.getDrawMode());
		}
		vao.unbind();
	}
}

void
RenderSystem::_uploadMatrices(std::shared_ptr<Shader>& shader, TransformComponent* transformation, glm::mat4 const& projectionMatrix, glm::mat4 const& cameraSpaceMatrix, glm::mat4 const& inverseCameraRotation, bool billboarding) 
{

	// TEMP UNTIL PROJECTION MATRIX UPDATES ARE TRACKED IN CAMERA!
	shader->setProperty(std::string("matrices.projection_matrix"), projectionMatrix);
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
		shader->setProperty(std::string("matrices.model_matrix"), modelMatrix);
		shader->setProperty(std::string("matrices.view_matrix"), cameraSpaceMatrix);
		shader->setProperty(std::string("matrices.modelview_matrix"), modelViewMatrix);
		if(transformation->getScale() == glm::vec3(1.f,1.f,1.f))
		{
			shader->setProperty(std::string("matrices.normal_matrix"), glm::mat3(modelViewMatrix));
		}
		else
		{
			// non-uniform scaling
			shader->setProperty(std::string("matrices.normal_matrix"), glm::inverseTranspose(glm::mat3(modelViewMatrix)));
		}
	}
	else 
	{
		shader->setProperty(std::string("matrices.model_matrix"), glm::mat4());
		shader->setProperty(std::string("matrices.view_matrix"), cameraSpaceMatrix);
		shader->setProperty(std::string("matrices.modelview_matrix"), cameraSpaceMatrix);
		shader->setProperty(std::string("matrices.normal_matrix"), glm::mat3(cameraSpaceMatrix));
	}
}

void
RenderSystem::_uploadLight(std::shared_ptr<Shader>& shader, LightComponent* light, glm::mat4 const& cameraSpaceMatrix, int index) 
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
	shader->setProperty(base + ".ambient", light->ambient);
	shader->setProperty(base + ".diffuse", light->diffuse);
	shader->setProperty(base + ".specular", light->specular);
	// set positional
	shader->setProperty(base + ".position", lightPos);
	shader->setProperty(base + ".direction", lightDir);
	shader->setProperty(base + ".range", 0.f); // only needed for spotlight/point light
	shader->setProperty(base + ".attenuation", glm::vec4(light->attenuation,light->linearAttenuation,light->exponentialAttenuation,0.f)); // only needed for spotlight/point light
	// set special
	shader->setProperty(base + ".cosCutoff", std::cosf(light->cutOffDegrees * (float)M_PI / 180.f)); // only needed for spotlight
	// set shadow
	std::shared_ptr<LightEffectState> const& lightEffectState = light->getLightEffectState();
	shader->setProperty(base + ".type", lightEffectState->shadowTechnique);
	shader->setProperty(base + ".numCascades", (int)lightEffectState->cascades);
	for(unsigned int i = 0; i < lightEffectState->cascades; ++i)
	{
		std::stringstream farStr;
		farStr << base << ".zFar[" << i << "]";
		shader->setProperty(farStr.str(), lightEffectState->farClipsHomogenous[i]);
		std::stringstream eyeToLightClipStr;
		eyeToLightClipStr << base << ".eyeToLightClip[" << i << "]";
		shader->setProperty(eyeToLightClipStr.str(), lightEffectState->eyeToLightClip[i]);
	}
}

void
RenderSystem::_uploadMaterial(std::shared_ptr<Shader>& shader, Material* material) 
{
	shader->setProperty(std::string("material.ambient"), material->getAmbient());
	shader->setProperty(std::string("material.diffuse"), material->getDiffuse());
	shader->setProperty(std::string("material.specular"), material->getSpecular());
	shader->setProperty(std::string("material.shininess"), material->getShininess());
}

std::shared_ptr<Shader>
RenderSystem::_getRenderShader(RenderComponent* component) 
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
	if(component->_getShaderCache().isCacheValid(component->material.getVersion(), component->mVaos[0].getVersion()))
	{
		shader = component->_getShaderCache().getShader();
	}
	else if(component->mEffect.get() != nullptr && component->mEffect->hasCompiledShaders()) 
	{
		shader = component->getEffect()->getShader(&component->material,&component->mVaos[0]);
		// update shader cache
		component->_getShaderCache().setShader(shader, component->material.getVersion(), component->mVaos[0].getVersion());
	}
	else
	{
		shader = defaultEffect->getShader(&component->material,&component->mVaos[0]);
		// update shader cache
		component->_getShaderCache().setShader(shader, component->material.getVersion(), component->mVaos[0].getVersion());
	}
	return shader;
}

std::shared_ptr<Shader>
RenderSystem::_getPassRenderShader(RenderComponent* component, Pass const* pass) const
{
	// TODO add caching for pass shader as well
	return pass->getShader(component->material,component->mVaos[0]);;
}

void
RenderSystem::_recalculateModelSpaceAABB(RenderComponent* component, TransformSystem* transformSystem) 
{
	unsigned int drawCalls = component->getNumDrawCalls();
	for(unsigned int i = 0; i < drawCalls; ++i)
	{
		DrawCall& drawCall = component->getDrawCall(i);
		if(drawCall.getAABBCalculationMode() == AABBCalculationMode::MANUAL)
		{
			_recalculateWorldSpaceAABB(component, i, transformSystem);
			continue; // skip calculation for user provided AABBs
		}
		VertexArrayObject& vao = component->getVertexArray(drawCall.getVaoIndex());
		AABB& aabb = drawCall.mModelSpaceAABB;
		aabb.clear();
		if(drawCall.getIaoIndex() != -1)
		{
			IndexArrayObject& iao = component->getIndexArray(drawCall.getIaoIndex());
			float* vertexData = vao.getDataPointer(Semantics::POSITION, BufferAccessMode::READ_ONLY);
			
			aabb.clear();
			unsigned int componentsPerPosition = vao.getNumBytesBySemantic(Semantics::POSITION) / sizeof(float);
			unsigned int* indices = iao.getIndexPointer();
			for(unsigned int v = 0; v < iao.getNumElements(); ++v)
			{
				if(componentsPerPosition == 3)
				{
					aabb.merge(((glm::vec3*)vertexData)[indices[v]]);
				}
				else if(componentsPerPosition == 4)
				{
					aabb.merge(((glm::vec4*)vertexData)[indices[v]]);
				}
			}
			iao.returnIndexPointer();
			vao.returnDataPointer(Semantics::POSITION);
		}
		else
		{
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
		}
		// 
		_recalculateWorldSpaceAABB(component, i, transformSystem);
	}
}

void
RenderSystem::_recalculateWorldSpaceAABB(RenderComponent* component, int drawCallIndex, TransformSystem* transformSystem) 
{
	
	transformSystem->lock();
	auto* transformComponent = transformSystem->get(component->getEntityId());
			
	auto& drawCall = component->getDrawCall(drawCallIndex);
	if(transformComponent != nullptr)
	{
		drawCall.mWorldSpaceAABB = std::move(drawCall.getModelSpaceAABB().transform(transformComponent->getWorldSpaceMatrix()));
	}
	else
	{
		drawCall.mWorldSpaceAABB = drawCall.getModelSpaceAABB();
	}
	transformSystem->unlock();
}

void
RenderSystem::_onViewportResize(int w, int h) 
{
	if(mRenderType == RenderType::DEFERRED_SHADING)
	{
		// setup render target for deferred rendering 
		mDeferredShadingTarget = std::shared_ptr<MultipleRenderTarget>(new MultipleRenderTarget(w,h));
		mDeferredShadingTarget->allocateRenderTexture(BufferAttachment::COLOR_0);	// diffuse texture
		mDeferredShadingTarget->allocateRenderTexture(BufferAttachment::COLOR_1);	// normals texture
		mDeferredShadingTarget->allocateRenderTexture(BufferAttachment::COLOR_2);	// positions texture
	}

	// Setup render targets for post processing
	for(int i = 0; i < 2; ++i)
	{
		mPostProcessingRenderTargets[i] = std::shared_ptr<RenderTarget>(
			new RenderTarget(
				Sampler::DIFFUSE,
				std::shared_ptr<Texture>(new Texture2D(
					NEAREST, 
					NEAREST, 
					w, 
					h, 
					RGBA, 
					-1,
					WrapMode::CLAMP_TO_EDGE,
					WrapMode::CLAMP_TO_EDGE,
					false,
					nullptr
				)), 
				RenderTargetType::RT_2D
			)
		);
	}
	
	glm::vec2 windowSize(
		(float)mPostProcessingRenderTargets[0]->getRenderTexture()->getWidth(),
		(float)mPostProcessingRenderTargets[0]->getRenderTexture()->getHeight()
	);
	glm::vec2 pixelSize(
		1.f / windowSize.x,
		1.f / windowSize.y
	);
	for(int i = 0; i < mPostProcessingEffects.size(); ++i)
	{
		auto shader = mPostProcessingEffects[i]->getShader();
		shader->bind();
		shader->setProperty(std::string("postProcessInfo.pixelSize"), pixelSize);
		shader->setProperty(std::string("postProcessInfo.windowSize"), windowSize);
	}
}

void
RenderSystem::addPostProcessingEffect(std::shared_ptr<G2::Effect> effect) 
{
	mPostProcessingEffects.push_back(effect);
}

bool
RenderSystem::_performFrustumCulling(RenderComponent* comp, Frustum const* frustum) 
{
	bool unculledFound = false;
	for(unsigned int v = 0; v < comp->getNumDrawCalls(); ++v)
	{
		DrawCall& drawCall = comp->getDrawCall(v);
		if(frustum->inside(drawCall.getWorldSpaceAABB()))
		{
			unculledFound = true;
			drawCall.mWasFrustumCulled = false;
		}
		else
		{
			drawCall.mWasFrustumCulled = true;
		}
	}
	return unculledFound;
}

void
RenderSystem::scheduleAABBRecalculation(unsigned int entityId) 
{
	mRecalcAABBEntityIds.push_back(entityId);
}

void
RenderSystem::updateTransparencyMode(unsigned int entityId, bool transparent) 
{
	if(transparent)
	{
		auto state = mTransparentEntityIds.insert(entityId);
		if(state.second == true)
		{
			// was newly inserted
			auto* comp = get(entityId);
			// add mapping for RenderComponent
			unsigned int offset = (unsigned int)mZSortedTransparentEntityIdsToDrawCall.size();
			mZSortedTransparentEntityIdsToDrawCall.resize(mZSortedTransparentEntityIdsToDrawCall.size() + comp->getNumDrawCalls());
			for(unsigned int v = 0; v < comp->getNumDrawCalls(); ++v)
			{
				mZSortedTransparentEntityIdsToDrawCall[offset++] = std::make_pair(comp->getEntityId(),v);
			}
		}
	}
	else
	{
		if(mTransparentEntityIds.erase(entityId) == 1)
		{
			// was removed
			auto* comp = get(entityId);

			mZSortedTransparentEntityIdsToDrawCall.resize(mZSortedTransparentEntityIdsToDrawCall.size() - comp->getNumDrawCalls());
			// rebuild mapping completely
			unsigned int offset = 0;
			for(auto it = mTransparentEntityIds.begin(); it != mTransparentEntityIds.end(); ++it)
			{
				auto* comp = get(*it);
				for(unsigned int v = 0; v < comp->getNumDrawCalls(); ++v)
				{
					mZSortedTransparentEntityIdsToDrawCall[offset++] = std::make_pair(comp->getEntityId(),v);
				}
			}
		}
	}
}

void
RenderSystem::_onDrawCallResize(unsigned int entityId, int sizeDifference) 
{
	auto* comp = get(entityId);
	if(comp->material.isTransparent())
	{
		// should be already registered
		mZSortedTransparentEntityIdsToDrawCall.resize(mZSortedTransparentEntityIdsToDrawCall.size() + sizeDifference);
	}
}

void
RenderSystem::onComponentAdded(unsigned int entityId) 
{
	RenderComponent* component = get(entityId);
	RenderStates defaultStates;

	_updateRenderStatesGroup(component, &defaultStates);
}

void
RenderSystem::onComponentRemoved(unsigned int entityId) 
{
	for(auto it = mRenderSortedComponents.begin(); it != mRenderSortedComponents.end(); ++it)
	{
		if((*it)->erase(entityId))
		{
			// cleanup since there might be a group with no entities
			_deleteEmptyRenderStatesGroups();
			return;
		}
	}
}

void
RenderSystem::_updateRenderStatesGroup(RenderComponent* component, RenderStates* newRenderStates) 
{
	for(auto it = mRenderSortedComponents.begin(); it != mRenderSortedComponents.end(); ++it)
	{
		if((*it)->consume(component->getEntityId(), *newRenderStates))
		{
			// link component to group
			component->_updateRenderStatesGroupLinkage(*it);
			// cleanup since there might be a group with no entities
			_deleteEmptyRenderStatesGroups();
			return;
		}
	}
	// create new group
	mRenderSortedComponents.push_back(std::shared_ptr<RenderStatesGroup>(new RenderStatesGroup(component->getEntityId(), *newRenderStates)));

	/*********************************************************************************************************
	 * TODO If Entity is currently in a group were it is the only member -> just change RenderStates object! *
	 *********************************************************************************************************/
	// link component to group
	component->_updateRenderStatesGroupLinkage(mRenderSortedComponents.back());
	// cleanup since there might be a group with no entities
	_deleteEmptyRenderStatesGroups();
}

void
RenderSystem::_deleteEmptyRenderStatesGroups() 
{
	for(int i = 0; i < mRenderSortedComponents.size(); ++i)
	{
		if(mRenderSortedComponents[i]->getEntityIds().size() == 0)
		{
			std::swap(mRenderSortedComponents[i], mRenderSortedComponents.back());
			mRenderSortedComponents.pop_back();
			--i;
		}
	}
}