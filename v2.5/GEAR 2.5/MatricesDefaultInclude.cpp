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
			"layout (std140) uniform G2Matrices {\n"\
			"	mat4 g2_projection_matrix; //The projection matrix\n"\
			"	mat4 g2_model_matrix; // the local transformation matrix of the vertex to render\n"\
			"	mat4 g2_view_matrix; // The world matrix of the camera\n"\
			"	mat4 g2_modelview_matrix; // The world matrix of the camera composed with the local transformation matrix of the vertex to render\n"\
			"	mat3 g2_normal_matrix; // The normal matrix for normal transformation\n"\
			"};\n"\
		};
		mPart = std::string(code);
	}
}