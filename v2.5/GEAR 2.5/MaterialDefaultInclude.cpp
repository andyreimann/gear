// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "MaterialDefaultInclude.h"
#include <math.h>

using namespace G2;

MaterialDefaultInclude::MaterialDefaultInclude(std::string const& shadingLanguage)
{
	if(shadingLanguage == "GLSL") 
	{
		const char* code =
		{
			"struct G2Material {\n"\
			"	vec4 ambient; // The ambientmaterial term\n"\
			"	vec4 diffuse; // The diffuse material term\n"\
			"	vec4 specular; // The specular material term\n"\
			"	float shininess; // The shininess factor for specular highlights\n"\
			"};\n"\
			"uniform G2Material material;\n"\
		};
		mPart = std::string(code);
	}
	else if(shadingLanguage == "CG") 
	{
		const char* code =
		{
			"struct G2Material {\n"\
			"	float4 ambient; // The ambientmaterial term\n"\
			"	float4 diffuse; // The diffuse material term\n"\
			"	float4 specular; // The specular material term\n"\
			"	float shininess; // The shininess factor for specular highlights\n"\
			"};\n"\
			"uniform G2Material material;\n"\
		};
		mPart = std::string(code);
	}
}