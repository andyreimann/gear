// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "LightDefaultInclude.h"
#include <math.h>

using namespace G2;

LightDefaultInclude::LightDefaultInclude(std::string const& shadingLanguage)
{
	if(shadingLanguage == "GLSL") 
	{
		const char* code =
		{
			"struct G2Light {\n"\
			"	vec4 ambient;			// The ambient color of the light\n"\
			"	vec4 diffuse;			// The diffuse color of the light\n"\
			"	vec4 specular;			// The specular color of the light\n"\
			"	vec4 attenuation;		// [x=constant,y=linear,z=quadric]\n"\
			"	vec4 zFar;				// The far clip planes used\n"\
			"	mat4  eyeToLightClip[4];// The bias*crop*proj*modelview matrix (only set if type = 1)\n"\
			"	vec4 position;			// Local position - if(position.w == 0.0) -> directional light, else -> point or spot light\n"\
			"	vec3 direction;			// Light direction\n"\
			"	float cosCutoff;		// if(cosCutoff >=0 ) spotlight else point or directional light\n"\
			"	int	  type;				// 0 = no shadow, 1 = CSM\n"\
			"	int	  numCascades;		// The number of shadow cascades\n"\
			"	float range;			// The distance or the range of the light cone\n"\
			"};\n"
			"layout (std140) uniform G2LightInfo {\n"\
			"	int G2ActiveLights;\n"\
			"	G2Light light[3]; // The data for the lights\n"\
			"};\n"
		};
		mPart = std::string(code);
	}
}

