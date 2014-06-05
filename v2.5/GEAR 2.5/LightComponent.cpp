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
	glm::mat4& passProjectionMatrix,
	glm::mat4& passCameraSpaceMatrix,
	AABB const& worldAABB
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
		// APPLY CROP MATRIX TO PROJECTION MATRIX
		float maxX = -FLT_MAX;
		float maxY = -FLT_MAX;
		float maxZ;
		float minX =  FLT_MAX;
		float minY =  FLT_MAX;
		float minZ;

		glm::mat4 nv_mvp;
		glm::vec4 transf;	
	
		// find the z-range of the current frustum as seen from the light
		// in order to increase precision
	
		// note that only the z-component is need and thus
		// the multiplication can be simplified
		// transf.z = shad_modelview[2] * f.point[0].x + shad_modelview[6] * f.point[0].y + shad_modelview[10] * f.point[0].z + shad_modelview[14];
		// frustum points are in inverse camera space!

		// passCameraSpaceMatrix contains lookAt of light!

		transf = passCameraSpaceMatrix*mShadowDescriptor.frusta[iterationIndex].getCornerPoints()[0];
		minZ = transf.z;
		maxZ = transf.z;
		for(int l=1; l<8; l++) {
			transf = passCameraSpaceMatrix*mShadowDescriptor.frusta[iterationIndex].getCornerPoints()[l];
			if(transf.z > maxZ) maxZ = transf.z;
			if(transf.z < minZ) minZ = transf.z;
		}

		// @todo only a hack, because there is some bug in aabb retrieval of hole scene (value here should be reseted to 1.0 which was the default).
		float radius = 0.0;
		// make sure all relevant shadow casters are included
		// note that these here are dummy objects at the edges of our scene
		// @todo here we should use the AABB of the hole Scene 
					
		transf.z = worldAABB.getMin().z;
		if(transf.z + radius > maxZ) maxZ = transf.z + radius;
		if(transf.z - radius < minZ) minZ = transf.z - radius;
		transf.z = worldAABB.getMax().z;
		if(transf.z + radius > maxZ) maxZ = transf.z + radius;
		if(transf.z - radius < minZ) minZ = transf.z - radius;

		// set the projection matrix with the new z-bounds
		// note the inversion because the light looks at the neg. z axis
		// gluPerspective(LIGHT_FOV, 1.0, maxZ, minZ); // for point lights
		passProjectionMatrix = glm::ortho(-1.f,1.f,-1.f,1.f,-maxZ,-minZ);

		glm::mat4 shadowModelViewProjection = passProjectionMatrix * passCameraSpaceMatrix;


		// find the extends of the frustum slice as projected in light's homogeneous coordinates

		for(int l=0; l<8; l++) {
			transf = shadowModelViewProjection*mShadowDescriptor.frusta[iterationIndex].getCornerPoints()[l];

			transf.x /= transf.w;
			transf.y /= transf.w;

			if(transf.x > maxX) maxX = transf.x;
			if(transf.x < minX) minX = transf.x;
			if(transf.y > maxY) maxY = transf.y;
			if(transf.y < minY) minY = transf.y;
		}

		float scaleX = 2.0f/(maxX - minX);
		float scaleY = 2.0f/(maxY - minY);
		float offsetX = -0.5f*(maxX + minX)*scaleX;
		float offsetY = -0.5f*(maxY + minY)*scaleY;

		// apply a crop matrix to modify the projection matrix we got from glOrtho.
		glm::mat4 cropMatrix;
		cropMatrix = glm::translate(cropMatrix, glm::vec3(offsetX, offsetY,0.0f) );
		cropMatrix = glm::scale(cropMatrix, glm::vec3( scaleX, scaleY, 1.0f) );
					
					
		// adjust the view frustum of the light, so that it encloses the camera frustum slice fully.
		// note that this function calculates the projection matrix as it sees best fit
		passProjectionMatrix = cropMatrix * passProjectionMatrix;

		mShadowDescriptor.setOrthoFrustum(iterationIndex, passProjectionMatrix*passCameraSpaceMatrix);
		// build up a matrix to transform a vertex from eye space to light clip space
		glm::mat4 bias( 0.5f, 0.0f, 0.0f, 0.0f, 
						0.0f, 0.5f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.5f, 0.0f,
						0.5f, 0.5f, 0.5f, 1.0f);
		mShadowDescriptor.eyeToLightClip[iterationIndex] = bias * passProjectionMatrix * passCameraSpaceMatrix * glm::inverse(mainCameraSpaceMatrix);
							
		// farClip[i] is originally in eye space - tells us how far we can see.
		// Here we compute it in camera homogeneous coordinates. Basically, we calculate
		// cam_proj * (0, 0, f[i].fard, 1)^t and then normalize to [0; 1]
		mShadowDescriptor.farClipsHomogenous[iterationIndex] = 0.5f*(-mShadowDescriptor.farClips[iterationIndex]*glm::value_ptr(mainCamera->getProjectionMatrix())[10]+glm::value_ptr(mainCamera->getProjectionMatrix())[14])/mShadowDescriptor.farClips[iterationIndex] + 0.5f;
	}
}