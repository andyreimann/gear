#pragma once

#include "UniformBufferObject.h"

namespace G2 
{
	/** This class defines ...
	 * @created:	2014/12/05
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	struct DefaultUniformBufferObjects
	{
		DefaultUniformBufferObjects();

		UniformBufferObject material;
		UniformBufferObject lights;
		UniformBufferObject matrices;
		UniformBufferObject postProcessingInfo;
	};
};