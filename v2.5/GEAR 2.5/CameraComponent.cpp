// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "CameraComponent.h"

#include <glm/ext.hpp>

using namespace G2;

CameraComponent::CameraComponent(std::string const& name) :
	mMoveSpeed(0.02f),
	mRotationSpeed(0.2f),
	mTranslation(0.f,0.5f,0.f),
	mUpVector(0.f,1.f,0.f),
	mViewVector(0.f,0.f,-1.f),
	mStrafeVector(1.f,0.f,0.f),
	mChanged(true) 
{
		// initial rotation
		rotate(15.f, 135.f);
}

void 
CameraComponent::stepForward() 
{
	mTranslation += mViewVector * mMoveSpeed;
	mChanged = true;
}

void 
CameraComponent::stepBackward() 
{
	mTranslation -= mViewVector * mMoveSpeed;
	mChanged = true;
}

void 
CameraComponent::stepLeft() 
{
	mTranslation -= mStrafeVector * mMoveSpeed;
	mChanged = true;
}

void 
CameraComponent::stepRight() 
{	
	mTranslation += mStrafeVector * mMoveSpeed;
	mChanged = true;
}

void 
CameraComponent::rotate(float degreesX, float degreesY) 
{
	glm::mat4 rotX = glm::rotate(degreesX, glm::vec3(1,0,0));
	// accumulate the real Y-Axis to rotate around

	mUpVector = glm::rotateX(mUpVector, (degreesX));
	mUpVector = glm::normalize(mUpVector);
	
	glm::mat4 rotY = glm::rotate(degreesY, mUpVector);

	mRotation = rotY * rotX * mRotation;

	glm::mat4 infRot = glm::inverse(mRotation);
	
	mViewVector = glm::normalize(glm::vec3(infRot * glm::vec4(0.f,0.f,-1.f,0.f)));
	mStrafeVector = glm::normalize(glm::vec3(infRot * glm::vec4(1.f,0.f,0.f,0.f)));
	
	mChanged = true;
}

bool 
CameraComponent::updateModelView() 
{
	if(mChanged) 
	{
		mCameraSpaceMatrix = mRotation * glm::translate(-mTranslation);
		mNormalMatrix = glm::mat3(glm::inverseTranspose(mRotation));
		mChanged = false;
		return true;
	}
	return false;
}

void
CameraComponent::setAsRenderCamera() 
{
	system->setRenderCamera(getEntityId());
}

void
CameraComponent::setProjectionMatrix(glm::mat4 const& value) 
{
	mProjectionMatrix = value;
}