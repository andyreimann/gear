#include "RotationHandler.h"
#include "GEARStudioEvents.h"
#include "EditorGeometryManager.h"

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
	if (!isHandleActive() || mEntity == nullptr)
	{
		return;
	}

	mRotationPlaneOffset = 0.f;
	if (entityId == mXAxisHandlerId)
	{
		mState = ROTATION_X_AXIS;
		mEntity->addComponent<G2::TransformComponent>();
		mRotationPlaneNormal = glm::vec3(-1.f, 0.f, 0.f);
		mRotationPlaneOffset = mEntity->getComponent<G2::TransformComponent>()->getPosition().x;
	}
	else if (entityId == mYAxisHandlerId)
	{
		mState = ROTATION_Y_AXIS;
		mEntity->addComponent<G2::TransformComponent>();
		mRotationPlaneNormal = glm::vec3(0.f,-1.f, 0.f);
		mRotationPlaneOffset = mEntity->getComponent<G2::TransformComponent>()->getPosition().y;
	}
	else if (entityId == mZAxisHandlerId)
	{
		mState = ROTATION_Z_AXIS;
		mEntity->addComponent<G2::TransformComponent>();
		mRotationPlaneNormal = glm::vec3(0.f, 0.f,-1.f);
		mRotationPlaneOffset = mEntity->getComponent<G2::TransformComponent>()->getPosition().z;
	}

	if (mState != ROTATION_NOT)
	{
		// save start state
		mHandlerStartPoint = intersection.getPoint();

		auto* camera = mCameraSystem->getRenderCamera();
		auto* transform = mTransformSystem->get(camera->getEntityId());

		G2::Ray mouseRay = G2::Ray::createScreenProjectionRay(
			mMousePos.x,
			mMousePos.y,
			transform->getWorldSpaceMatrix(),
			camera->getProjectionMatrix(),
			glm::detail::tvec4<int>(0, 0, camera->getViewportWidth(), camera->getViewportHeight()));

		// get the real picking point on the plane
		bool res;
		mHandlerStartPoint = _getPlaneIntersection(mouseRay, &res);


		mEntityStartPosition = mEntity->getComponent<G2::TransformComponent>()->getPosition();
		mEntityStartRotation = mEntity->getComponent<G2::TransformComponent>()->getRotation();
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
	if (!isHandleActive())
	{
		return;
	}

	// TODO Button should be defined in a button mapping to be configurable!
	if (button == G2::MOUSE_LEFT)
	{
		EditorGeometryManager::clearGlobalGeometry("RotationHandler");
		// reset state to become inactive
		if (mState != ROTATION_NOT)
		{
			mState = ROTATION_NOT;
			G2S::onRotationHandleReleased();
		}
	}
}

void RotationHandler::_onMouseMove(glm::detail::tvec2<int> const& pos)
{
	mMousePos = pos;
	if (!isHandleActive())
	{
		return;
	}

	if (mState != ROTATION_NOT)
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

			// DEBUG
			EditorGeometryManager::clearGlobalGeometry("RotationHandler");

			glm::vec3 geometry[2];
			// DEBUG END

			glm::vec3 pickDir = glm::normalize(mPickOffset);
			glm::vec3 currentDir;
			auto* tc = mEntity->addComponent<G2::TransformComponent>();
			tc->setRotation(mEntityStartRotation);

			float degrees = 0.f;
			float sign = 1.f;
			glm::vec3 axis(0.f, 1.f, 0.f);
			if (mState == ROTATION_X_AXIS)
			{
				pickDir = glm::normalize(mPickOffset);
				currentDir = glm::normalize(ptOnPlane - mEntityStartPosition);
				float dot = glm::dot(pickDir, currentDir);
				sign = (glm::dot(mRotationPlaneNormal, glm::cross(pickDir, currentDir)) >= 0.f) ? -1.f : 1.f;
				degrees = std::acos(dot) * 180.f / 3.14f;
				axis = glm::vec3(1.f, 0.f, 0.f);
				//G2S::onLog(INFO, "X sign=" + std::to_string(sign) + "Degrees=" + std::to_string(degrees) + ", PickDir[" + std::to_string(pickDir.x) + "," + std::to_string(pickDir.y) + "," + std::to_string(pickDir.z) + "]" + ", CurrentDir[" + std::to_string(currentDir.x) + "," + std::to_string(currentDir.y) + "," + std::to_string(currentDir.z) + "]");
			}
			else if (mState == ROTATION_Y_AXIS)
			{
				pickDir = glm::normalize(mPickOffset);
				currentDir = glm::normalize(ptOnPlane - mEntityStartPosition);
				float dot = glm::dot(pickDir, currentDir);
				sign = (glm::dot(mRotationPlaneNormal, glm::cross(pickDir, currentDir)) >= 0.f) ? -1.f : 1.f;
				degrees = std::acos(dot) * 180.f / 3.14f;
				axis = glm::vec3(0.f, 1.f, 0.f);
				//G2S::onLog(INFO, "Y sign=" + std::to_string(sign) + "Degrees=" + std::to_string(degrees) + ", PickDir[" + std::to_string(pickDir.x) + "," + std::to_string(pickDir.y) + "," + std::to_string(pickDir.z) + "]" + ", CurrentDir[" + std::to_string(currentDir.x) + "," + std::to_string(currentDir.y) + "," + std::to_string(currentDir.z) + "]");
			}
			else if (mState == ROTATION_Z_AXIS)
			{
				pickDir = glm::normalize(mPickOffset);
				currentDir = glm::normalize(ptOnPlane - mEntityStartPosition);
				float dot = glm::dot(pickDir, currentDir);
				sign = (glm::dot(mRotationPlaneNormal, glm::cross(pickDir, currentDir)) >= 0.f) ? -1.f : 1.f;
				degrees = std::acos(dot) * 180.f / 3.14f;
				axis = glm::vec3(0.f, 0.f, 1.f);
				//G2S::onLog(INFO, "Z sign=" + std::to_string(sign) + "Degrees=" + std::to_string(degrees) + ", PickDir[" + std::to_string(pickDir.x) + "," + std::to_string(pickDir.y) + "," + std::to_string(pickDir.z) + "]" + ", CurrentDir[" + std::to_string(currentDir.x) + "," + std::to_string(currentDir.y) + "," + std::to_string(currentDir.z) + "]");
			}
			tc->setRotation(glm::normalize(glm::cross(glm::angleAxis(degrees * sign, axis), mEntityStartRotation)));
			G2S::onRotationHandleMoved();

			// DEBUG
			// current dir - blue
			//geometry[0] = mEntityStartPosition;
			//geometry[1] = mEntityStartPosition + (1.f * currentDir);
			//EditorGeometryManager::addGlobalGeometry("RotationHandler", geometry, 2, glm::vec4(0.f, 0.f, 1.f, 1.f));

			//// pick dir - red
			//geometry[0] = mEntityStartPosition;
			//geometry[1] = mEntityStartPosition + (1.f * glm::normalize(mPickOffset));
			//EditorGeometryManager::addGlobalGeometry("RotationHandler", geometry, 2, glm::vec4(1.f, 0.f, 0.f, 1.f));

			// plane normal - green
			// geometry[0] = mEntityStartPosition;
			// geometry[1] = mEntityStartPosition + (3.f * mRotationPlaneNormal);
			// EditorGeometryManager::addGlobalGeometry("RotationHandler", geometry, 2, glm::vec4(0.f, 1.f, 0.f, 1.f));
			// DEBUG END
		}
	}
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