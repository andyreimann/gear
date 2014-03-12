// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "SimpleShaderPart.h"
#include "UberShader.h"

#include <string>

namespace G2 
{
	/** This class provides the shader code to insert if the default resource
	 * 'G2.matrices' is included.
	 * @created:	2014/02/09
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class MatricesDefaultInclude : public SimpleShaderPart 
	{
		public:
			/** This constructs a new MatricesDefaultInclude.
			 */
			MatricesDefaultInclude(ShadingLanguage::Name shadingLanguage);

	};
};