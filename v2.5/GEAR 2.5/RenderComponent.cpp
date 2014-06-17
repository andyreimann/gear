#include "RenderComponent.h"

using namespace G2;

RenderComponent::RenderComponent() 
	: billboarding(false),
	drawMode(GL_TRIANGLES),
	aabbAnimationRecalc(false)
{
}

RenderComponent::RenderComponent(RenderComponent && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

RenderComponent&
RenderComponent::operator=(RenderComponent && rhs) 
{
	material = std::move(rhs.material);
	mVaos = std::move(rhs.mVaos);
	mVaosFrustumCulled = std::move(rhs.mVaosFrustumCulled);
	drawMode = rhs.drawMode;
	billboarding = rhs.billboarding;
	objectSpaceAABBs = std::move(rhs.objectSpaceAABBs);
	worldSpaceAABBs = std::move(rhs.worldSpaceAABBs);
	aabbAnimationRecalc = rhs.aabbAnimationRecalc;
	mEffect = std::move(rhs.mEffect);
	mShaderCache = std::move(rhs.mShaderCache);
	
	rhs.mVaos.clear();
	rhs.mVaosFrustumCulled.clear();
	rhs.objectSpaceAABBs.clear();
	rhs.drawMode = GL_INVALID_VALUE;
	
	return static_cast<RenderComponent&>(BaseComponent::operator=(std::move(rhs)));
}

G2::RenderComponent::~RenderComponent() 
{
	if(material.isTransparent() && getEntityId() != Entity::UNINITIALIZED_ENTITY_ID)
	{
		auto* renderSystem = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>();
		ECSManager::getShared().getSystem<RenderSystem,RenderComponent>()->updateTransparencyMode(getEntityId(), false);
	}
}

void
RenderComponent::_updateRenderStatesGroupLinkage(std::shared_ptr<RenderStatesGroup> newGroup) 
{
	if(mRenderStatesGroup.get() != nullptr)
	{
		// remove from group
		mRenderStatesGroup->erase(getEntityId());
	}
	mRenderStatesGroup = newGroup;
}
void
RenderComponent::setEffect(std::shared_ptr<G2::Effect> const& value) 
{
	mEffect = value; 
	mShaderCache = ShaderCache(); // invalidate
}

void
RenderComponent::allocateVertexArrays(unsigned int numVertexArrayObjects) 
{
	int sizeDifference = (int)numVertexArrayObjects - (int)mVaos.size();
	mVaos.resize(numVertexArrayObjects);
	mVaosFrustumCulled.resize(numVertexArrayObjects);
	auto* renderSystem = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>();
	renderSystem->scheduleAABBRecalculation(getEntityId());
	renderSystem->_onVertexArrayObjectsResize(getEntityId(),sizeDifference);
	material._connectToEntityId(getEntityId());
}