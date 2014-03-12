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
	/** This class defines...
	 * @created:	2014/01/15
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class Intersection 
	{
		public:
			/** This constructs a new Intersection which is set to NO_INTERSECTION
			 */
			Intersection() : mState(NO_INTERSECTION) {}
			/** This constructs a new Intersection at the given point and normal.
			 */
			Intersection(glm::vec3 const& point, glm::vec3 const& normal) : mPoint(point), mNormal(normal), mState(INTERSECTION) {}
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
			
			static Intersection rayTriangle(Ray const& r, glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3)
			{
				glm::vec3 intersectionPt;
				bool intersects = glm::intersectRayTriangle(r.getOrigin(), glm::vec3(r.getDir()), p1, p2, p3, intersectionPt);
				if(!intersects)
				{
					return Intersection();
				}
				else
				{
					glm::vec3 v1 = p2 - p1;
					glm::vec3 v2 = p3 - p1;
					return Intersection(intersectionPt, glm::cross(v1,v2));
				}

				//float eps = std::numeric_limits<float>::epsilon();
				//glm::vec3 rayDir = glm::vec3(r.getDir());
				//glm::vec3 v1 = p2 - p1;
				//glm::vec3 v2 = p3 - p1;
				//glm::vec3 n = glm::cross(v1,v2);
				//n = glm::normalize(n);
				//n *= -1;
				//float dist = -glm::dot(r.getOrigin() - p1, n) / glm::dot(rayDir, n);
				//if( dist > 0.0f ) 
				//{
				//	float bx, by, cx, cy, px, py;
				//	glm::vec3 intersection = r.getOrigin() + (rayDir * dist);
				//	glm::vec3 p = intersection - p1;
				//	float nmax = std::max<float>(fabs(n.x), std::max<float>(fabs(n.y), fabs(n.z) ) );
				//	if( fabs( n.x ) == nmax )
				//	{
				//		bx = v1.y;
				//		by = v1.z;
				//		cx = v2.y;
				//		cy = v2.z;
				//		px = p.y;
				//		py = p.z;
				//	}
				//	else if( fabs( n.y ) == nmax ) 
				//	{
				//		bx = v1.x;
				//		by = v1.z;
				//		cx = v2.x;
				//		cy = v2.z;
				//		px = p.x;
				//		py = p.z;
				//	}
				//	else 
				//	{
				//		bx = v1.x;
				//		by = v1.y;
				//		cx = v2.x;
				//		cy = v2.y;
				//		px = p.x;
				//		py = p.y;
				//	}
				//	float u = (py * cx - px * cy) / (by * cx - bx * cy);
				//	float v = (py * bx - px * by) / (cy * bx - cx * by);
				//				
				//	if( u > 0.0f-eps && v > 0.0f-eps && (u + v) < 1.0f+eps) 
				//	{
				//		// this intersection point is closer
				//		return Intersection(intersection, n);
				//	}
				//}
				//return Intersection();
			}

			
			static Intersection rayTriangle(Ray const& r, glm::vec4 const& p1, glm::vec4 const& p2, glm::vec4 const& p3)
			{
				glm::vec3 p1v3(p1);
				glm::vec3 p2v3(p2);
				glm::vec3 p3v3(p3);
				return rayTriangle(r, p1v3,p2v3,p3v3);
			}
		private:

			IntersectionState	mState;
			glm::vec3			mPoint;
			glm::vec3			mNormal;
	};
};