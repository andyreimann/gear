// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Sampler.h"

using namespace G2;

Sampler::Name
Sampler::getSampler(std::string const& name) 
{
	if(name == "DIFFUSE") 
	{
		return DIFFUSE;
	}
	else if(name == "NORMAL") 
	{
		return NORMAL;
	}
	else if(name == "ALPHA") 
	{
		return ALPHA;
	}
	else if(name == "SHADOW") 
	{
		return SHADOW;
	}
	else if(name == "SPECULAR") 
	{
		return SPECULAR;
	}
	else if(name == "SAMPLER_6") 
	{
		return SAMPLER_6;
	}
	else if(name == "SAMPLER_7") 
	{
		return SAMPLER_7;
	}
	else if(name == "SAMPLER_8") 
	{
		return SAMPLER_8;
	}
	else if(name == "SAMPLER_9") 
	{
		return SAMPLER_9;
	}
	else if(name == "SAMPLER_10") 
	{
		return SAMPLER_10;
	}
	else if(name == "SAMPLER_11") 
	{
		return SAMPLER_11;
	}
	else if(name == "SAMPLER_12") 
	{
		return SAMPLER_12;
	}
	else if(name == "SAMPLER_13") 
	{
		return SAMPLER_13;
	}
	else if(name == "SAMPLER_14") 
	{
		return SAMPLER_14;
	}
	else if(name == "SAMPLER_15") 
	{
		return SAMPLER_15;
	}
	else if(name == "SAMPLER_16") 
	{
		return SAMPLER_16;
	}
	return SAMPLER_INVALID;
}