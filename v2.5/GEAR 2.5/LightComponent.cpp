#include "LightComponent.h"
#include "RenderComponent.h"
#include "CameraComponent.h"
#include "Pass.h"
#include "Texture.h"
#include "Logger.h"

#include <G2Core/ECSManager.h>
#include <glm/ext.hpp>


using namespace G2;

LightComponent::LightComponent(LightType::Name type)
	: mType(type),
	ambient(0.f,0.f,0.f,0.f),
	diffuse(1.f,1.f,1.f,1.f),
	specular(1.f,1.f,1.f,1.f),
	mEnabled(true),
	cutOffDegrees(360.f),
	attenuation(1.f),
	linearAttenuation(0.f),
	exponentialAttenuation(0.f),
	mShadowDescriptor(ShadowDescriptor::noShadows())
{
	if(type == LightType::POSITIONAL)
	{
		mDefaultPosition = mCachedPosition = glm::vec4(0.f,0.f,0.f,1.f);
	}
	else if(type == LightType::DIRECTIONAL)
	{
		// final light dir=-Z!
		mDefaultDirection = mCachedDirection = glm::vec3(0.f,0.f,1.f);
		mDefaultPosition = mCachedPosition = glm::vec4(0.f,0.f,0.f,0.f);
	}
	else if(type == LightType::SPOT)
	{
		// final light dir=-Y!
		mDefaultDirection = mCachedDirection = glm::vec3(0.f,0.f,-1.f);
		mDefaultPosition = mCachedPosition = glm::vec4(0.f,0.f,0.f,1.f);
	}
}

LightComponent&
LightComponent::setupColor(glm::vec4 const& ambient, glm::vec4 const& diffuse, glm::vec4 const& specular) 
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	return *this;
}

void
LightComponent::_updateTransformedPosition(glm::vec4 const& pos) 
{
	mCachedPosition = pos;
}

void
LightComponent::_updateTransformedDirection(glm::vec3 const& dir) 
{
	mCachedDirection = dir;
}

bool
LightComponent::configureShadows(ShadowDescriptor const& shadowDescriptor) 
{
	// check preconditions
	if(shadowDescriptor.numCascades > 0 && mType != LightType::DIRECTIONAL)
	{
		return false; // CSM only supported by directional lights so far
	}
	if(shadowDescriptor.numCascades > 4)
	{
		return false; // CSM only supported with up to 4 cascades
	}
	// setup effect to use
	std::string effectName = shadowDescriptor.customEffect;
	if(effectName.length() == 0)
	{
		// no custom effect
		if(shadowDescriptor.numCascades == 0)
		{
			if(mType != LightType::POSITIONAL)
			{
				effectName = "GEAR_SM.g2fx"; // simple shadow mapping
			}
			else
			{
				effectName = "GEAR_PLSM.g2fx"; // point light shadow mapping
			}
		}
		else
		{
			effectName = "GEAR_CSM.g2fx"; // cascaded shadow mapping for directional light
		}
	}
	// load effect
	std::shared_ptr<Effect> effect = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>()->getEngineEffectImporter().import(effectName);
	if(effect.get() == nullptr)
	{
		// something went wrong
		return false;
	}
	// setup render component
	auto* renderComponent = G2::BaseComponent<G2::RenderSystem>::getComponentByEntityId<G2::RenderComponent>(getEntityId());
	if(renderComponent == nullptr)
	{
		renderComponent = G2::BaseComponent<G2::RenderSystem>::create<G2::RenderComponent>(getEntityId());
	}
	renderComponent->setEffect(effect);
	mShadowDescriptor = shadowDescriptor;
	return true;
}

void
LightComponent::_prePassRendering(Pass const* pass, CameraComponent const* mainCamera) 
{
	if( pass->getRenderTarget().getRenderTargetType() == RenderTargetType::RT_2D_ARRAY &&
		mShadowDescriptor.numCascades == pass->getRenderTarget().getRenderTexture()->getDepth() &&
		mType == LightType::DIRECTIONAL)
	{
		// prepare the shadow descriptor to render Cascaded Shadow Maps
		// -> here we prepare the near/far clip planes to slice the main cameras frustum 
		mShadowDescriptor.splitWeight = 0.95f;
		mShadowDescriptor.splitDistFactor = 1.05f;
		mShadowDescriptor.setupSplitDistance(mainCamera->getZNear(),mainCamera->getZFar());
	}
}

void
LightComponent::_prePassIterationRendering(
	Pass const* pass, 
	int iterationIndex, 
	CameraComponent const* mainCamera, 
	glm::mat4 const& mainCameraSpaceMatrix,
	glm::mat4& passCameraSpaceMatrix
) 
{
	if( pass->getPov() == PointOfView::LOCAL &&
		pass->getRenderTarget().getRenderTargetType() == RenderTargetType::RT_2D_ARRAY &&
		mType == LightType::DIRECTIONAL)
	{
		// prepare to render Cascaded Shadow Maps
		// -> set the model view matrix to look into light direction
		passCameraSpaceMatrix = glm::lookAt(glm::vec3(),-getTransformedDirection(), glm::vec3(-1.f, 0.f, 0.f));

		mShadowDescriptor.setupFrustumPoints(
								iterationIndex, 
								mainCamera->getViewportWidth(), 
								mainCamera->getViewportHeight(), 
								mainCamera->getFovY(), 
								mainCameraSpaceMatrix, 
								glm::inverse(mainCameraSpaceMatrix), 
								mainCamera->getInverseCameraRotation()
							);
	}
}