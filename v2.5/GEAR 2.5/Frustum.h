#pragma once
#include "AABB.h"

#include <glm/glm.hpp>

namespace G2 
{
	namespace FrustumPlane
	{
		enum Name {
				TOP = 0, 
				BOTTOM, 
				LEFT,
				RIGHT, 
				NEARP, 
				FARP, 
				NUM_PLANES
		};
	};
	/// This class defines...
	/// @created:	2014/05/24
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class Frustum 
	{
		friend struct ShadowDescriptor; // DEBUG

		public:
			/// This constructs a new Frustum.
			Frustum();
			
			/// Tests whether a point is inside the frustum or not.
			/// @param p The point to test.
			bool inside(glm::vec3 const& p) const;
			/** Tests whether an Axis-Aligned bounding box is inside the frustum or not.
			 * @param aabb The Axis-Aligned bounding box to test.
			 */
			bool inside(AABB const& aabb) const;
			/** This function will setup the Frustum to represent the one given 
			 * by the modelview projection matrix.
			 * @param modelViewProjectionMatrix The model view projection matrix to get a Frustum representation for.
			 */
			void setup(glm::mat4 const& modelViewProjectionMatrix);

			glm::vec4 const* getCornerPoints() const { return &mCorners[0]; } 
		protected:
			
			/// This struct defines a plane of a Frustum.
			/// The Frustum class uses this struct to calculate the Frustum planes of the Frustum to later be able to perform frustum culling within it.
			/// @brief This struct defines a plane of a Frustum.
			/// @author Andy Reimann
			struct Plane {
				float a, b, c, d;
				glm::vec3 normal;
				/** This function will set the Plane equation coefficients for the Plane equation a*x + b*y + c*z = d.
				 * @param a The a coefficient.
				 * @param b The a coefficient.
				 * @param c The a coefficient.
				 * @param d The a coefficient.
				 */
				void setCoefficients(float a, float b, float c, float d);

				
				float distance( glm::vec3 const& pt ) const;
			};

			glm::vec4	mCorners[8];
			Plane		mPlanes[FrustumPlane::NUM_PLANES]; // the six planes which defines the frustum
	};
};