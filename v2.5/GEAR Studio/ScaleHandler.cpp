#include "ScaleHandler.h"
#include "GEARStudioEvents.h"

#include <G2Core/ECSManager.h>

ScaleHandler::ScaleHandler(unsigned int xAxisHandlerId, unsigned int yAxisHandlerId, unsigned int zAxisHandlerId) :
	mXAxisHandlerId(xAxisHandlerId),
	mYAxisHandlerId(yAxisHandlerId),
	mZAxisHandlerId(zAxisHandlerId),
	mState(SCALE_NOT),
	mEntity(nullptr)
{
	mCameraSystem = G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>();
	mTransformSystem = G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>();

	// register to GEAR events
	G2::EventDistributer::onFrameRendered.hook(this, &ScaleHandler::_onFrameRendered);
	G2::EventDistributer::onMouseUp.hook(this, &ScaleHandler::_onMouseUp);
	G2::EventDistributer::onMouseDown.hook(this, &ScaleHandler::_onMouseDown);
	G2::EventDistributer::onMouseMove.hook(this, &ScaleHandler::_onMouseMove);

	// register to Editor events
	GEARStudioEvents::onEditorHandleSelected.hook(this, &ScaleHandler::_onEditorHandleSelected);
	GEARStudioEvents::onManagedEntitySelected.hook(this, &ScaleHandler::_onManagedEntitySelected);
}

ScaleHandler::~ScaleHandler()
{
	// unhook from GEAR events
	G2::EventDistributer::onFrameRendered.unHookAll(this);
	G2::EventDistributer::onMouseUp.unHookAll(this);
	G2::EventDistributer::onMouseDown.unHookAll(this);
	G2::EventDistributer::onMouseMove.unHookAll(this);

	// unhook from Editor events
	GEARStudioEvents::onEditorHandleSelected.unHookAll(this);
	GEARStudioEvents::onManagedEntitySelected.unHookAll(this);
}

void ScaleHandler::_onEditorHandleSelected(unsigned int entityId, G2::Intersection const& intersection)
{
	mScalePlaneOffset = 0.f;
	if (entityId == mXAxisHandlerId)
	{
		mState = SCALE_X_AXIS;
		mScalePlaneNormal = glm::vec3(0.f, -1.f, 0.f);
		mEntity->addComponent<G2::TransformComponent>();
		mScalePlaneOffset = mEntity->getComponent<G2::TransformComponent>()->getPosition().y;
	}
	else if (entityId == mYAxisHandlerId)
	{
		mState = SCALE_Y_AXIS;
		mScalePlaneNormal = glm::vec3(0.f, 0.f, -1.f);
		mEntity->addComponent<G2::TransformComponent>();
		mScalePlaneOffset = mEntity->getComponent<G2::TransformComponent>()->getPosition().z;
	}
	else if (entityId == mZAxisHandlerId)
	{
		mState = SCALE_Z_AXIS;
		mScalePlaneNormal = glm::vec3(0.f, -1.f, 0.f);
		mEntity->addComponent<G2::TransformComponent>();
		mScalePlaneOffset = mEntity->getComponent<G2::TransformComponent>()->getPosition().y;
	}

	if (mState != SCALE_NOT)
	{
		// save start state
		mHandlerStartPoint = intersection.getPoint();
		mEntityStartPosition = mEntity->getComponent<G2::TransformComponent>()->getPosition();
		mPickOffset = mHandlerStartPoint - mEntityStartPosition;
	}
}

void
ScaleHandler::_onManagedEntitySelected(ManagedEntity* entity)
{
	mEntity = entity;
}

void ScaleHandler::_onFrameRendered(G2::FrameInfo const& frame)
{

}

void ScaleHandler::_onMouseUp(G2::MouseButton button, glm::detail::tvec2<int> const& pos)
{
	// TODO Button should be defined in a button mapping to be configurable!
	if (button == G2::MOUSE_LEFT)
	{
		// reset state to become inactive
		if (mState != SCALE_NOT)
		{
			mState = SCALE_NOT;
			GEARStudioEvents::onTranslationHandleReleased();
		}
	}
}

void ScaleHandler::_onMouseDown(G2::MouseButton button, glm::detail::tvec2<int> const& pos)
{

}

void ScaleHandler::_onMouseMove(glm::detail::tvec2<int> const& pos)
{
	//if (mState != SCALE_NOT)
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
	//		auto* tc = mEntity->addComponent<G2::TransformComponent>();

	//		glm::vec3 pos = tc->getPosition();
	//		glm::vec3 parentsScale = tc->getParentsScale();

	//		if (mState == SCALE_X_AXIS)
	//		{
	//			if (parentsScale.x == 0.f)
	//			{
	//				// div by 0 prevention
	//				parentsScale.x = 1.f;
	//			}
	//			pos.x = (ptOnPlane.x - mPickOffset.x) / parentsScale.x;
	//		}
	//		else if (mState == SCALE_Y_AXIS)
	//		{
	//			if (parentsScale.y == 0.f)
	//			{
	//				// div by 0 prevention
	//				parentsScale.y = 1.f;
	//			}
	//			pos.y = (ptOnPlane.y - mPickOffset.y) / parentsScale.y;
	//		}
	//		else if (mState == SCALE_Z_AXIS)
	//		{
	//			if (parentsScale.z == 0.f)
	//			{
	//				// div by 0 prevention
	//				parentsScale.z = 1.f;
	//			}
	//			pos.z = (ptOnPlane.z - mPickOffset.z) / parentsScale.z;
	//		}
	//		tc->setPosition(pos);
	//		GEARStudioEvents::onTranslationHandleMoved();
	//	}
	//}
}

glm::vec3
ScaleHandler::_getPlaneIntersection(G2::Ray const& ray, bool* result) const
{
	glm::vec3 const& normal = mScalePlaneNormal;
	float const& offset = mScalePlaneOffset;
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