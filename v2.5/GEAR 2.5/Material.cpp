// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Material.h"
#include <limits>

using namespace G2;

Material::Material() 
	: mAmbient(0.2f,0.2f,0.2f,1.0f),
	mDiffuse(1.0f,1.0f,1.0f,1.0f),
	mSpecular(0.0f,0.0f,0.0f,1.0f),
	mShininess(1.0f),
	mIsTransparent(false) 
{
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
	mIsTransparent = rhs.mIsTransparent;
	mAmbient = std::move(rhs.mAmbient);
	mDiffuse = std::move(rhs.mDiffuse);
	mSpecular = std::move(rhs.mSpecular);
	mTextures = std::move(rhs.mTextures);
	mShininess = rhs.mShininess;
	// copy and increment the version!
	setVersion(rhs.getVersion()+1);
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
	mShininess = shininess;
	updateVersion();
	return *this;
}

Material&
Material::setSpecular(glm::vec4 const& specular)
{
	mSpecular = specular;
	updateTransparencyFlag();
	updateVersion();
	return *this;
}

Material&
Material::setDiffuse(glm::vec4 const& diffuse) {
	mDiffuse = diffuse;
	updateTransparencyFlag();
	updateVersion();
	return *this;
}

Material&
Material::setAmbient(glm::vec4 const& ambient)
{
	mAmbient = ambient;
	updateTransparencyFlag();
	updateVersion();
	return *this;
}

void
Material::updateTransparencyFlag()
{
	float threshold = 1.f - std::numeric_limits<int>::epsilon();
	if(mAmbient.w < threshold || mDiffuse.w < threshold || mSpecular.w < threshold)
	{
		mIsTransparent = true;
		return;
	}
	mIsTransparent = false;
}

Material&
Material::setAmbientAndDiffuse(glm::vec4 const& color) 
{
	mAmbient = color;
	mDiffuse = color;
	updateTransparencyFlag();
	updateVersion();
	return *this;
}

bool
Material::operator==(Material const& rhs) 
{
	return  mAmbient == rhs.getAmbient() &&
			mDiffuse == rhs.getDiffuse() &&
			mSpecular == rhs.getSpecular() &&
			mShininess == rhs.getShininess();
}