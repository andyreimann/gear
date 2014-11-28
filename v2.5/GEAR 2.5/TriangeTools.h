#pragma once
#include "NonInstanceable.h"
#include "Ray.h"

#include <glm/glm.hpp>
#include <vector>
#include <map>

namespace G2 
{
	/** This class provides some helpful static functions in regards to problems related to triangles.
	 * @created:	2014/06/19
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class TriangeTools : public NonInstanceable
	{
		public:
			/** Calculates the tablent and binormal vector for a given triangle.
			 * @param vertexIndex1 The first point index of the triangle.
			 * @param vertexIndex2 The second point index of the triangle.
			 * @param vertexIndex3 The third point index of the triangle.
			 * @param p1 The first point of the triangle.
			 * @param p2 The second point of the triangle.
			 * @param p3 The third point of the triangle.
			 * @param t1 The first points texture coordinate.
			 * @param t2 The second points texture coordinate.
			 * @param t3 The third points texture coordinate.
			 * @param binormals A reference to a preallocated vector were the binormals are written to.
			 * @param binormals A reference to a preallocated vector were the tanbents are written to.
			 */
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
			/** Checks whether a given triangle is degenerated or not.
			 * A degenerated triangle is a triangle, in which at least 2 points
			 * share the same coordinates.
			 * @param p1 The first point of the triangle.
			 * @param p2 The second point of the triangle.
			 * @param p3 The third point of the triangle.
			 * @return True if the triangle is degenerated, false if not.
			 */
			static bool isDegenerated(
				glm::vec3 const& p1,
				glm::vec3 const& p2,
				glm::vec3 const& p3
				);

			/** This function performs an intersection of a ray and a triangle defined by 3 points.
			* @param ray The ray which is used for the intersection calculation.
			* @param p1 The first point of the triangle
			* @param p2 The second point of the triangle
			* @param p3 The third point of the triangle
			* @return A pair of an intersection point, if some intersection was found and a boolean indicating if an intersection was found. The boolean is true, if the intersection point is a real intersection point and false if not.
			*/
			static std::pair<glm::vec4, bool> intersectRayTriangle(G2::Ray const& ray, glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3);

		private:
			~TriangeTools();
	};
};