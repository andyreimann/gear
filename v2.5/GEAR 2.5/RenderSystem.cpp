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
#include "TriangleTools.h"
#include "TriangleStream.h"

#include <G2Core/ECSManager.h>
#include <G2Core/EventDistributer.h>
#include <G2Core/GfxDevice.h>

#include <glm/ext.hpp>
#include <algorithm>

#define USE_FRUSTUM_CULLING

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
	mClearColor(0.25f, 0.25f, 0.25f, 1.f),
	mLastWindowSize(1.f,1.f)
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
	mFullScreenQuad.writeData(G2Core::Semantics::POSITION, geometry);
	glm::vec2 texCoord[4];
	texCoord[0] = glm::vec2(0.f,0.f);
	texCoord[1] = glm::vec2(1.f,0.f);
	texCoord[2] = glm::vec2(1.f,1.f);
	texCoord[3] = glm::vec2(0.f,1.f);
	mFullScreenQuad.writeData(G2Core::Semantics::TEXCOORD_0, texCoord);

	// setup the default texture
	glm::vec4 data(1.f, 1.f, 1.f, 1.f);
	mDefaultTex = std::shared_ptr<Texture2D>(
		new Texture2D(
		G2Core::FilterMode::NEAREST,
		G2Core::FilterMode::NEAREST,
		1u, 1u,
		G2Core::DataFormat::Base::RGBA,
		G2Core::DataFormat::Internal::R32G32B32A32_F,
		G2Core::WrapMode::REPEAT,
		G2Core::WrapMode::REPEAT,
		false,
		G2Core::DataType::FLOAT,
		&(data.x)
	));

	for (int i = 0; i <= G2Core::TexSlot::TEX_SLOT32; ++i)
	{
		mDefaultTex->bind(static_cast<G2Core::TexSlot::Name>(i));
	}
}

RenderSystem::~RenderSystem() 
{
	components.clear();
	EventDistributer::onViewportResize.unHook(this, &RenderSystem::_onViewportResize);
}

void
RenderSystem::runPhase(std::string const& name, FrameInfo const& frameInfo)
{
	if(name == G2Core::ECSPhase::POSTUPDATE) 
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
	else if (name == G2Core::ECSPhase::RENDER)
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
		mSceneRenderTarget->bind(0);
	}
	
	G2_gfxDevice()->setViewport(G2::rect(0.f,0.f,mainCamera->getViewportWidth(),mainCamera->getViewportHeight()));
	
	// TODO cache inverse camera space matrix!
	glm::vec4 cameraPosition = glm::inverse(cameraSpaceMatrix) * glm::vec4(0.f,0.f,0.f,1.f);

	_renderAllComponents(&mainCamera->getFrustum(), mainCamera->getProjectionMatrix(), cameraSpaceMatrix, inverseCameraRotation, cameraPosition, transformSystem, lightSystem, G2Core::Flags::ALL_FLAGS);
					
	if(mPostProcessingEffects.size() > 0)
	{
		// capture frame in first render target
		mSceneRenderTarget->unbind();
		// bind initial rendering of scene in Sampler::DIFFUSE when entering the post processing chaing
		mSceneRenderTarget->getRenderTexture()->bind(
			static_cast<G2Core::TexSlot::Name>(G2Core::TexSlot::TEX_SLOT1 + (int)mSceneRenderTarget->getRenderTextureSampler())
		);
	}

	mPostProcessingRenderStates.applyStates(false);

	_uploadMatrices(nullptr, glm::mat4(), glm::mat4(), glm::mat4(), false);

	for(int i = 0; i < mPostProcessingEffects.size(); ++i)
	{
		mPostProcessingEffects[i]->getShader()->bind();

		G2_gfxDevice()->setViewport(G2::rect(0.f, 0.f, mPostProcessingRenderTargets[i]->getRenderTexture()->getWidth(), mPostProcessingRenderTargets[i]->getRenderTexture()->getHeight()));


		if(i < mPostProcessingEffects.size() - 1)
		{
			// bind next ping pong render target for capturing
			mPostProcessingRenderTargets[i]->bind(0);
		}
		// draw post processing
		mFullScreenQuad.bind(mPostProcessingEffects[i]->getShader());
		mFullScreenQuad.draw(G2Core::DrawMode::QUADS, 4);
		mFullScreenQuad.unbind();
		// unbind render target
		if (i < mPostProcessingEffects.size() - 1)
		{
			// bind next ping pong render target for capturing
			mPostProcessingRenderTargets[i]->unbind();

			// bind output of this post processing step to the RenderTarget Sampler
			mPostProcessingRenderTargets[i]->getRenderTexture()->bind(
				static_cast<G2Core::TexSlot::Name>(G2Core::TexSlot::TEX_SLOT1 + (int)mPostProcessingRenderTargets[i]->getRenderTextureSampler())
			);
		}
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
	G2_gfxDevice()->setViewport(G2::rect(0.f,0.f,mainCamera->getViewportWidth(),mainCamera->getViewportHeight()));
	
	// TODO cache inverse camera space matrix!
	glm::vec4 cameraPosition = glm::inverse(cameraSpaceMatrix) * glm::vec4(0.f,0.f,0.f,1.f);

	_renderAllComponents(&mainCamera->getFrustum(), mainCamera->getProjectionMatrix(), cameraSpaceMatrix, inverseCameraRotation, cameraPosition, transformSystem, lightSystem, G2Core::Flags::ALL_FLAGS);
			
	mDeferredShadingTarget->unbind();

	mDeferredShadingPass = DeferredShadingPass::SHADING_PASS;
	// TODO render to full screen quad -> possible user defined shading shader
	// Need to find solution of how to get a Shader from an Effect without a material
			
	mDeferredShadingTarget->getRenderTexture(G2Core::FrameBufferAttachmentPoint::COLOR_0)->bind(G2Core::TexSlot::TEX_SLOT1);	// diffuse
	mDeferredShadingTarget->getRenderTexture(G2Core::FrameBufferAttachmentPoint::COLOR_1)->bind(G2Core::TexSlot::TEX_SLOT2);	// normals
	mDeferredShadingTarget->getRenderTexture(G2Core::FrameBufferAttachmentPoint::COLOR_2)->bind(G2Core::TexSlot::TEX_SLOT3);	// positions
			
	// combine everything back using one surface shader
	// here the POST PROCESSING can take place:  Glow, Distortion, Edge-Smoothing, Fog, ...
	//mShadingEffect->bind();
	//mFullScreenQuad.draw(GL_QUADS, 0);
	assert(false);
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
					else if(it->getPov() == PointOfView::MAIN_CAMERA_FLIP_Y)
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

					unsigned int entityIdToSkip = comp.getEntityId();

					if(!it->getSkipPassRenderComponent())
					{
						// we want to render the RenderComponent the Pass is attached to
						entityIdToSkip = G2::Entity::UNINITIALIZED_ENTITY_ID;
					}

					_renderAllComponents(&frustum, passProjectionMatrix, passCameraSpaceMatrix, inverseCameraRotation, glm::vec4(cameraPosition, 1.f), transformSystem, lightSystem, it->getRenderLayerMask(), &(*it), entityIdToSkip);
					
					it->getRenderTarget().unbind();
				}
				// bind result of pass
				it->getRenderTarget().getRenderTexture()->bind(
					static_cast<G2Core::TexSlot::Name>(G2Core::TexSlot::TEX_SLOT1+(int)it->getRenderTarget().getRenderTextureSampler())
				);
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
				G2Core::RenderLayer::RenderLayerMask validRenderLayers,
				G2::Pass const* pass,
				unsigned int entityIdToSkip
)
{

	// render for each RenderStatesGroup
	for (int i = 0; i < mRenderSortedComponents.size() ; ++i) 
	{
		std::shared_ptr<RenderStatesGroup> componentGroup = mRenderSortedComponents[i];

		if (componentGroup->getRenderStates().getRenderDepth() == false)
		{
			continue;
		}

		// send states to renderer
		componentGroup->getRenderStates().applyStates(pass != nullptr);
		auto const& renderComponentIds = componentGroup->getEntityIds();
		for (int k = 0; k < renderComponentIds.size() ; ++k) 
		{
			RenderComponent* comp = get(renderComponentIds[k]);
			if( comp->getEntityId() == entityIdToSkip || 
				comp->material.isTransparent() || 
				(comp->getRenderLayerMask() & validRenderLayers) == G2Core::Flags::NO_FLAGS)
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
		if(comp->getEntityId() == entityIdToSkip || 
		   (comp->getRenderLayerMask() & validRenderLayers) == G2Core::Flags::NO_FLAGS)
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

		G2_gfxDevice()->setCullFaceEnabled(false);
		G2_gfxDevice()->setDepthWritesEnabled(false);
		_render(projectionMatrix, cameraSpaceMatrix, inverseCameraRotation, comp, shader, transformSystem, lightSystem, mZSortedTransparentEntityIdsToDrawCall[i].second);
		G2_gfxDevice()->setDepthWritesEnabled(true);
		G2_gfxDevice()->setCullFaceEnabled(true);
	}

	// render for each RenderStatesGroup, which does not write depth values 
	for (int i = 0; i < mRenderSortedComponents.size(); ++i)
	{
		std::shared_ptr<RenderStatesGroup> componentGroup = mRenderSortedComponents[i];

		if (componentGroup->getRenderStates().getRenderDepth() == true)
		{
			continue;
		}
		// send states to renderer
		componentGroup->getRenderStates().applyStates(pass != nullptr);
		auto const& renderComponentIds = componentGroup->getEntityIds();
		for (int k = 0; k < renderComponentIds.size(); ++k)
		{
			RenderComponent* comp = get(renderComponentIds[k]);
			if (comp->getEntityId() == entityIdToSkip ||
				comp->material.isTransparent() ||
				(comp->getRenderLayerMask() & validRenderLayers) == G2Core::Flags::NO_FLAGS)
			{
				continue;
			}
			bool unculledFound = _performFrustumCulling(comp, frustum);
			if (unculledFound)
			{
				// calc which shader to use for rendering
				std::shared_ptr<Shader> shader;
				if (pass != nullptr && pass->hasShader())
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
	G2_gfxDevice()->setDepthWritesEnabled(true);
}

void
RenderSystem::_render(glm::mat4 const& projectionMatrix, glm::mat4 const& cameraSpaceMatrix, glm::mat4 const& inverseCameraRotation, RenderComponent* component, std::shared_ptr<Shader>& boundShader, TransformSystem* transformSystem, LightSystem* lightSystem, int drawCallToDraw)
{
	// Upload Matrices
	auto* transformation = transformSystem->get(component->getEntityId());
	_uploadMatrices(transformation, projectionMatrix, cameraSpaceMatrix, inverseCameraRotation, component->billboarding);
	_uploadAllLights(lightSystem, cameraSpaceMatrix);

	// Bind Textures
	auto const& textures = component->material.getTextures();
	for(auto it = textures.begin(); it != textures.end(); ++it)
	{
		if(it->second.get() != nullptr)
		{
			// tex slots are continuous numbers and samplers start at 0 -> simple calculation
			it->second->bind(
				static_cast<G2Core::TexSlot::Name>(G2Core::TexSlot::TEX_SLOT1+(int)it->first)
			);
		}
	}

	// Upload Material
	_uploadMaterial(&component->material);

	// Draw all attached VAO
	if(drawCallToDraw == -1)
	{ // draw all
		for (unsigned int k = 0; k < component->getNumDrawCalls(); ++k) 
		{
			DrawCall& drawCall = component->getDrawCall(k);
#ifdef USE_FRUSTUM_CULLING
			if (drawCall.isFrustumCulled() || !drawCall.isEnabled())
			{
				return;
			}
#else
			if (!drawCall.isEnabled())
			{
				return;
			}
#endif
			// get the referenced VertexArrayObject of this DrawCall
			auto& vao = component->mVaos[drawCall.getVaoIndex()];
			vao.bind(boundShader);
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
#ifdef USE_FRUSTUM_CULLING
		if(drawCall.isFrustumCulled() || !drawCall.isEnabled())
		{
			return;
		}
#else
		if(!drawCall.isEnabled())
		{
			return;
		}
#endif
		// get the referenced VertexArrayObject of this DrawCall
		auto& vao = component->mVaos[drawCall.getVaoIndex()];
		vao.bind(boundShader);
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


	// Bind back default texture on all slots this component has overridden
	for (auto it = textures.begin(); it != textures.end(); ++it)
	{
		if (it->second.get() != nullptr)
		{
			// tex slots are continuous numbers and samplers start at 0 -> simple calculation
			mDefaultTex->bind(
				static_cast<G2Core::TexSlot::Name>(G2Core::TexSlot::TEX_SLOT1 + (int)it->first)
			);
		}
	}
}

void
RenderSystem::_uploadMatrices(TransformComponent* transformation, glm::mat4 const& projectionMatrix, glm::mat4 const& cameraSpaceMatrix, glm::mat4 const& inverseCameraRotation, bool billboarding)
{
	// TEMP UNTIL PROJECTION MATRIX UPDATES ARE TRACKED IN CAMERA!
	mMatricesData.projectionMatrix = projectionMatrix;
	mMatricesData.viewMatrix = cameraSpaceMatrix;
	// TEMP END
	if(transformation != nullptr) 
	{
		if(!billboarding)
		{
			mMatricesData.modelMatrix = transformation->getWorldSpaceMatrix();
		}
		else
		{
			mMatricesData.modelMatrix = transformation->getWorldSpaceMatrix() * inverseCameraRotation;
		}
		mMatricesData.modelViewMatrix = mMatricesData.viewMatrix * mMatricesData.modelMatrix;
		if(transformation->getScale() == glm::vec3(1.f,1.f,1.f))
		{
			mMatricesData.normalMatrix = mMatricesData.modelViewMatrix;
		}
		else
		{
			// non-uniform scaling
			mMatricesData.normalMatrix = glm::mat4(glm::inverseTranspose(mMatricesData.modelViewMatrix));
		}
	}
	else 
	{
		mMatricesData.modelMatrix = glm::mat4();
		mMatricesData.viewMatrix = cameraSpaceMatrix;
		mMatricesData.modelViewMatrix = cameraSpaceMatrix;
		mMatricesData.normalMatrix = cameraSpaceMatrix;
	}
	mDefaultUBOs.matrices.bind();
	mDefaultUBOs.matrices.updateSubData(0, sizeof(G2Core::ShaderView::Matrices), &mMatricesData);
	mDefaultUBOs.matrices.unbind();
}

void
RenderSystem::_uploadAllLights(LightSystem* lightSystem, glm::mat4 const& cameraSpaceMatrix)
{
	int maxLights = std::min(8, (int)lightSystem->components.size());
	int numActive = 0;
	for (int l = 0; l < maxLights; ++l)
	{
		auto& lightComponent = lightSystem->components[l];
		if (!lightComponent.isEnabled())
		{
			continue;
		}
		_uploadLight(&lightComponent, lightSystem, cameraSpaceMatrix, numActive++);
	}
	lightSystem->mLightData.activeLights = numActive;

	mDefaultUBOs.lights.bind();
	static const int preLightSize = sizeof(int) + sizeof(sizeof(glm::vec3)); //activeLights + _preStructPaddingToN4
	mDefaultUBOs.lights.updateSubData(0, preLightSize + (sizeof(G2Core::ShaderView::Light) * numActive), &lightSystem->mLightData);
	mDefaultUBOs.lights.unbind();
}

void
RenderSystem::_uploadLight(LightComponent* light, LightSystem* lightSystem, glm::mat4 const& cameraSpaceMatrix, int index)
{
	G2Core::ShaderView::Light* lightData = &lightSystem->mLightData.light[index];

	if(light->getType() != LightType::DIRECTIONAL)
	{
		lightData->position = cameraSpaceMatrix * light->getTransformedPosition();
	}
	if(light->getType() != LightType::POSITIONAL)
	{
		lightData->direction = glm::normalize(glm::mat3(cameraSpaceMatrix) * light->getTransformedDirection());
	}
	// set coloring
	lightData->ambient = light->ambient;
	lightData->diffuse = light->diffuse;
	lightData->specular = light->specular;
	lightData->range = 0.f; // only needed for spotlight/point light
	lightData->attenuation = glm::vec4(light->attenuation, light->linearAttenuation, light->exponentialAttenuation, 0.f);
	// set special
	lightData->cosCutoff = std::cosf(light->cutOffDegrees * (float)M_PI / 180.f); // only needed for spotlight
	// set shadow
	std::shared_ptr<LightEffectState> const& lightEffectState = light->getLightEffectState();

	lightData->type = lightEffectState->shadowTechnique;
	lightData->numCascades = (int)lightEffectState->cascades;
	for(unsigned int i = 0; i < lightEffectState->cascades; ++i)
	{
		lightData->zFar[i] = lightEffectState->farClipsHomogenous[i];
		lightData->eyeToLightClip[i] = lightEffectState->eyeToLightClip[i];
	}
}

void
RenderSystem::_uploadMaterial(Material* material) 
{
	// Upload material shader view to UBO
	mDefaultUBOs.material.bind();
	mDefaultUBOs.material.updateSubData(0, sizeof(G2Core::ShaderView::Material), material->getShaderView());
	mDefaultUBOs.material.unbind();
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
	return pass->getShader(component->material,component->mVaos[0]);
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
			float* vertexData = vao.getDataPointer(G2Core::Semantics::POSITION, G2Core::BufferAccessMode::READ_ONLY);
			
			aabb.clear();
			unsigned int componentsPerPosition = vao.getNumBytesBySemantic(G2Core::Semantics::POSITION) / sizeof(float);
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
			vao.returnDataPointer(G2Core::Semantics::POSITION);
		}
		else
		{
			unsigned int componentsPerPosition = vao.getNumBytesBySemantic(G2Core::Semantics::POSITION) / sizeof(float);
			if(componentsPerPosition == 3)
			{
				glm::vec3* vertices = (glm::vec3*)vao.getDataPointer(G2Core::Semantics::POSITION, G2Core::BufferAccessMode::READ_ONLY);
				for(unsigned int v = 0; v < vao.getNumElements(); ++v)
				{
					aabb.merge(vertices[v]);
				}
				vao.returnDataPointer(G2Core::Semantics::POSITION);
			}
			else if(componentsPerPosition == 4)
			{
				glm::vec4* vertices = (glm::vec4*)vao.getDataPointer(G2Core::Semantics::POSITION, G2Core::BufferAccessMode::READ_ONLY);
				for(unsigned int v = 0; v < vao.getNumElements(); ++v)
				{
					aabb.merge(glm::vec3(vertices[v]));
				}
				vao.returnDataPointer(G2Core::Semantics::POSITION);
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
		mDeferredShadingTarget = std::shared_ptr<MultipleRenderTarget>(new MultipleRenderTarget(w,h,G2Core::DataFormat::Internal::R32G32B32A32_F));
		mDeferredShadingTarget->allocateRenderTexture(G2Core::FrameBufferAttachmentPoint::COLOR_0);	// diffuse texture
		mDeferredShadingTarget->allocateRenderTexture(G2Core::FrameBufferAttachmentPoint::COLOR_1);	// normals texture
		mDeferredShadingTarget->allocateRenderTexture(G2Core::FrameBufferAttachmentPoint::COLOR_2);	// positions texture
	}

	// Setup render targets for post processing
	for (int i = 0; i < mPostProcessingEffects.size(); ++i)
	{

		unsigned int customWidth = (unsigned int)mPostProcessingEffects[i]->getSetting("RenderTargetWidth", "-1").toInt();
		unsigned int customHeight = (unsigned int)mPostProcessingEffects[i]->getSetting("RenderTargetHeight", "-1").toInt();
		bool hasCustomSize = customWidth != -1 && customHeight != -1;

		if (!hasCustomSize)
		{
			mPostProcessingRenderTargets[i] = std::shared_ptr<RenderTarget>(
				new RenderTarget(
				Sampler::getSampler(mPostProcessingEffects[i]->getSetting("RenderTarget", "DIFFUSE").value),
				std::shared_ptr<Texture>(new Texture2D(
				G2Core::FilterMode::LINEAR,
				G2Core::FilterMode::LINEAR,
				w,
				h,
				G2Core::DataFormat::Base::RGBA,
				G2Core::DataFormat::Internal::R32G32B32A32_F,
				G2Core::WrapMode::CLAMP_TO_EDGE,
				G2Core::WrapMode::CLAMP_TO_EDGE,
				false,
				G2Core::DataType::UNSIGNED_BYTE,
				nullptr
				)),
				RenderTargetType::RT_2D
				)
			);
		}
	}

	// update the last known window size
	mLastWindowSize.x = (float)w;
	mLastWindowSize.y = (float)h;
	
	// update post processing uniforms for all Effects
	for(int i = 0; i < mPostProcessingEffects.size(); ++i)
	{
		_updatePostProcessingUniforms(mPostProcessingEffects[i]);
	}

	// re-initialize initial scene capturing if it is already existing (=if we have at least one post processing Effect)
	if(mSceneRenderTarget.get() != nullptr)
	{
		mSceneRenderTarget = std::shared_ptr<RenderTarget>(
			new RenderTarget(
				Sampler::DIFFUSE, // initial Scene rendering always binds to DIFFUSE
				std::shared_ptr<Texture>(
					new Texture2D(
						G2Core::FilterMode::LINEAR,
						G2Core::FilterMode::LINEAR,
						(unsigned int)mLastWindowSize.x,
						(unsigned int)mLastWindowSize.y,
						G2Core::DataFormat::Base::RGBA,
						G2Core::DataFormat::Internal::R32G32B32A32_F,
						G2Core::WrapMode::CLAMP_TO_EDGE,
						G2Core::WrapMode::CLAMP_TO_EDGE,
						false,
						G2Core::DataType::UNSIGNED_BYTE,
						nullptr
					)
				),
				RenderTargetType::RT_2D
			)
		);
	}
}

void
RenderSystem::addPostProcessingEffect(std::shared_ptr<G2::Effect> effect) 
{
	if (mSceneRenderTarget.get() == nullptr)
	{
		// lazy init initial scene capturing RenderTarget
		mSceneRenderTarget = std::shared_ptr<RenderTarget>(
			new RenderTarget(
				Sampler::DIFFUSE, // initial Scene rendering always binds to DIFFUSE
					std::shared_ptr<Texture>(
					new Texture2D(
						G2Core::FilterMode::LINEAR,
						G2Core::FilterMode::LINEAR,
						(unsigned int)mLastWindowSize.x,
						(unsigned int)mLastWindowSize.y,
						G2Core::DataFormat::Base::RGBA,
						G2Core::DataFormat::Internal::R32G32B32A32_F,
						G2Core::WrapMode::CLAMP_TO_EDGE,
						G2Core::WrapMode::CLAMP_TO_EDGE,
						false,
						G2Core::DataType::UNSIGNED_BYTE,
						nullptr
					)
				),
				RenderTargetType::RT_2D
			)
		);
	}

	// Add the Effect to the list of post processing effects.
	mPostProcessingEffects.push_back(effect);

	// Create a RenderTarget for the post processing shader.
	// The output sampler in the setting "RenderTarget" defines to which sampler the output is
	// bound after the post processing Effect was applied.
	// If the Effect is the last one in a chain of multiple post processing Effects,
	// The value is just ignored since the output is then written directly into the back buffer.
	unsigned int customWidth = (unsigned int)effect->getSetting("RenderTargetWidth", "-1").toInt();
	unsigned int customHeight = (unsigned int)effect->getSetting("RenderTargetHeight", "-1").toInt();
	bool hasCustomSize = customWidth != -1 && customHeight != -1;

	mPostProcessingRenderTargets.push_back(
		std::shared_ptr<RenderTarget>(
			new RenderTarget(
			Sampler::getSampler(effect->getSetting("RenderTarget", "DIFFUSE").value),
			std::shared_ptr<Texture>(
				new Texture2D(
					G2Core::FilterMode::LINEAR,
					G2Core::FilterMode::LINEAR,
					hasCustomSize ? customWidth : (unsigned int)mLastWindowSize.x,
					hasCustomSize ? customHeight : (unsigned int)mLastWindowSize.y,
					G2Core::DataFormat::Base::RGBA,
					G2Core::DataFormat::Internal::R32G32B32A32_F,
					G2Core::WrapMode::CLAMP_TO_EDGE,
					G2Core::WrapMode::CLAMP_TO_EDGE,
					false,
					G2Core::DataType::UNSIGNED_BYTE,
					nullptr
				)
			),
			RenderTargetType::RT_2D
			)
		)
	);
	// set post processing specific uniform variables if present in Effect
	_updatePostProcessingUniforms(mPostProcessingEffects.back());
}

void
RenderSystem::_updatePostProcessingUniforms(std::shared_ptr<G2::Effect> const& effect) const
{
	if (effect->hasCompiledShaders())
	{
		glm::vec2 pixelSize(
			1.f / mLastWindowSize.x,
			1.f / mLastWindowSize.y
			);
		auto shader = effect->getShader();
		shader->bind();
		shader->setProperty(std::string("postProcessInfo.pixelSize"), pixelSize);
		shader->setProperty(std::string("postProcessInfo.windowSize"), mLastWindowSize);
	}
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
G2::RenderSystem::_onDrawCallRemoved(unsigned int entityId, unsigned int drawCallIndex)
{
	auto* renderComp = get(entityId);
	if (renderComp != nullptr && renderComp->material.isTransparent())
	{
		if (renderComp->getNumDrawCalls() == 0)
		{
			mTransparentEntityIds.erase(entityId);
		}

		auto* comp = get(entityId);

		mZSortedTransparentEntityIdsToDrawCall.resize(mZSortedTransparentEntityIdsToDrawCall.size() - 1);
		// rebuild mapping completely
		unsigned int offset = 0;
		for (auto it = mTransparentEntityIds.begin(); it != mTransparentEntityIds.end(); ++it)
		{
			auto* comp = get(*it);
			for (unsigned int v = 0; v < comp->getNumDrawCalls(); ++v)
			{
				mZSortedTransparentEntityIdsToDrawCall[offset++] = std::make_pair(comp->getEntityId(), v);
			}
		}
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

Intersection
G2::RenderSystem::intersect(G2::Ray const& ray, G2Core::RenderLayer::RenderLayerMask renderLayers)
{
	Intersection closestMatch, intersection;

	auto* transformSystem = ECSManager::getShared().getSystem<TransformSystem, TransformComponent>();

	for(int i = 0; i < components.size(); ++i)
	{
		auto& comp = components[i];// check if this component has a pass attached

		if((renderLayers & comp.getRenderLayerMask()) == G2Core::Flags::NO_FLAGS)
		{
			continue; // none of the requested RenderLayers is assigned - skip
		}

		// TODO Transform Ray into model space to be able to perform intersection in model space!
		auto* compTransformation = transformSystem->get(comp.getEntityId());
		glm::mat4 worldSpaceMatrix;
		glm::mat4 invWorldSpaceMatrix;
		if(compTransformation != nullptr)
		{
			auto const& scale = compTransformation->getScale();
			if (scale.x == 0.f || scale.y == 0.f || scale.z == 0.f)
			{
				// there is no inverse matrix if we have at least one scaling value of 0!
				continue;
			}
			// TODO: accelerate!
			worldSpaceMatrix = compTransformation->getWorldSpaceMatrix();
			invWorldSpaceMatrix = glm::inverse(compTransformation->getWorldSpaceMatrix());
		}

		// transform the ray into model space
		Ray modelSpaceRay(
			glm::vec3(invWorldSpaceMatrix * glm::vec4(ray.getOrigin(), 1.f)),
			invWorldSpaceMatrix * ray.getDir()
		);

		for (unsigned int k = 0; k < comp.getNumDrawCalls(); ++k)
		{

			DrawCall& drawCall = comp.getDrawCall(k);
			if (!drawCall.isEnabled())
			{
				continue; // skip invisible meshes
			}
			if (drawCall.getWorldSpaceAABB().intersects(ray))
			{
				glm::vec3* p1, *p2, *p3;
				p1 = p2 = p3 = nullptr;
				// create a uniform triangle stream
				G2::TriangleStream triangleStream(
					G2Core::Semantics::POSITION,
					drawCall.getIaoIndex() == -1 ? nullptr : &comp.getIndexArray(drawCall.getIaoIndex()),
					&comp.getVertexArray(drawCall.getVaoIndex()),
					drawCall.getDrawMode()
					);
				// stream all data as triangles
				while (triangleStream.hasNext())
				{
					// stream triangle
					triangleStream.next(&p1, &p2, &p3);
					// run the intersection
					if(Intersection::rayTriangle(
						intersection,
						worldSpaceMatrix,
						ray, modelSpaceRay,
						*p1, *p2, *p3))
					{
						if (closestMatch.getState() == G2::IntersectionState::INTERSECTION)
						{
							// calculate distances in world space for closest match
							float dist = glm::length2(closestMatch.getPoint() - ray.getOrigin());
							// calculate distances in world space for intersection point (already in world space)
							float newDist = glm::length2(intersection.getPoint() - ray.getOrigin());
							if (newDist < dist)
							{
								closestMatch = intersection;
								closestMatch.setEntityId(comp.getEntityId());
							}
						}
						else
						{
							// just take it as it is - already in world space coordinates
							closestMatch = intersection;
							closestMatch.setEntityId(comp.getEntityId());
						}
					}
				}
			}
		}
	}
	return closestMatch;
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

void
G2::RenderSystem::_connectShaderToUniformBlocks(Shader* boundShader)
{
	if (boundShader != nullptr)
	{
		boundShader->_setUBOBlockBinding("G2Material", &mDefaultUBOs.material);
		boundShader->_setUBOBlockBinding("G2Matrices", &mDefaultUBOs.matrices);
		boundShader->_setUBOBlockBinding("G2LightInfo", &mDefaultUBOs.lights);
		boundShader->_setUBOBlockBinding("G2PostProcessingInfo", &mDefaultUBOs.postProcessingInfo);
	}
}