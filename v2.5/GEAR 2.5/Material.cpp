// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Material.h"
#include "RenderComponent.h"

#include <G2Core/Entity.h>
#include <G2Core/ECSManager.h>
#include <limits>

using namespace G2;

Material::Material() 
	: mIsTransparent(false),
	mEntityId(Entity::UNINITIALIZED_ENTITY_ID)
{
	mMaterialData.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	mMaterialData.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterialData.specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	mMaterialData.shininess = 0.f;
}

Material::Material(Material && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

Material& Material::operator=(Material && rhs) 
{
	
	// 1. Stage: delete maybe allocated resources on target type
	// nothing here
	// 2. Stage: transfer data from src to target
	mEntityId = rhs.mEntityId;
	mIsTransparent = rhs.mIsTransparent;
	mMaterialData = std::move(rhs.mMaterialData);
	// copy and increment the version!
	setVersion(rhs.getVersion()+1);
	rhs.mEntityId = Entity::UNINITIALIZED_ENTITY_ID;
	return *this;
}

bool
Material::isTransparent() const
{
	return mIsTransparent;
}

Material&
Material::setShininess(float const& shininess)
{
	mMaterialData.shininess = shininess;
	updateVersion();
	return *this;
}

Material&
Material::setSpecular(glm::vec4 const& specular)
{
	mMaterialData.specular = specular;
	_updateTransparencyFlag();
	updateVersion();
	return *this;
}

Material&
Material::setDiffuse(glm::vec4 const& diffuse) {
	mMaterialData.diffuse = diffuse;
	_updateTransparencyFlag();
	updateVersion();
	return *this;
}

Material&
Material::setAmbient(glm::vec4 const& ambient)
{
	mMaterialData.ambient = ambient;
	_updateTransparencyFlag();
	updateVersion();
	return *this;
}

void
Material::_updateTransparencyFlag()
{
	float threshold = 1.f - std::numeric_limits<int>::epsilon();
	if (mMaterialData.ambient.w < threshold || mMaterialData.diffuse.w < threshold || mMaterialData.specular.w < threshold)
	{
		if(mIsTransparent != true)
		{
			mIsTransparent = true;
			// newly transparent
			ECSManager::getShared().getSystem<RenderSystem,RenderComponent>()->updateTransparencyMode(mEntityId, true);
		}
		return;
	}
	if(mIsTransparent == true)
	{
		// newly solid
		mIsTransparent = false;
		ECSManager::getShared().getSystem<RenderSystem,RenderComponent>()->updateTransparencyMode(mEntityId, false);
	}
}

Material&
Material::setAmbientAndDiffuse(glm::vec4 const& color) 
{
	mMaterialData.ambient = color;
	mMaterialData.diffuse = color;
	_updateTransparencyFlag();
	updateVersion();
	return *this;
}

bool
Material::operator==(Material const& rhs) 
{
	return  mMaterialData.ambient == rhs.getAmbient() &&
		mMaterialData.diffuse == rhs.getDiffuse() &&
		mMaterialData.specular == rhs.getSpecular() &&
		mMaterialData.shininess == rhs.getShininess();
}

void
Material::_connectToEntityId(unsigned int entityId) 
{
	mEntityId = entityId;
	ECSManager::getShared().getSystem<RenderSystem,RenderComponent>()->updateTransparencyMode(mEntityId, mIsTransparent);
}