// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Sampler.h"

#include <string>
#include <vector>

namespace G2 
{
	/** This struct holds meta data for a shader it is assigned to.
	 * @created:	2014/02/12
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	struct ShaderMetaData 
	{
		ShaderMetaData() 
		{
			samplers.resize(Sampler::NUM_SAMPLERS);
		}
		struct SamplerMetaData
		{
			SamplerMetaData() : samplerSlot(Sampler::SAMPLER_INVALID) {}
			Sampler::Name	samplerSlot;
			std::string		name;		// The name of the meta data
		};
		std::vector<SamplerMetaData> samplers; // The sampler names inside the shader
	};
};