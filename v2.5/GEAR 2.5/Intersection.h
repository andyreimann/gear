// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Ray.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <limits>

namespace G2 
{
	enum IntersectionState 
	{
		NO_INTERSECTION,
		INTERSECTION,
	};
	/** This class holds all information about an Intersection object.
	 * It also provides useful functions to calculate intersections.
	 * @created:	2014/01/15
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class Intersection 
	{
		public:
			/** This constructs a new Intersection which is set to NO_INTERSECTION
			 */
			Intersection();
			/** This constructs a new Intersection at the given point and normal.
			 */
			Intersection(glm::vec3 const& point, glm::vec3 const& normal);
			/** This function will return the normal of the Intersection
			* @return The
			*/
			glm::vec3 const& getNormal() const { return mNormal; }
			/** This function will return the point of the Intersection
			* @return The point of the Intersection
			*/
			glm::vec3 const& getPoint() const { return mPoint; }
			/** This function will return the IntersectionState of the Intersection.
			* @return The IntersectionState of the Intersection.
			*/
			G2::IntersectionState const& getState() const { return mState; }
			/** Calculates an intersection between a ray and a triangle.
			 * @param r The ray to intersect the triangle with.
			 * @param p1 The first point of the triangle.
			 * @param p2 The second point of the triangle.
			 * @param p3 The third point of the triangle.
			 * @return The intersection object containing the result of the calculation.
			 */
			static Intersection rayTriangle(Ray const& r, glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3);
			/** Calculates an intersection between a ray and a triangle.
			 * @param r The ray to intersect the triangle with.
			 * @param p1 The first point of the triangle.
			 * @param p2 The second point of the triangle.
			 * @param p3 The third point of the triangle.
			 * @return The intersection object containing the result of the calculation.
			 */
			static Intersection rayTriangle(Ray const& r, glm::vec4 const& p1, glm::vec4 const& p2, glm::vec4 const& p3);
		private:

			IntersectionState	mState;		// The state of the Intersection.
			glm::vec3			mPoint;		// The intersection point in case there is an intersection.
			glm::vec3			mNormal;	// The intersection points normal vector in case there is an intersection.
	};
};