// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Ray.h"
#include "Entity.h"
#include <glm/glm.hpp>

namespace G2 
{
	/** This class defines the data structure for a simple Axis-Aligned Bounding Box.
	 * @note The Corners of the Axis-Aligned Bounding Box should be in the full transformed format, which means that on Building the Min and Max-Vector, these Vectors should have applied all Transformations which are done up to the time of Building the Box.
	 * @brief Defines a Data structure for a Axis-Aligned Bounding Box
	 * @author Andy Reimann
	 */
	class AABB : public Entity 
	{

		public:
			/** Creates an AABB, which is in an empty state.
			 */
			AABB();
			/** Move constructor
			 */
			AABB(AABB && rhs);
			/** Move assignment operator to move an AABB.
			 */
			AABB& operator=(AABB && rhs);
			/** Constructor.
			 * @param Min the minimal corner of the box.
			 * @param Max the maximal corner of the box.
			 * @note The AABB will not check if the given coordinates are already the min and max values!
			 */
			AABB( glm::vec3 const& Min, glm::vec3 const& Max);
			/** This function will add a Point to the AABBox and will change internal parameters if needed.
			 * @param pt The Point to add.
			 * This have to be done, because adding a Point to the AABBox will only change the min and max members of the AABBox.
			 */
			AABB& merge( glm::vec3 const& pt );
			/** This function will add all corner points from the given AABB
			 * to the AABB.
			 * @param aabb The AABB to add.
			 */
			AABB& merge( AABB const& aabb );
			/** Returns the distance from the origin of the given Ray
			 * to the center point of the AABB.
			 * @returns The distance from the origin of the given Ray
			 * to the center point of the AABB.
			 */
			float getDistanceFromCenter(Ray const& ray) const;
			/** This function will set the internal state to represent an AABB with the given min and max coordinates.
			 * @param Min the minimal corner of the box.
			 * @param Max the maximal corner of the box.
			 */
			AABB& setFromMinAndMax( glm::vec3 const& Min, glm::vec3 const& Max);
			/** This function will set the internal state to represent an AABB with the given min and max coordinates.
			 * @param Min the minimal corner of the box.
			 * @param Max the maximal corner of the box.
			 */
			AABB& setFromCenterAndHalfExtends( glm::vec3 const& center, glm::vec3 const& halfExtends);
			/** This function performs a Ray- Axis-Aligned Bounding Box Intersection test.
			 * @return true if the Ray intersects the Axis-Aligned Bounding Box, false if not
			 */
			bool intersects( Ray const& ray ) const;
			/** Checks if the given AABB overlaps with the AABB
			 * @param aabb The AABB to check.
			 * @return True if the 2 AABB overlap, false if not.
			 */
			bool overlaps(AABB const& aabb) const;
			//bool contains(AABB const&) const;
			/** Checks if the given point is within the volume of the AABB.
			 * @param point The point to check.
			 * @return True is the point is within the AABB volume, false if not.
			 */
			bool contains(glm::vec3 const& point) const;
			/** Sets the center point of the AABB to the given one.
			 * @param center The new center point of the AABB.
			 */
			void setCenter(glm::vec3 const& center);
			/** Translates the AABB with the given translation vector.
			 * @param translation The translation vector to apply.
			 */
			void translate(glm::vec3 const& translation);
		
			/** This function calculates the maximum extend of the AABB.
 			 * @return The axis with the maximum extend.
			 * @note This function can be used to better compute the split axis for acceleration structures like a kd-tree.
			 */
			unsigned int maxExtend() const;
			/** Enables the rendering of the AABB.
			 * @note This function is only for developing purposes and should
			 * not be considered to be used in production environments.
			 * It will therefore build up the geometry and add a RenderComponent as 
			 * well as a TransformComponent.
			 */
			void enableRendering();
			/** Disables the rendering of the AABB.
			 * @note This function is only for developing purposes and should
			 * not be considered to be used in production environments.
			 */
			void disableRendering();
			/** Performs a reset of the AABB to an empty state.
			 * @note After this operation, a call to isEmpty() will return true.
			 * @return The AABB instance.
			 */
			AABB& clear();
			/** Checks if the AABB is in an empty state.
			 * @note An AABB is in an empty state when no point was added to it.
			 * @return True if the AABB is in an empty state, false if not.
			 */
			bool isEmpty() const;
			/** Returns the center point of the AABB.
			 * @return The center point of the AABB.
			 */
			glm::vec3 const& getCenter() const; 
			/** Returns the half extends vector of the AABB.
			 * @return The half extends vector of the AABB.
			 */
			glm::vec3 const& getHalfExtends() const; 
			/** Returns the corner with the minimum coordinates.
			 * @return The corner with the minimum coordinates.
			 */
			glm::vec3 getMin() const;
			/** Returns the corner with the maximum coordinates.
			 * @return The corner with the maximum coordinates.
			 */
			glm::vec3 getMax() const;

		private:
			/** Updates a maybe attached TransformComponent with the current
			 * center point.
			 * If no TransformComponent is attached, the call is silently ignored.
			 */
			void syncTransformComponent();

			glm::vec3	mCenter;			//!< The center point of the AABB
			glm::vec3	mHalfExtends;		//!< The half extends in each axis of the AABB
	};
};
