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
			"	vec4 ambient;			// The ambient color of the light\n"\
			"	vec4 diffuse;			// The diffuse color of the light\n"\
			"	vec4 specular;			// The specular color of the light\n"\
			"	vec4 attenuation;		// [x=constant,y=linear,z=quadric]\n"\
			"	// positional\n"\
			"	vec4 position;			// Local position - if(position.w == 0.0) -> directional light, else -> point or spot light\n"\
			"	vec3 direction;			// Light direction\n"\
			"	float range;			// The distance or the range of the light cone\n"\
			/*"	mat4 light2Local;		// converts light position to local position\n"\
			"	mat4 light2World;		// converts light position to world position\n"\
			"	mat4 world2Light;		// converts world position to light position\n"\
			"	mat4 local2Light;		// converts local position to light position\n"*/\
			"	// special\n"\
			"	float cosCutoff;		// if(cosCutoff >=0 ) spotlight else point or directional light\n"\
			"	// shadow\n"\
			"	int	  type;				// 0 = no shadow, 1 = CSM\n"\
			"	int	  numCascades;		// The number of shadow cascades\n"\
			"	float zFar[4];			// The far clip planes used\n"\
			"	mat4  eyeToLightClip[4];// The bias*crop*proj*modelview matrix (only set if type = 1)\n"\
			"};\n"\
			"uniform G2LightInfo light[8];\n"\
			"uniform int G2ActiveLights;\n"\
		};
		mPart = std::string(code);
	}
	else if(shadingLanguage == ShadingLanguage::CG) 
	{
		const char* code =
		{
			"struct G2LightInfo {\n"\
			"	// coloring\n"\
			"	float4 ambient;       // The ambient color of the light\n"\
			"	float4 diffuse;       // The diffuse color of the light\n"\
			"	float4 specular;      // The specular color of the light\n"\
			"	float4 attenuation;   // [x=constant,y=linear,z=quadric]\n"\
			"	// positional\n"\
			"	float4 position;      // Local position - if(position.w == 0.0) -> directional light, else -> point or spot light\n"\
			"	float3 direction;     // Light direction\n"\
			"	float range;          // The distance or the range of the light cone\n"\
			/*"	float4x4 light2Local;   // converts light position to local position\n"\
			"	float4x4 light2World;   // converts light position to world position\n"\
			"	float4x4 world2Light;   // converts world position to light position\n"\
			"	float4x4 local2Light;   // converts local position to light position\n"*/\
			"	// special\n"\
			"	float cosCutoff;      // if(cosCutoff >=0 ) spotlight else point or directional light\n"\
			"	// shadow\n"\
			"	int	  type;			// 0 = no shadow, 1 = CSM\n"\
			"	int	  numCascades;	// The number of shadow cascades\n"\
			"	float zFar[4];		// The far clip planes used\n"\
			"	float4x4 eyeToLightClip[4];// The bias*crop*proj*modelview matrix (only set if type = 1)\n"\
			"};\n"\
			"uniform G2LightInfo light[8];\n"\
			"uniform int G2ActiveLights;\n"\
		};
		mPart = std::string(code);
	}
}