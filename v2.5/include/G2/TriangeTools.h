#pragma once
#include "NonInstanceable.h"

#include <glm/glm.hpp>
#include <vector>

namespace G2 
{
	/** This class defines...
	 * @created:	2014/06/19
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class TriangeTools : public NonInstanceable
	{
		public:

			static void calculateTangentAndBinormalForTriangle(
				unsigned int vertexIndex1,
				unsigned int vertexIndex2,
				unsigned int vertexIndex3,
				glm::vec3 const& p1,
				glm::vec3 const& p2,
				glm::vec3 const& p3,
				glm::vec2 const& t1,
				glm::vec2 const& t2,
				glm::vec2 const& t3,
				std::vector<glm::vec3>& binormals,
				std::vector<glm::vec3>& tangents
				);

			static bool isDegenerated(
				glm::vec3 const& p1,
				glm::vec3 const& p2,
				glm::vec3 const& p3
				);
		private:
			~TriangeTools();
	};
};