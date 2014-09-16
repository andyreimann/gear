// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "MatricesDefaultInclude.h"
#include <math.h>

using namespace G2;

MatricesDefaultInclude::MatricesDefaultInclude(std::string const& shadingLanguage)
{
	if(shadingLanguage == "GLSL") 
	{
		const char* code =
		{
			// see https://www.opengl.org/wiki/Interface_Block_(GLSL)
			"struct G2Matrices {\n"\
			"	mat4 projection_matrix; //The projection matrix\n"\
			"	mat3 normal_matrix; // The normal matrix for normal transformation\n"\
			"	mat4 model_matrix; // the local transformation matrix of the vertex to render\n"\
			"	mat4 view_matrix; // The world matrix of the camera\n"\
			"	mat4 modelview_matrix; // The world matrix of the camera composed with the local transformation matrix of the vertex to render\n"\
			"};\n"\
			"uniform G2Matrices matrices;\n"\
		};
		mPart = std::string(code);
	}
	else if(shadingLanguage == "CG") 
	{
		const char* code =
		{
			"struct G2Matrices {\n"\
			"	float4x4 projection_matrix; //The projection matrix\n"\
			"	float3x3 normal_matrix; // The normal matrix for normal transformation\n"\
			"	float4x4 model_matrix; // the local transformation matrix of the vertex to render\n"\
			"	float4x4 view_matrix; // The world matrix of the camera\n"\
			"	float4x4 modelview_matrix; // The world matrix of the camera composed with the local transformation matrix of the vertex to render\n"\
			"};\n"\
			"uniform G2Matrices matrices;\n"\
		};
		mPart = std::string(code);
	}
}