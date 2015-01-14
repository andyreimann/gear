#include "ScaleHandler.h"
#include "GEARStudioEvents.h"

#include <G2Core/ECSManager.h>

ScaleHandler::ScaleHandler(unsigned int xAxisHandlerId, unsigned int yAxisHandlerId, unsigned int zAxisHandlerId) :
	mXAxisHandlerId(xAxisHandlerId),
	mYAxisHandlerId(yAxisHandlerId),
	mZAxisHandlerId(zAxisHandlerId),
	mState(SCALE_NOT),
	mEntity(nullptr),
	G2S::HandlerBase(G2S::HandleMode::SCALE_HANDLE)
{
	mRenderSystem = G2::ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>();
	mCameraSystem = G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>();
	mTransformSystem = G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>();

	// register to GEAR events
	G2::EventDistributer::onMouseUp.hook(this, &ScaleHandler::_onMouseUp);
	G2::EventDistributer::onMouseMove.hook(this, &ScaleHandler::_onMouseMove);

	// register to Editor events
	G2S::onEditorHandleSelected.hook(this, &ScaleHandler::_onEditorHandleSelected);
	G2S::onManagedEntitySelected.hook(this, &ScaleHandler::_onManagedEntitySelected);
}

ScaleHandler::~ScaleHandler()
{
	// unhook from GEAR events
	G2::EventDistributer::onMouseUp.unHookAll(this);
	G2::EventDistributer::onMouseMove.unHookAll(this);

	// unhook from Editor events
	G2S::onEditorHandleSelected.unHookAll(this);
	G2S::onManagedEntitySelected.unHookAll(this);
}

void ScaleHandler::_onEditorHandleSelected(unsigned int entityId, G2::Intersection const& intersection)
{
	if (!isHandleActive())
	{
		return;
	}

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
		mEntityStartScale = mEntity->getComponent<G2::TransformComponent>()->getScale();
		mPickOffset = mHandlerStartPoint - mEntityStartPosition;
	}
}

void ScaleHandler::_onManagedEntitySelected(ManagedEntity* entity)
{
	mEntity = entity;
}

void ScaleHandler::handleActivityChanged()
{
	// handle visibility
	mRenderSystem->get(mXAxisHandlerId)->setDrawcallEnabled(isHandleActive());
	mRenderSystem->get(mYAxisHandlerId)->setDrawcallEnabled(isHandleActive());
	mRenderSystem->get(mZAxisHandlerId)->setDrawcallEnabled(isHandleActive());
}

void ScaleHandler::_onMouseUp(G2::MouseButton button, glm::detail::tvec2<int> const& pos)
{
	if (!isHandleActive())
	{
		return;
	}

	// TODO Button should be defined in a button mapping to be configurable!
	if (button == G2::MOUSE_LEFT)
	{
		// reset state to become inactive
		if (mState != SCALE_NOT)
		{
			mState = SCALE_NOT;
			G2S::onScaleHandleReleased();
		}
	}
}

void ScaleHandler::_onMouseMove(glm::detail::tvec2<int> const& pos)
{
	if (!isHandleActive())
	{
		return;
	}

	if (mState != SCALE_NOT)
	{
		// intersect the mouse ray with the XZ-plane on the Y value of the 

		auto* camera = mCameraSystem->getRenderCamera();
		auto* transform = mTransformSystem->get(camera->getEntityId());

		G2::Ray mouseRay = G2::Ray::createScreenProjectionRay(
			pos.x,
			pos.y,
			transform->getWorldSpaceMatrix(),
			camera->getProjectionMatrix(),
			glm::detail::tvec4<int>(0, 0, camera->getViewportWidth(), camera->getViewportHeight()));

		bool intersection;
		glm::vec3 ptOnPlane = _getPlaneIntersection(mouseRay, &intersection);
		if (intersection)
		{
			glm::vec3 scale = mEntityStartScale;

			if (mState == SCALE_X_AXIS)
			{
				float dist = (ptOnPlane.x - mEntityStartPosition.x);
				if (dist != 0.f)
				{
					// the distance from the pick position to the origin on the currrent axis is mapped to a scale multiplication of 1.0
					float initDist = mPickOffset.x;
					float localScale = 1.f / (initDist / dist);
					scale.x *= localScale;
				}
			}
			else if (mState == SCALE_Y_AXIS)
			{
				float dist = (ptOnPlane.y - mEntityStartPosition.y);
				if (dist != 0.f)
				{
					// the distance from the pick position to the origin on the currrent axis is mapped to a scale multiplication of 1.0
					float initDist = mPickOffset.y;
					float localScale = 1.f / (initDist / dist);
					scale.y *= localScale;
				}
			}
			else if (mState == SCALE_Z_AXIS)
			{
				float dist = (ptOnPlane.z - mEntityStartPosition.z);
				if (dist != 0.f)
				{
					// the distance from the pick position to the origin on the currrent axis is mapped to a scale multiplication of 1.0
					float initDist = mPickOffset.z;
					float localScale = 1.f / (initDist / dist);
					scale.z *= localScale;
				}
			}
			auto* tc = mEntity->addComponent<G2::TransformComponent>();
			tc->setScale(scale);
			G2S::onScaleHandleMoved();
		}
	}
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