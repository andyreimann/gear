#include "TransformationHandler.h"
#include "GEARStudioEvents.h"

#include <G2Core/ECSManager.h>

TransformationHandler::TransformationHandler(unsigned int transformationAnchorId) :
	mTransformationAnchorId(transformationAnchorId),
	mEntity(nullptr)
{
	mTransformSystem = G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>();

	// register to GEAR events
	G2::EventDistributer::onRenderFrame.hook(this, &TransformationHandler::_onRenderFrame);

	// register to Editor events
	GEARStudioEvents::onManagedEntitySelected.hook(this, &TransformationHandler::_onManagedEntitySelected);
}

TransformationHandler::~TransformationHandler()
{
	// unhook from GEAR events
	G2::EventDistributer::onFrameRendered.unHookAll(this);
	// unhook from Editor events
	GEARStudioEvents::onManagedEntitySelected.unHookAll(this);
}

void
TransformationHandler::_onManagedEntitySelected(ManagedEntity* entity)
{
	mEntity = entity;
	_update();
}

void
TransformationHandler::_onRenderFrame(G2::FrameInfo const& frame)
{
	_update();
}

void
TransformationHandler::_update()
{
	if (mEntity != nullptr)
	{
		// connect handles to selected entity
		auto* anchorTc = mTransformSystem->get(mTransformationAnchorId);
		auto* tc = mEntity->addComponent<G2::TransformComponent>();
		if (anchorTc->getParentEntityId() != mEntity->getId())
		{
			anchorTc->setParent(tc);
		}
		anchorTc->setScale(1.f / (tc->getParentsScale() * tc->getScale())); // negate scale
	}
	else
	{
		// reset connection
		mTransformSystem->get(mTransformationAnchorId)->setParent(nullptr)->setScale(glm::vec3(1.f,1.f,1.f));
	}
}