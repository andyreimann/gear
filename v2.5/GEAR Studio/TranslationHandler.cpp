#include "TranslationHandler.h"
#include "GEARStudioEvents.h"

#include <G2Core/ECSManager.h>

TranslationHandler::TranslationHandler(unsigned int xAxisHandlerId, unsigned int yAxisHandlerId, unsigned int zAxisHandlerId) :
	mXAxisHandlerId(xAxisHandlerId),
	mYAxisHandlerId(yAxisHandlerId),
	mZAxisHandlerId(zAxisHandlerId),
	mState(TRANSLATE_NOT),
	mEntity(nullptr),
	G2S::HandlerBase(G2S::HandleMode::TRANSLATION_HANDLE)
{
	mRenderSystem = G2::ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>();
	mCameraSystem = G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>();
	mTransformSystem = G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>();

	// register to GEAR events
	G2::EventDistributer::onMouseUp.hook(this, &TranslationHandler::_onMouseUp);
	G2::EventDistributer::onMouseMove.hook(this, &TranslationHandler::_onMouseMove);

	// register to Editor events
	G2S::onEditorHandleSelected.hook(this, &TranslationHandler::_onEditorHandleSelected);
	G2S::onManagedEntitySelected.hook(this, &TranslationHandler::_onManagedEntitySelected);
}

TranslationHandler::~TranslationHandler()
{
	// unhook from GEAR events
	G2::EventDistributer::onMouseUp.unHookAll(this);
	G2::EventDistributer::onMouseMove.unHookAll(this);

	// unhook from Editor events
	G2S::onEditorHandleSelected.unHookAll(this);
	G2S::onManagedEntitySelected.unHookAll(this);
}

void TranslationHandler::_onEditorHandleSelected(unsigned int entityId, G2::Intersection const& intersection)
{
	if (!isHandleActive())
	{
		return;
	}

	mTranslationPlaneOffset = 0.f;
	if (entityId == mXAxisHandlerId)
	{
		mState = TRANSLATE_X_AXIS;
		mTranslationPlaneNormal = glm::vec3(0.f, -1.f, 0.f);
		mEntity->addComponent<G2::TransformComponent>();
		mTranslationPlaneOffset = mEntity->getComponent<G2::TransformComponent>()->getPosition().y;
	}
	else if (entityId == mYAxisHandlerId)
	{
		mState = TRANSLATE_Y_AXIS;
		mTranslationPlaneNormal = glm::vec3(0.f, 0.f, -1.f);
		mEntity->addComponent<G2::TransformComponent>();
		mTranslationPlaneOffset = mEntity->getComponent<G2::TransformComponent>()->getPosition().z;
	}
	else if (entityId == mZAxisHandlerId)
	{
		mState = TRANSLATE_Z_AXIS;
		mTranslationPlaneNormal = glm::vec3(0.f, -1.f, 0.f);
		mEntity->addComponent<G2::TransformComponent>();
		mTranslationPlaneOffset = mEntity->getComponent<G2::TransformComponent>()->getPosition().y;
	}

	if (mState != TRANSLATE_NOT)
	{
		// save start state
		mHandlerStartPoint = intersection.getPoint();
		mEntityStartPosition = mEntity->getComponent<G2::TransformComponent>()->getPosition();
		mPickOffset = mHandlerStartPoint - mEntityStartPosition;
	}
}

void
TranslationHandler::_onManagedEntitySelected(ManagedEntity* entity)
{
	mEntity = entity;
}

void TranslationHandler::handleActivityChanged()
{
	// handle visibility
	mRenderSystem->get(mXAxisHandlerId)->setDrawcallEnabled(isHandleActive());
	mRenderSystem->get(mYAxisHandlerId)->setDrawcallEnabled(isHandleActive());
	mRenderSystem->get(mZAxisHandlerId)->setDrawcallEnabled(isHandleActive());
}

void TranslationHandler::_onMouseUp(G2::MouseButton button, glm::detail::tvec2<int> const& pos)
{
	if (!isHandleActive())
	{
		return;
	}
	// TODO Button should be defined in a button mapping to be configurable!
	if (button == G2::MOUSE_LEFT)
	{
		// reset state to become inactive
		if (mState != TRANSLATE_NOT)
		{
			mState = TRANSLATE_NOT;
			G2S::onTranslationHandleReleased();
		}
	}
}

void TranslationHandler::_onMouseMove(glm::detail::tvec2<int> const& pos)
{
	if (!isHandleActive())
	{
		return;
	}

	if (mState != TRANSLATE_NOT)
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
			auto* tc = mEntity->addComponent<G2::TransformComponent>();

			glm::vec3 pos = tc->getPosition();
			glm::vec3 parentsScale = tc->getParentsScale();

			if (mState == TRANSLATE_X_AXIS)
			{
				if (parentsScale.x == 0.f)
				{
					// div by 0 prevention
					parentsScale.x = 1.f;
				}
				pos.x = (ptOnPlane.x - mPickOffset.x) / parentsScale.x;
			}
			else if (mState == TRANSLATE_Y_AXIS)
			{
				if (parentsScale.y == 0.f)
				{
					// div by 0 prevention
					parentsScale.y = 1.f;
				}
				pos.y = (ptOnPlane.y - mPickOffset.y) / parentsScale.y;
			}
			else if (mState == TRANSLATE_Z_AXIS)
			{
				if (parentsScale.z == 0.f)
				{
					// div by 0 prevention
					parentsScale.z = 1.f;
				}
				pos.z = (ptOnPlane.z - mPickOffset.z) / parentsScale.z;
			}
			tc->setPosition(pos);
			G2S::onTranslationHandleMoved();
		}
	}
}

glm::vec3
TranslationHandler::_getPlaneIntersection(G2::Ray const& ray, bool* result) const
{
	glm::vec3 const& normal = mTranslationPlaneNormal;
	float const& offset = mTranslationPlaneOffset;
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