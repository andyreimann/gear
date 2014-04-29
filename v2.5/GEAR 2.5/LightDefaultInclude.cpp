// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "LightDefaultInclude.h"
#include <math.h>

using namespace G2;

LightDefaultInclude::LightDefaultInclude(ShadingLanguage::Name shadingLanguage)
{
	if(shadingLanguage == ShadingLanguage::GLSL) 
	{
		const char* code =
		{
			"struct G2LightInfo {\n"\
			"	// coloring\n"\
			"	vec4 color;         // The color of the light\n"\
			"	vec4 attenuation;   // [x=constant,y=linear,z=quadric]\n"\
			"	// positional\n"\
			"	vec4 position;      // Local position - if(position.w == 0.0) -> directional light, else -> point or spot light\n"\
			"	vec3 direction;     // Light direction\n"\
			"	float range;        // The distance or the range of the light cone\n"\
			/*"	mat4 light2Local;   // converts light position to local position\n"\
			"	mat4 light2World;   // converts light position to world position\n"\
			"	mat4 world2Light;   // converts world position to light position\n"\
			"	mat4 local2Light;   // converts local position to light position\n"*/\
			"	// special\n"\
			"	float cosCutoff;    // if(cosCutoff >=0 ) spotlight else point or directional light\n"\
			"};\n"\
			"uniform G2LightInfo light;\n"\
		};
		mPart = std::string(code);
	}
	else if(shadingLanguage == ShadingLanguage::CG) 
	{
		const char* code =
		{
			"struct G2LightInfo {\n"\
			"	// coloring\n"\
			"	float4 color;         // The color of the light\n"\
			"	float4 attenuation;   // [x=constant,y=linear,z=quadric]\n"\
			"	// positional\n"\
			"	float4 position;      // Local position - if(position.w == 0.0) -> directional light, else -> point or spot light\n"\
			"	float3 direction;     // Light direction\n"\
			"	float range;        // The distance or the range of the light cone\n"\
			/*"	float4x4 light2Local;   // converts light position to local position\n"\
			"	float4x4 light2World;   // converts light position to world position\n"\
			"	float4x4 world2Light;   // converts world position to light position\n"\
			"	float4x4 local2Light;   // converts local position to light position\n"*/\
			"	// special\n"\
			"	float cosCutoff;    // if(cosCutoff >=0 ) spotlight else point or directional light\n"\
			"};\n"\
			"uniform G2LightInfo light;\n"\
		};
		mPart = std::string(code);
	}
}