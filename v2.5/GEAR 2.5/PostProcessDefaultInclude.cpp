// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "PostProcessDefaultInclude.h"
#include <math.h>

using namespace G2;

PostProcessDefaultInclude::PostProcessDefaultInclude(ShadingLanguage::Name shadingLanguage)
{
	if(shadingLanguage == ShadingLanguage::GLSL) 
	{
		const char* code =
		{
			"struct G2PostProcessingInfo {\n"\
			"	vec2 pixelSize;			// The size of one screen pixel in window coordinates\n"\
			"	vec2 windowSize;		// The width (=x) and height (=y) of the window in pixels\n"\
			"};\n"\
			"uniform G2PostProcessingInfo postProcessInfo;\n"\
		};
		mPart = std::string(code);
	}
	else if(shadingLanguage == ShadingLanguage::CG) 
	{
		const char* code =
		{
			"struct G2PostProcessingInfo {\n"\
			"	float2 pixelSize;			// The size of one screen pixel in window coordinates\n"\
			"	float2 windowSize;		// The width (=x) and height (=y) of the window in pixels\n"\
			"};\n"\
			"uniform G2PostProcessingInfo postProcessInfo;\n"\
		};
		mPart = std::string(code);
	}
}