// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "MacroResource.h"

using namespace G2;


MacroResource::Name
MacroResource::toMacroResource(std::string const& resource) 
{
	if(resource == "LIGHTS")
	{
		return LIGHTS;
	}
	else if(resource == "QUALITY")
	{
		return QUALITY;
	}
	else if(resource == "HAS_SHADOWS")
	{
		return HAS_SHADOWS;
	}
	else if(resource == "HAS_DIFFUSE_TEX")
	{
		return HAS_DIFFUSE_TEX;
	}
	else if(resource == "HAS_NORMAL_MAP")
	{
		return HAS_NORMAL_MAP;
	}
	else if(resource == "HAS_BUMP_MAP")
	{
		return HAS_BUMP_MAP;
	}
	return INVALID_RESOURCE;
}