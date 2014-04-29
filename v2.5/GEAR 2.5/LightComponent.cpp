#include "LightComponent.h"

using namespace G2;

LightComponent::LightComponent(LightType::Name type)
	: mType(type),
	ambient(1.f,1.f,1.f,1.f),
	diffuse(1.f,1.f,1.f,1.f),
	specular(1.f,1.f,1.f,1.f),
	mEnabled(true),
	cutOffDegrees(0.f),
	attenuation(1.f),
	linearAttenuation(0.f),
	exponentialAttenuation(0.f)
{
	if(type == LightType::POSITIONAL)
	{
		mDefaultPosition = mCachedPosition = glm::vec4(0.f,0.f,0.f,1.f);
	}
	else if(type == LightType::DIRECTIONAL)
	{
		// final light dir=-Y!
		mDefaultPosition = mCachedPosition = glm::vec4(0.f,1.f,0.f,0.f);
	}
}

LightComponent&
LightComponent::setupColor(glm::vec4 const& ambient, glm::vec4 const& diffuse, glm::vec4 const& specular) 
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	return *this;
}

void
LightComponent::_updateTransformedPosition(glm::vec4 const& pos) 
{
	mCachedPosition = pos;
	if(mType == LightType::DIRECTIONAL)
	{
		mCachedPosition = glm::normalize(mCachedPosition);
	}
}