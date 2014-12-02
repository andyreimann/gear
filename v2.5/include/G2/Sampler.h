// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"

namespace G2 
{
	/** Samplers are used to keep a Texture in sync with a sampler Property of a Shader.
	 * Inside of a Shader, you define a sampler Property for a given Sampler by giving it
	 * the appropriate default value. The engine then keeps the Textures from Materials
	 * in sync with the Properties and bind the Textures to the requested Property Samplers
	 * at runtime, if the Material provides a Texture for a Sampler.
	 */
	namespace Sampler
	{
		enum Name {
			DIFFUSE = 0,
			NORMAL = 1,
			ALPHA = 3,
			SHADOW = 4,
			SPECULAR = 5,
			SAMPLER_6 = 6,
			SAMPLER_7 = 7,
			SAMPLER_8 = 8,
			SAMPLER_9 = 9,
			SAMPLER_10 = 10,
			SAMPLER_11 = 11,
			SAMPLER_12 = 12,
			SAMPLER_13 = 13,
			SAMPLER_14 = 14,
			SAMPLER_15 = 15,
			SAMPLER_16 = 16,
			NUM_SAMPLERS,
			SAMPLER_INVALID,
		};
		/** This function will parse the given string to the appropriate
		 * Sampler enum value.
		 * @param name The name to parse.
		 * @return The parsed Sampler enum value.
		 */
		Sampler::Name getSampler(std::string const& name);
	};
};