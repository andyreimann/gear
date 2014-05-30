// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "TransformComponent.h"
#include "Logger.h"

using namespace G2;

// GLM Quaternion doc: http://glm.g-truc.net/0.9.0/api/a00184.html#a2d9d1916258d13bd5f0373341400f18c


G2::TransformComponent::TransformComponent() : 
	mParentEntityId(0),
	mIsDirty(true),
	mPosition(0.f,0.f,0.f),
	mScale(1.f,1.f,1.f),
	mMode(TransformMode::SRT),
	mLastUpdateId(-1),
	mUpdated(false)
{ }

G2::TransformComponent::TransformComponent(TransformMode::Name mode) : 
	mParentEntityId(0),
	mIsDirty(true),
	mPosition(0.f,0.f,0.f),
	mScale(1.f,1.f,1.f),
	mMode(mode),
	mLastUpdateId(-1),
	mUpdated(false)
{ }

TransformComponent::TransformComponent(TransformComponent && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

TransformComponent& TransformComponent::operator=(TransformComponent && rhs) 
{
	mParentEntityId = rhs.mParentEntityId;
	mIsDirty = rhs.mIsDirty;
	mPosition = std::move(rhs.mPosition);
	mScale = std::move(rhs.mScale);
	mRotation = std::move(rhs.mRotation);
	mLocalSpace = std::move(rhs.mLocalSpace);
	mWorldSpace = std::move(rhs.mWorldSpace);
	mChildEntityIds = std::move(rhs.mChildEntityIds);
	mMode = rhs.mMode;
	mLastUpdateId = rhs.mLastUpdateId;
	mUpdated = rhs.mUpdated;
	
	return static_cast<TransformComponent&>(BaseComponent::operator=(std::move(rhs)));
}

TransformComponent*
TransformComponent::translate(glm::vec3 const& translation) 
{
	mPosition += translation; 
	_setDirty();
	return this;
}

TransformComponent*
TransformComponent::setPosition(glm::vec3 const& position) 
{
	mPosition = position; 
	_setDirty();
	return this;
}

glm::vec3 const&
TransformComponent::getPosition() const
{
	return mPosition;
}

TransformComponent*
TransformComponent::rotate(glm::quat const& rotation) 
{
	mRotation = glm::cross(mRotation, rotation);
	_setDirty();
	return this;
}

TransformComponent*
TransformComponent::rotate(glm::mat3 const& rotation) 
{
	mRotation = glm::cross(mRotation, glm::toQuat(rotation));
	_setDirty();
	return this;
}

TransformComponent*
TransformComponent::rotate(glm::mat4 const& rotation) 
{
	mRotation = glm::cross(mRotation, glm::toQuat(rotation));
	_setDirty();
	return this;
}

TransformComponent*
TransformComponent::rotateX(float degrees) 
{
	return rotateAxis(degrees, glm::vec3(1.f,0.f,0.f));
}

TransformComponent*
TransformComponent::rotateY(float degrees) 
{
	return rotateAxis(degrees, glm::vec3(0.f,1.f,0.f));
}

TransformComponent*
TransformComponent::rotateZ(float degrees) 
{
	return rotateAxis(degrees, glm::vec3(0.f,0.f,1.f));
}

TransformComponent*
TransformComponent::rotateAxis(float degrees, glm::vec3 const& axis) 
{
	mRotation = glm::cross(mRotation, glm::angleAxis(degrees, axis)); 
	_setDirty();
	return this;
}

TransformComponent*
TransformComponent::rotateEulerAnglesYXZ(float yaw, float pitch, float roll) 
{
	mRotation = glm::cross(mRotation, glm::toQuat(glm::yawPitchRoll(yaw,pitch,roll))); 
	_setDirty();
	return this;
}

TransformComponent*
TransformComponent::setRotation(glm::quat const& rotation) 
{
	mRotation = rotation; 
	_setDirty();
	return this;
}

TransformComponent*
TransformComponent::setRotation(glm::mat3 const& rotation) 
{
	mRotation = glm::toQuat(rotation); 
	_setDirty();
	return this;
}

TransformComponent*
TransformComponent::setRotation(glm::mat4 const& rotation) 
{
	mRotation = glm::toQuat(rotation); 
	_setDirty();
	return this;
}

glm::quat const&
TransformComponent::getRotation() const
{
	return mRotation;
}

TransformComponent*
TransformComponent::setScale(glm::vec3 const& scale) 
{
	mScale = scale; 
	_setDirty();
	return this;
}

glm::vec3 const&
TransformComponent::getScale() const
{
	return mScale;
}

void
TransformComponent::updateWorldSpaceMatrix(long updateId) 
{
	if(mLastUpdateId == updateId)
	{
		return; // already called this frame
	}

	auto* parentTransformComponent = system->get(mParentEntityId);

	if(!_isDirty())
	{
		mUpdated = false;
	}
	else
	{
		// SRT
		if(mMode == TransformMode::SRT)
		{
			mWorldSpaceOrthogonal = glm::translate(mPosition) * glm::toMat4(mRotation);
			mLocalSpace = mWorldSpace = mWorldSpaceOrthogonal * glm::scale(mScale);
		}
		else
		{
			mWorldSpaceOrthogonal = glm::toMat4(mRotation) * glm::translate(mPosition);
			mLocalSpace = mWorldSpace = glm::scale(mScale) * mWorldSpaceOrthogonal;
		}
		
		auto* parentTransformComponent = system->get(mParentEntityId);
		if(parentTransformComponent != nullptr)
		{
			// TODO: implement component sorting in TransformSystem to not update parents here!
			parentTransformComponent->updateWorldSpaceMatrix(updateId);
			mWorldSpace = parentTransformComponent->getWorldSpaceMatrix() * mLocalSpace;
		}
		mIsDirty = false;
		mUpdated = true;
	}
	mLastUpdateId = updateId;
}

glm::mat4 const&
TransformComponent::getLocalSpaceMatrix() const
{
	// do not check update here - done in TransformSystem postUpdate step
	return mLocalSpace;
}

glm::mat4 const&
TransformComponent::getWorldSpaceMatrix() const
{
	// do not check update here - done in TransformSystem postUpdate step
	return mWorldSpace;
}

TransformComponent*
TransformComponent::setParent(TransformComponent* parent) 
{
	if(parent != nullptr && parent != this)
	{
		if(parent->_hasCircularDependency(getEntityId()))
		{
			// parent dependency chain contains this TransformComponent
			logger << "[TransformComponent] Warning: Circular dependency detected while trying to set the parent TransformComponent for TransformComponent attached on Entity " << getEntityId() << "!" << endl;
			return this;
		}
		mParentEntityId = parent->getEntityId();
		parent->mChildEntityIds.push_back(getEntityId());
		_setDirty();
	}
	return this;
}

unsigned int
TransformComponent::getParentEntityId() const
{
	return mParentEntityId;
}

bool
TransformComponent::_isDirty() const 
{
	return mIsDirty;
}

void
TransformComponent::_setDirty()
{
	mIsDirty = true;
	for (int i = 0; i < mChildEntityIds.size() ; ++i) 
	{
		auto* tc = system->get(mChildEntityIds[i]);
		if(tc != nullptr)
		{
			tc->_setDirty();
		}
	}
}

bool
TransformComponent::_hasCircularDependency(unsigned int initialParentEntityId) const 
{
	if(mParentEntityId == initialParentEntityId)
	{
		// circular dependency detected
		return true;
	}
	auto* parent = system->get(mParentEntityId);
	if(parent != nullptr) 
	{
		return parent->_hasCircularDependency(initialParentEntityId);
	}

	return false;
}
