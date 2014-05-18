// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "CameraComponent.h"

#include <glm/ext.hpp>

using namespace G2;

CameraComponent::CameraComponent(std::string const& name) :
	mName(name),
	mMoveSpeed(0.02f),
	mRotationSpeed(0.2f),
	mViewportWidth(0),
	mViewportHeight(1)
{
}

CameraComponent::CameraComponent(CameraComponent && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

CameraComponent& CameraComponent::operator=(CameraComponent && rhs) 
{
	mName = std::move(rhs.mName);
	mMoveSpeed = rhs.mMoveSpeed;
	mRotationSpeed = rhs.mRotationSpeed;
	mProjectionMatrix = std::move(rhs.mProjectionMatrix);
	mInverseCameraRotation = std::move(rhs.mInverseCameraRotation);
	mViewportWidth = rhs.mViewportWidth;
	mViewportHeight = rhs.mViewportHeight;
	
	return static_cast<CameraComponent&>(BaseComponent::operator=(std::move(rhs)));
}

void
CameraComponent::setAsRenderCamera() 
{
	system->setRenderCamera(getEntityId());
}

void
CameraComponent::setProjectionMatrix(glm::mat4 const& value, int width, int height) 
{
	mProjectionMatrix = value;
	mViewportWidth = width;
	mViewportHeight = height;
	if(mViewportHeight == 0)
	{
		mViewportHeight = 1;
	}
}