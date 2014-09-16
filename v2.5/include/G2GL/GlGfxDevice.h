#pragma once
#include "Defines.h"
#include <glm/glm.hpp>

extern "C"
{
	GEAROPENGLDLL_API int GfxVersion();
	GEAROPENGLDLL_API void ClearColor(glm::vec4 const& color);
};