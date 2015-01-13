#include "RotationHandler.h"
#include "GEARStudioEvents.h"

#include <G2Core/ECSManager.h>

RotationHandler::RotationHandler(unsigned int xAxisHandlerId, unsigned int yAxisHandlerId, unsigned int zAxisHandlerId) :
	mXAxisHandlerId(xAxisHandlerId),
	mYAxisHandlerId(yAxisHandlerId),
	mZAxisHandlerId(zAxisHandlerId),
	mState(ROTATION_NOT),
	mEntity(nullptr),
	G2S::HandlerBase(G2S::HandleMode::ROTATION_HANDLE)
{
	mRenderSystem = G2::ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>();
	mCameraSystem = G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>();
	mTransformSystem = G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>();

	// register to GEAR events
	G2::EventDistributer::onMouseUp.hook(this, &RotationHandler::_onMouseUp);
	G2::EventDistributer::onMouseMove.hook(this, &RotationHandler::_onMouseMove);

	// register to Editor events
	G2S::onEditorHandleSelected.hook(this, &RotationHandler::_onEditorHandleSelected);
	G2S::onManagedEntitySelected.hook(this, &RotationHandler::_onManagedEntitySelected);
}

RotationHandler::~RotationHandler()
{
	// unhook from GEAR events
	G2::EventDistributer::onMouseUp.unHookAll(this);
	G2::EventDistributer::onMouseMove.unHookAll(this);

	// unhook from Editor events
	G2S::onEditorHandleSelected.unHookAll(this);
	G2S::onManagedEntitySelected.unHookAll(this);
}

void RotationHandler::_onEditorHandleSelected(unsigned int entityId, G2::Intersection const& intersection)
{
	mRotationPlaneOffset = 0.f;
	if (entityId == mXAxisHandlerId)
	{
		mState = ROTATION_X_AXIS;
		mRotationPlaneNormal = glm::vec3(0.f, -1.f, 0.f);
		mEntity->addComponent<G2::TransformComponent>();
		mRotationPlaneOffset = mEntity->getComponent<G2::TransformComponent>()->getPosition().y;
	}
	else if (entityId == mYAxisHandlerId)
	{
		mState = ROTATION_Y_AXIS;
		mRotationPlaneNormal = glm::vec3(0.f, 0.f, -1.f);
		mEntity->addComponent<G2::TransformComponent>();
		mRotationPlaneOffset = mEntity->getComponent<G2::TransformComponent>()->getPosition().z;
	}
	else if (entityId == mZAxisHandlerId)
	{
		mState = ROTATION_Z_AXIS;
		mRotationPlaneNormal = glm::vec3(0.f, -1.f, 0.f);
		mEntity->addComponent<G2::TransformComponent>();
		mRotationPlaneOffset = mEntity->getComponent<G2::TransformComponent>()->getPosition().y;
	}

	if (mState != ROTATION_NOT)
	{
		// save start state
		mHandlerStartPoint = intersection.getPoint();
		mEntityStartPosition = mEntity->getComponent<G2::TransformComponent>()->getPosition();
		//mEntityStartScale = mEntity->getComponent<G2::TransformComponent>()->getScale();
		mPickOffset = mHandlerStartPoint - mEntityStartPosition;
	}
}

void RotationHandler::_onManagedEntitySelected(ManagedEntity* entity)
{
	mEntity = entity;
}

void RotationHandler::handleActivityChanged()
{
	// handle visibility
	mRenderSystem->get(mXAxisHandlerId)->setDrawcallEnabled(isHandleActive());
	mRenderSystem->get(mYAxisHandlerId)->setDrawcallEnabled(isHandleActive());
	mRenderSystem->get(mZAxisHandlerId)->setDrawcallEnabled(isHandleActive());
}

void RotationHandler::_onMouseUp(G2::MouseButton button, glm::detail::tvec2<int> const& pos)
{
	// TODO Button should be defined in a button mapping to be configurable!
	if (button == G2::MOUSE_LEFT)
	{
		// reset state to become inactive
		if (mState != ROTATION_NOT)
		{
			mState = ROTATION_NOT;
			G2S::onScaleHandleReleased();
		}
	}
}

void RotationHandler::_onMouseMove(glm::detail::tvec2<int> const& pos)
{
	//if (mState != ROTATION_NOT)
	//{
	//	// intersect the mouse ray with the XZ-plane on the Y value of the 

	//	auto* camera = mCameraSystem->getRenderCamera();
	//	auto* transform = mTransformSystem->get(camera->getEntityId());

	//	G2::Ray mouseRay = G2::Ray::createScreenProjectionRay(
	//		pos.x,
	//		pos.y,
	//		transform->getWorldSpaceMatrix(),
	//		camera->getProjectionMatrix(),
	//		glm::detail::tvec4<int>(0, 0, camera->getViewportWidth(), camera->getViewportHeight()));

	//	bool intersection;
	//	glm::vec3 ptOnPlane = _getPlaneIntersection(mouseRay, &intersection);
	//	if (intersection)
	//	{

	//		glm::vec3 scale = mEntityStartScale;

	//		if (mState == ROTATION_X_AXIS)
	//		{
	//			float dist = (ptOnPlane.x - mPickOffset.x);
	//			// the distance from the pick position to the origin on the currrent axis is mapped to a scale multiplication of 1.0
	//			float localScale = dist / mEntityStartPosition.x;
	//			scale.x *= localScale;
	//		}
	//		else if (mState == ROTATION_Y_AXIS)
	//		{
	//			float dist = (ptOnPlane.y - mPickOffset.y);
	//			// the distance from the pick position to the origin on the currrent axis is mapped to a scale multiplication of 1.0
	//			float localScale = dist / mEntityStartPosition.y;
	//			scale.y *= localScale;
	//		}
	//		else if (mState == ROTATION_Z_AXIS)
	//		{
	//			float dist = (ptOnPlane.z - mPickOffset.z);
	//			// the distance from the pick position to the origin on the currrent axis is mapped to a scale multiplication of 1.0
	//			float localScale = dist / mEntityStartPosition.z;
	//			scale.z *= localScale;
	//		}
	//		auto* tc = mEntity->addComponent<G2::TransformComponent>();
	//		tc->setScale(scale);
	//		G2S::onScaleHandleMoved();
	//	}
	//}
}

glm::vec3
RotationHandler::_getPlaneIntersection(G2::Ray const& ray, bool* result) const
{
	glm::vec3 const& normal = mRotationPlaneNormal;
	float const& offset = mRotationPlaneOffset;
	// Find t.
	float dot = glm::dot(normal, glm::vec3(ray.getDir()));
	if (dot == 0.f)
	{
		// ray is parallel top plane
		(*result) = false;
		return glm::vec3();
	}
	double t = -(glm::dot(normal, ray.getOrigin()) + offset) / (dot);
	if (t < 0)
	{
		// the ray does not hit the surface, that is, the surface is "behind" the ray
		(*result) = false;
		return glm::vec3();
	}
	// Get a point on the plane.
	glm::vec3 p = ray.getOrigin() + glm::vec3(ray.getDir() * t);

	(*result) = true;
	return p;
}