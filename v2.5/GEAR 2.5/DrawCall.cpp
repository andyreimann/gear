#include "DrawCall.h"
#include "Defines.h"
#include "RenderComponent.h"

#include <G2Core/Entity.h>
#include <G2Core/ECSManager.h>
#include <utility>

using namespace G2;

DrawCall::DrawCall()
	: mAABBCalculationMode(AABBCalculationMode::AUTOMATIC),
	mLastAutoRecalcFrame(-1),
	mEnabled(true),
	mDrawCallIndex(-1),
	mVaoIndex(-1),
	mIaoIndex(-1),
	mEntityId(Entity::UNINITIALIZED_ENTITY_ID),
	mDrawMode(G2Core::DrawMode::TRIANGLES),
	mWasFrustumCulled(false)
{
}

DrawCall::DrawCall(DrawCall const& rhs) 
{
	*this = rhs;
}

DrawCall::DrawCall(DrawCall && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

DrawCall&
DrawCall::operator=(DrawCall const& rhs) 
{
	// do assignment here
	mAABBCalculationMode = rhs.mAABBCalculationMode;
	mModelSpaceAABB = rhs.mModelSpaceAABB;
	mWorldSpaceAABB = rhs.mWorldSpaceAABB;
	mLastAutoRecalcFrame = rhs.mLastAutoRecalcFrame;
	mEnabled = rhs.mEnabled;
	mDrawCallIndex = rhs.mDrawCallIndex;
	mVaoIndex = rhs.mVaoIndex;
	mIaoIndex = rhs.mIaoIndex;
	mDrawMode = rhs.mDrawMode;
	mWasFrustumCulled = rhs.mWasFrustumCulled;
	// mEntityId is not modified since it is managed by the RenderComponent!
	return *this;
}

DrawCall&
DrawCall::operator=(DrawCall && rhs) 
{
	// 1. Stage: delete maybe allocated resources on target type
	// 2. Stage: transfer data from src to target
	mAABBCalculationMode = rhs.mAABBCalculationMode;
	mModelSpaceAABB = std::move(rhs.mModelSpaceAABB);
	mWorldSpaceAABB = std::move(rhs.mWorldSpaceAABB);
	mLastAutoRecalcFrame = rhs.mLastAutoRecalcFrame;
	mEnabled = rhs.mEnabled;
	mDrawCallIndex = rhs.mDrawCallIndex;
	mVaoIndex = rhs.mVaoIndex;
	mIaoIndex = rhs.mIaoIndex;
	mEntityId = rhs.mEntityId;
	mDrawMode = rhs.mDrawMode;
	mWasFrustumCulled = rhs.mWasFrustumCulled;
	// 3. Stage: modify src to a well defined state
	rhs.mEntityId = Entity::UNINITIALIZED_ENTITY_ID;
	rhs.mVaoIndex = -1;
	rhs.mIaoIndex = -1;
	rhs.mDrawMode = G2Core::DrawMode::TRIANGLES;
	return *this;
}

DrawCall&
DrawCall::setAABBCalculationMode(AABBCalculationMode mode) 
{
	if(mode != mAABBCalculationMode)
	{
		mAABBCalculationMode = mode;
		if(mode != MANUAL && mEntityId != Entity::UNINITIALIZED_ENTITY_ID)
		{
			// is linked - inform RenderComponent to schedule AABB recalc
			auto* renderSystem = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>();
			auto* renderComponent = renderSystem->get(getEntityId());
			if(renderComponent != nullptr)
			{
				renderComponent->scheduleAABBRecalculation();
			}
		}
	}

	return *this;
}