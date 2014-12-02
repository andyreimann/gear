// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "NonInstanceable.h"

#include <glm/glm.hpp>

namespace G2 
{
	namespace Tools 
	{
		/** This class provides some easy to use helper functions based on std::string objects.
		 * @created:	2014/12/02
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class Projection : public NonInstanceable
		{
			public:
				static int project(glm::vec3 const& obj, glm::mat4 const& modelviewMat, glm::mat4 const& projectionMat, glm::detail::tvec4<int> const& viewport, glm::vec3& windowCoordinates);
				static int unProject(glm::vec3 const& win, glm::mat4 const& modelviewMat, glm::mat4 const& projectionMat, glm::detail::tvec4<int> const& viewport, glm::vec3& obj);


			private:
				~Projection();
		};

	};
};
