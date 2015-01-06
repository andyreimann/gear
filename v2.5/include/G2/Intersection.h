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
			/** Sets the internal point and normal to the given ones.
			 * @param point The point to set the Intersection to.
			 * @param normal The normal to set the Intersection normal vector to.
			 */
			Intersection& set(glm::vec3 const& point, glm::vec3 const& normal);
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
			/** This function will return the id of the Entity, wich was intersected.
			* @return The id of the Entity, wich was intersected.
			*/
			unsigned int const& getEntityId() const { return mEntityId; }
			/** This function will set the Entity ID to the given value.
			* @param value The EntityId.
			*/
			void setEntityId(unsigned int const& value) { mEntityId = value; }
			/** Calculates an intersection between a ray and a triangle.
			 * @param intersection The Intersection info object to write the result to in world space coordinates. Note that the object will not be changed in case there is no intersection!
			 * @param worldSpaceMatrix The current world space matrix.
			 * @param worldSpaceRay The ray to intersect the triangle with in world space coordinates.
			 * @param modelSpaceRay The ray to intersect the triangle with in model space coordinates.
			 * @param p1 The first point of the triangle in model space coordinates.
			 * @param p2 The second point of the triangle in model space coordinates.
			 * @param p3 The third point of the triangle in model space coordinates.
			 * @return True if an intersection was found, false if not.
			 */
			static bool rayTriangle(
				Intersection& intersection,
				glm::mat4 const& worldSpaceMatrix,
				G2::Ray const& worldSpaceRay,
				G2::Ray const& modelSpaceRay,
				glm::vec3 const& p1, 
				glm::vec3 const& p2, 
				glm::vec3 const& p3);
			/** Calculates an intersection between a ray and a triangle.
			* @param intersection The Intersection info object to write the result to in world space coordinates. Note that the object will not be changed in case there is no intersection!
			* @param worldSpaceMatrix The current world space matrix.
			* @param worldSpaceRay The ray to intersect the triangle with in world space coordinates.
			* @param modelSpaceRay The ray to intersect the triangle with in model space coordinates.
			* @param p1 The first point of the triangle in model space coordinates.
			* @param p2 The second point of the triangle in model space coordinates.
			* @param p3 The third point of the triangle in model space coordinates.
			* @return True if an intersection was found, false if not.
			 */
			static bool rayTriangle(
				Intersection& intersection,
				glm::mat4 const& worldSpaceMatrix,
				G2::Ray const& worldSpaceRay,
				G2::Ray const& modelSpaceRay,
				glm::vec4 const& p1,
				glm::vec4 const& p2,
				glm::vec4 const& p3);
		private:

			IntersectionState	mState;		// The state of the Intersection.
			glm::vec3			mPoint;		// The intersection point in case there is an intersection.
			glm::vec3			mNormal;	// The intersection points normal vector in case there is an intersection.
			unsigned int		mEntityId;	// The ID of the Entity which was intersected, or G2::Entity::UNINITIALIZED_ENTITY_ID if no Entity was intersected.
			
	};
};