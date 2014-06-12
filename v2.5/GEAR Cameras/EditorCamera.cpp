#include "EditorCamera.h"

#include <G2/AbstractWindow.h>
#include <G2/CameraComponent.h>
#include <G2/TransformComponent.h>
#include <G2Core/EventDistributer.h>

using namespace G2Cameras;

EditorCamera::EditorCamera(G2::AbstractWindow* window) 
	: mView(0.f,0.f,-1.f,0.f),
	mUp(0.f,1.f,0.f,0.f),
	mStrafe(1.f,0.f,0.f,0.f),
	mRotY(0),
	mRotX(0),
	mMouseCoords(0,0),
	mTranslationMode(NO_TRANSLATION),
	mRotationMode(NO_ROTATION),
	mSpeedBoost(10.f),
	mViewPlaneTranslationSpeed(0.05f),
	mRotationSpeed(0.05f),
	mWindow(window),
	mPaused(false)
{
	auto* cameraComponent = addComponent<G2::CameraComponent>("Editor Camera");
	cameraComponent->setMoveSpeed(1.0f);
	cameraComponent->setMoveSpeed(0.02f);

	addComponent<G2::TransformComponent>(G2::TransformMode::TRS);
	
	G2::EventDistributer::onMouseMove.hook(this, &EditorCamera::onMouseMove);
	G2::EventDistributer::onMouseUp.hook(this, &EditorCamera::onMouseUp);
	G2::EventDistributer::onMouseDown.hook(this, &EditorCamera::onMouseDown);
	G2::EventDistributer::onMouseWheel.hook(this, &EditorCamera::onMouseWheel);
	G2::EventDistributer::onKeyUp.hook(this, &EditorCamera::onKeyUp);
	G2::EventDistributer::onKeyDown.hook(this, &EditorCamera::onKeyDown);
}

EditorCamera::EditorCamera(EditorCamera && rhs) :
	mWindow(rhs.mWindow)
{
	G2::EventDistributer::onMouseMove.hook(this, &EditorCamera::onMouseMove);
	G2::EventDistributer::onMouseUp.hook(this, &EditorCamera::onMouseUp);
	G2::EventDistributer::onMouseDown.hook(this, &EditorCamera::onMouseDown);
	G2::EventDistributer::onMouseWheel.hook(this, &EditorCamera::onMouseWheel);
	G2::EventDistributer::onKeyUp.hook(this, &EditorCamera::onKeyUp);
	G2::EventDistributer::onKeyDown.hook(this, &EditorCamera::onKeyDown);

	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

EditorCamera&
EditorCamera::operator=(EditorCamera && rhs) 
{
	mWindow = rhs.mWindow;
	mView = rhs.mView;
	mUp = rhs.mUp;
	mStrafe = rhs.mStrafe;
	mRotY = rhs.mRotY;
	mRotX = rhs.mRotX;
	mSpeedBoost = rhs.mSpeedBoost;
	mViewPlaneTranslationSpeed = rhs.mViewPlaneTranslationSpeed;
	mRotationSpeed = rhs.mRotationSpeed;
	mMouseCoords = rhs.mMouseCoords;
	mTranslationMode = rhs.mTranslationMode;
	mRotationMode = rhs.mRotationMode;
	mPaused = rhs.mPaused;

	return static_cast<EditorCamera&>(G2::Entity::operator=(std::move(rhs)));
}

EditorCamera::~EditorCamera() 
{
	G2::EventDistributer::onMouseMove.unHook(this, &EditorCamera::onMouseMove);
	G2::EventDistributer::onMouseUp.unHook(this, &EditorCamera::onMouseUp);
	G2::EventDistributer::onMouseDown.unHook(this, &EditorCamera::onMouseDown);
	G2::EventDistributer::onMouseWheel.unHook(this, &EditorCamera::onMouseWheel);
	G2::EventDistributer::onKeyUp.unHook(this, &EditorCamera::onKeyUp);
	G2::EventDistributer::onKeyDown.unHook(this, &EditorCamera::onKeyDown);
}

EditorCamera&  
EditorCamera::rotate(float xDegrees, float yDegrees)
{
	mRotX += xDegrees;
	mRotY += yDegrees;

	// calculate orientation
	glm::mat4 orientation;
	orientation = glm::rotate(orientation, mRotX, glm::vec3(1,0,0));
	orientation = glm::rotate(orientation, mRotY, glm::vec3(0,1,0));
	// update camera vectors
	glm::mat4 invOrientation = glm::inverse(orientation);
	mView	= glm::normalize(invOrientation * glm::vec4(0.f,0.f,-1.f,0.f));
	mUp		= glm::normalize(invOrientation * glm::vec4(0.f,1.f, 0.f,0.f));
	mStrafe	= glm::normalize(invOrientation * glm::vec4(1.f,0.f, 0.f,0.f));
		
	getComponent<G2::TransformComponent>()->setRotation(orientation);
	return *this;
}

EditorCamera& 
EditorCamera::pan(float strafe, float up)
{
	getComponent<G2::TransformComponent>()->translate(glm::vec3(mStrafe) * strafe);
	getComponent<G2::TransformComponent>()->translate(glm::vec3(mUp) * up);
	return *this;
}

EditorCamera& 
EditorCamera::moveView(float units)
{
	getComponent<G2::TransformComponent>()->translate(glm::vec3(mView) * -units);
	return *this;
}

void
EditorCamera::onMouseMove(glm::detail::tvec2<int> const& mouseCoords) 
{
	if(mPaused)
	{
		return;
	}
	int dx = mouseCoords.x - mMouseCoords.x;
	int dy = mouseCoords.y - mMouseCoords.y;
		
	mMouseCoords = mouseCoords;

	if(mRotationMode == AROUND_LOCATION)
	{
		rotate(
			dy*getComponent<G2::CameraComponent>()->getRotationSpeed() * mSpeedBoost * mRotationSpeed,
			dx*getComponent<G2::CameraComponent>()->getRotationSpeed() * mSpeedBoost * mRotationSpeed
		);
	}
	else if(mTranslationMode == VIEW_PLANE)
	{
		pan(
			getComponent<G2::CameraComponent>()->getMoveSpeed() * -dx * mSpeedBoost * mViewPlaneTranslationSpeed,
			getComponent<G2::CameraComponent>()->getMoveSpeed() * dy * mSpeedBoost * mViewPlaneTranslationSpeed
		);
	}
}

void
EditorCamera::onMouseDown(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords) 
{
	if(mPaused)
	{
		return;
	}
	if(button == G2::MOUSE_LEFT)
	{
		mWindow->setHideMouseMode(true);
	}
	else if(button == G2::MOUSE_RIGHT)
	{
		mWindow->setHideMouseMode(true);
		mRotationMode = AROUND_LOCATION;
	}
	else if(button == G2::MOUSE_MIDDLE)
	{
		mWindow->setHideMouseMode(true);
		mTranslationMode = VIEW_PLANE;
	}
}

void
EditorCamera::onMouseUp(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords) 
{
	if(mPaused)
	{
		return;
	}
	if(button == G2::MOUSE_LEFT)
	{
		mWindow->setHideMouseMode(false);
	}
	else if(button == G2::MOUSE_RIGHT)
	{
		mWindow->setHideMouseMode(false);
		mRotationMode = NO_ROTATION;
	}
	else if(button == G2::MOUSE_MIDDLE)
	{
		mWindow->setHideMouseMode(false);
		mTranslationMode = NO_TRANSLATION;
	}
}

void
EditorCamera::onMouseWheel(int y) 
{
	if(mPaused)
	{
		return;
	}
	if(mTranslationMode != VIEW_PLANE)
	{
		moveView(getComponent<G2::CameraComponent>()->getMoveSpeed() * y * mSpeedBoost);
	}
}

void
EditorCamera::onKeyUp(G2::KeyCode keyCode) 
{
	if(keyCode == G2::KC_SPACE)
	{
		mSpeedBoost = 10.f;
	}
}

void
EditorCamera::onKeyDown(G2::KeyCode keyCode) 
{
	if(keyCode == G2::KC_SPACE)
	{
		mSpeedBoost = 1.f;
	}
}

void
EditorCamera::setViewport(int width, int height) 
{
	if(height == 0)
	{ 
		height = 1; 
	}
	getComponent<G2::CameraComponent>()
		->setProjectionMatrix(
			width, height, 0.01f, 50.f, 70.f
		);
}

EditorCamera&
EditorCamera::pause() 
{
	mPaused = true;
	return *this;
}

EditorCamera&
EditorCamera::unpause() 
{
	mPaused = false;
	return *this;
}