// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Shader.h"
#include "UniformBufferObject.h"

using namespace G2;

#include "RenderComponent.h"
#include <G2Core/GfxDevice.h>
#include <G2Core/ECSManager.h>

Shader::Shader() 
{
}

Shader::~Shader() 
{
	G2_gfxDevice()->freeGfxResource(mGfxHandle);
}

void
Shader::setConditions(std::vector<MacroCondition> const& conditions) 
{
	mConditions = conditions;
}

void
Shader::initWithMetaData(ShaderMetaData const& metaData) 
{
	bind();
	for (int i = 0; i < metaData.samplers.size() ; ++i) 
	{
		ShaderMetaData::SamplerMetaData const& samplerMetaData = metaData.samplers[i];
		if(samplerMetaData.samplerSlot != Sampler::SAMPLER_INVALID)
		{
			// BIND SAMPLER
			setProperty(std::move(samplerMetaData.name),samplerMetaData.samplerSlot);
		}
	}

	// connect this Shader to all existing default UniformBufferObjects
	// coming from the default includes GEAR provides
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem, G2::RenderComponent>()
		->_connectShaderToUniformBlocks(this);
}



void
Shader::bind() 
{
	G2_gfxDevice()->bindShader(mGfxHandle);
}

void
Shader::setProperty(std::string const& property, glm::mat4 const& value) 
{
	G2_gfxDevice()->setShaderUniformMat4(mGfxHandle,property,value);
}

void
Shader::setProperty(std::string const& property, glm::mat3 const& value) 
{
	G2_gfxDevice()->setShaderUniformMat3(mGfxHandle,property,value);
}

void
Shader::setProperty(std::string const& property, glm::vec4 const& value) 
{
	G2_gfxDevice()->setShaderUniformVec4(mGfxHandle,property,value);
}

void
Shader::setProperty(std::string const& property, glm::vec3 const& value) 
{
	G2_gfxDevice()->setShaderUniformVec3(mGfxHandle,property,value);
}

void
Shader::setProperty(std::string const& property, glm::vec2 const& value) 
{
	G2_gfxDevice()->setShaderUniformVec2(mGfxHandle,property,value);
}

void
Shader::setProperty(std::string const& property, float value) 
{
	G2_gfxDevice()->setShaderUniformFloat(mGfxHandle,property,value);
}

void
Shader::setProperty(std::string const& property, int value) 
{
	G2_gfxDevice()->setShaderUniformInt(mGfxHandle,property,value);
}

bool
Shader::compile(std::string const& shadingLanguage, std::string const& vertexCode, std::string const& geometryCode, std::string const& fragmentCode, G2Core::VertexInputLayout const& vertexInputLayout) 
{
	mVertexInputLayout = vertexInputLayout;
	mGfxHandle = G2_gfxDevice()->compileShader(vertexInputLayout,shadingLanguage,vertexCode,geometryCode,fragmentCode);
	return mGfxHandle->valid;
}

void
G2::Shader::_setUBOBlockBinding(std::string const& blockName, UniformBufferObject* ubo)
{
	G2_gfxDevice()->setShaderUBOBlockBinding(mGfxHandle, ubo->mUniformBufferResource, blockName);
}