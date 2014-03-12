// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include <iostream>

#include <glm/glm.hpp>

namespace G2
{
	/** This class represents a ray in 3 dimensional space.
	 * @author Andy Reimann <devildeath1988@gmail.com>
	 */
	class Ray 
	{
		public:
			/** Constructs a new ray with an origin and a direction.
			 * @param origin The origin of the ray.
			 * @param dir The direction of the ray.
			 */
			Ray(glm::vec3 const& origin, glm::vec4 const& dir);

			static Ray createMouseProjectionRay(int mouseX, 
												int mouseY, 
												glm::vec3 const& camViewDir,
												glm::mat4 const& camModelView, 
												glm::mat4 const& camProjection, 
												glm::detail::tvec4<int> const& viewport);
			
			/** This function will rotate the direction of the ray 
			 * with the given matrix.
			 * @param matrix the matrix to rotate the ray with.
			 * @return The Ray instance reference.
			 */
			Ray& rotate(glm::mat4 const& martix);
			
			/** This function will return the origin of the Ray.
			 * @return The origin of the Ray.
			 */
			glm::vec3 const& getOrigin() const;

			/** This function will return the normalized direction of the Ray.
			 * @return The normalized direction of the Ray.
			 */
			glm::vec4 const& getDir() const;

		private:

			glm::vec3	mOrigin;	// The origin of the Ray
			glm::vec4	mDir;		// The normalized direction of the Ray
	};
};