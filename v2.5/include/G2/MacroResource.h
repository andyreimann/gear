// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include <string>

namespace G2 
{
	namespace MacroResource 
	{
		enum Name 
		{
			INVALID_RESOURCE = 0,
			LIGHTS,
			QUALITY,
			HAS_SHADOWS,
			HAS_DIFFUSE_TEX,
			HAS_NORMAL_MAP,
			HAS_BUMP_MAP,
			NUM_RESOURCES,
		};

		Name toMacroResource(std::string const& resource);
	};
};