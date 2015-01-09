#include "LightComponent.h"
#include "RenderComponent.h"
#include "CameraComponent.h"
#include "Pass.h"
#include "Texture.h"
#include "Logger.h"

#include <G2Core/ECSManager.h>
#include <glm/ext.hpp>

static  glm::mat4 BIAS( 0.5f, 0.0f, 0.0f, 0.0f, 
						0.0f, 0.5f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.5f, 0.0f,
						0.5f, 0.5f, 0.5f, 1.0f);

using namespace G2;

glm::vec4 LightComponent::AMBIENT_DEFAULT = glm::vec4(0.f, 0.f, 0.f, 0.f);
glm::vec4 LightComponent::DIFFUSE_DEFAULT = glm::vec4(1.f, 1.f, 1.f, 1.f);
glm::vec4 LightComponent::SPECULAR_DEFAULT = glm::vec4(1.f, 1.f, 1.f, 1.f);
float LightComponent::ATTENUATION_CONST_DEFAULT = 1.f;
float LightComponent::ATTENUATION_LIN_DEFAULT = 0.f;
float LightComponent::ATTENUATION_EXP_DEFAULT = 0.f;

LightComponent::LightComponent(LightType::Name type)
	: mType(type),
	ambient(AMBIENT_DEFAULT),
	diffuse(DIFFUSE_DEFAULT),
	specular(SPECULAR_DEFAULT),
	mEnabled(true),
	cutOffDegrees(360.f),
	attenuation(ATTENUATION_CONST_DEFAULT),
	linearAttenuation(ATTENUATION_LIN_DEFAULT),
	exponentialAttenuation(ATTENUATION_EXP_DEFAULT),
	mShadowDescriptor(ShadowDescriptor::noShadows()),
	mLightEffectState(new LightEffectState)
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
	/********************************************************************************
	 * IMPORTANT																	*
	 * This function should create a shared pointer									*
	 * to a really lightweight runtime resource for lighting effects in general.	*
	 * Goal: Only set this object to a value, if the configuration stage passes!	*
	 ********************************************************************************/

	// check preconditions
	if(shadowDescriptor.mNumCascades > 0 && mType != LightType::DIRECTIONAL)
	{
		return false; // CSM only supported by directional lights so far
	}
	if(shadowDescriptor.mNumCascades > 4)
	{
		return false; // CSM only supported with up to 4 cascades
	}
	// setup effect to use
	std::string effectName = shadowDescriptor.mCustomEffect;
	if(effectName.length() == 0)
	{
		// no custom effect
		if(shadowDescriptor.mNumCascades == 0)
		{
			effectName = "GEAR_CSM.g2fx"; // cascaded shadow mapping for directional light
		}
	}
	// load effect
	std::shared_ptr<Effect> effect = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>()->getEngineEffectImporter().import(effectName);
	if(effect.get() == nullptr)
	{
		// something went wrong while loading effect file
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

	// initialize light effect state
	if(mShadowDescriptor.mTechnique == AdvancedShadowTechnique::CASCADED_SHADOW_MAPS)
	{
		// allocate memory
		mLightEffectState->nearClips.resize(mShadowDescriptor.mNumCascades);
		mLightEffectState->farClips.resize(mShadowDescriptor.mNumCascades);
		mLightEffectState->farClipsHomogenous.resize(mShadowDescriptor.mNumCascades);
		mLightEffectState->frustumPoints.resize(mShadowDescriptor.mNumCascades*8);
		mLightEffectState->eyeToLightClip.resize(mShadowDescriptor.mNumCascades);
		// init state
		mLightEffectState->cascades = mShadowDescriptor.mNumCascades;
		mLightEffectState->splitWeight = mShadowDescriptor.mSplitWeight;
		mLightEffectState->splitDistFactor = mShadowDescriptor.mSplitDistFactor;
		mLightEffectState->shadowTechnique = mShadowDescriptor.mTechnique;
	}
	return true;
}

void
LightComponent::setupSplitDistance(float zNear, float zFar) 
{
	float ratio = zFar/zNear;

	// set the first near clip plane to the full near clip plane of the user camera
	mLightEffectState->nearClips[0] = zNear; 

	for( unsigned int i = 1; i < mShadowDescriptor.mNumCascades; ++i ) {
		float si = i / (float)mShadowDescriptor.mNumCascades;

		mLightEffectState->nearClips[i] = mLightEffectState->splitWeight*(zNear*powf(ratio, si)) + (1-mLightEffectState->splitWeight)*(zNear + (zFar - zNear)*si);
		mLightEffectState->farClips[i-1] = mLightEffectState->nearClips[i] * mLightEffectState->splitDistFactor;
	}
	// set the last far clip plane to the full far clip plane of the user camera
	mLightEffectState->farClips[mShadowDescriptor.mNumCascades-1] = zFar;
}

void
LightComponent::setupFrustumPoints(int pass, int width, int height, float fovY, glm::mat4 modelView, glm::mat4 const& invCameraTransformation, glm::mat4 const& invCameraRotation) 
{
	glm::vec3 view_dir = glm::vec3(invCameraRotation * glm::vec4(0.f,0.f,-1.f,0.f));
	view_dir = glm::normalize(view_dir);

	glm::vec3 center = glm::vec3(invCameraTransformation * glm::vec4(0.f,0.f,0.f,1.f));

	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::cross(view_dir,up);

	// compute the center points of the near clip plane and the far clip plane
	glm::vec3 fc = center + view_dir*mLightEffectState->farClips[pass];
	glm::vec3 nc = center + view_dir*mLightEffectState->nearClips[pass];

	// compute the right orientation vector
	right = glm::normalize(right);
	up = glm::cross(right,view_dir);
	up = glm::normalize(up);

	float viewportRatio = (float)width/(float)height;

	// these heights and widths are half the heights and widths of
	float tan_fov_1_2 = (float)tan((fovY/57.2957795)/2.0f); // in radians
	// the near and far plane rectangles
	float near_height = tan_fov_1_2 * mLightEffectState->nearClips[pass];
	float near_width = near_height * viewportRatio;
	float far_height = tan_fov_1_2 * mLightEffectState->farClips[pass];
	float far_width = far_height * viewportRatio;

	unsigned int i = pass*8;
	mLightEffectState->frustumPoints[i] = glm::vec4(glm::vec3(nc - up*near_height - right*near_width), 1.f);
	mLightEffectState->frustumPoints[i+1] = glm::vec4(glm::vec3(nc + up*near_height - right*near_width), 1.f);
	mLightEffectState->frustumPoints[i+2] = glm::vec4(glm::vec3(nc + up*near_height + right*near_width), 1.f);
	mLightEffectState->frustumPoints[i+3] = glm::vec4(glm::vec3(nc - up*near_height + right*near_width), 1.f);
	mLightEffectState->frustumPoints[i+4] = glm::vec4(glm::vec3(fc - up*far_height - right*far_width), 1.f);
	mLightEffectState->frustumPoints[i+5] = glm::vec4(glm::vec3(fc + up*far_height - right*far_width), 1.f);
	mLightEffectState->frustumPoints[i+6] = glm::vec4(glm::vec3(fc + up*far_height + right*far_width), 1.f);
	mLightEffectState->frustumPoints[i+7] = glm::vec4(glm::vec3(fc - up*far_height + right*far_width), 1.f);
}

void
LightComponent::_prePassRendering(Pass const* pass, CameraComponent const* mainCamera) 
{
	if( pass->getRenderTarget().getRenderTargetType() == RenderTargetType::RT_2D_ARRAY &&
		mShadowDescriptor.mNumCascades == pass->getRenderTarget().getRenderTexture()->getDepth() &&
		mType == LightType::DIRECTIONAL)
	{
		// prepare the shadow descriptor to render Cascaded Shadow Maps
		// -> here we prepare the near/far clip planes to slice the main cameras frustum 
		setupSplitDistance(mainCamera->getZNear(),mainCamera->getZFar());
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

		setupFrustumPoints(
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

		transf = passCameraSpaceMatrix*mLightEffectState->frustumPoints[iterationIndex*8];
		minZ = transf.z;
		maxZ = transf.z;
		for(int l=1; l<8; l++) {
			transf = passCameraSpaceMatrix*mLightEffectState->frustumPoints[iterationIndex*8+l];
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
			transf = shadowModelViewProjection*mLightEffectState->frustumPoints[iterationIndex*8+l];

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

		glm::mat4 passMVP = passProjectionMatrix*passCameraSpaceMatrix;

		//mShadowDescriptor.setOrthoFrustum(iterationIndex, passMVP);
		// build up a matrix to transform a vertex from eye space to light clip space
		mLightEffectState->eyeToLightClip[iterationIndex] = BIAS * passMVP * glm::inverse(mainCameraSpaceMatrix);

		// farClip[i] is originally in eye space - tells us how far we can see.
		// Here we compute it in camera homogeneous coordinates. Basically, we calculate
		// cam_proj * (0, 0, f[i].fard, 1)^t and then normalize to [0; 1]
		mLightEffectState->farClipsHomogenous[iterationIndex] = 0.5f*(-mLightEffectState->farClips[iterationIndex]*glm::value_ptr(mainCamera->getProjectionMatrix())[10]+glm::value_ptr(mainCamera->getProjectionMatrix())[14])/mLightEffectState->farClips[iterationIndex] + 0.5f;
	}
}