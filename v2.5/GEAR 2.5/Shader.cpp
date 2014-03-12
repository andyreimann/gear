// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Shader.h"

using namespace G2;

Shader::Shader() 
{
}

Shader::~Shader() 
{
}

void
Shader::setConditions(std::vector<MacroCondition> const& conditions) 
{
	mConditions = conditions;
}

void
Shader::initWithMetaData(ShaderMetaData const& metaData) 
{
	for (int i = 0; i < metaData.samplers.size() ; ++i) 
	{
		ShaderMetaData::SamplerMetaData const& samplerMetaData = metaData.samplers[i];
		bind();
		if(samplerMetaData.samplerSlot != Sampler::SAMPLER_INVALID)
		{
			// BIND SAMPLER
			setProperty(Property(samplerMetaData.name),samplerMetaData.samplerSlot);
		}
	}
}