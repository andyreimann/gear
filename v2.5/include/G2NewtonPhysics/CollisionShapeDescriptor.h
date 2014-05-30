#pragma once
#include <G2/VertexStream.h>

#include <glm/glm.hpp>

namespace G2 
{
	namespace Physics 
	{
		namespace CollisionShape
		{
			enum Name {
				SPHERE = 0,
				BOX,
				CYLINDER,
				CAPSULE,
				CONE,
				MULTISPHERE,
				CONVEX_HULL,
				CONVEX_TRIANGLE_MESH,
				TRIANGLE_MESH,
				HEIGHTFIELD_TERRAIN,
				STATIC_PLANE,
				INVALID_COLLISION_SHAPE,
			};
		};
		/// This class defines...
		/// @created:	2014/05/29
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class CollisionShapeDescriptor
		{
			public:
				
				static CollisionShapeDescriptor sphere(float radius);
				static CollisionShapeDescriptor staticPlane(glm::vec3 const& normal, float constant);
				static CollisionShapeDescriptor box(glm::vec3 const& halfExtends);
				static CollisionShapeDescriptor cylinder(glm::vec3 const& halfExtends);
				static CollisionShapeDescriptor capsule(float radius, float height);
				static CollisionShapeDescriptor cone(float radius, float height);
				static CollisionShapeDescriptor convexHull(std::vector<G2::VertexArrayObject>& vaos);
				static CollisionShapeDescriptor triangleMesh(std::vector<G2::VertexArrayObject>& vaos, glm::mat4 const& worldSpaceMatrix);

				/// Move constructor.
				CollisionShapeDescriptor(CollisionShapeDescriptor && rhs);
				/// Basic move operator.
				CollisionShapeDescriptor& operator=(CollisionShapeDescriptor && rhs);
				/// Returns the desired CollisionShape described.
				CollisionShape::Name getShape() const { return mType; }
				/// Returns the set radius.
				float getRadius() const { return mRadius; }
				/// Returns the set constant value.
				float getConstant() const { return mConstant; }
				/// Returns the set normal value.
				glm::vec3 const& getNormal() const { return mNormal; }
				/// Returns the set VertexStream.
				VertexStream const& getVertexStream() const { return mVertexStream; }

			private:
				/// This constructs a new CollisionShapeDescriptor.
				CollisionShapeDescriptor(CollisionShape::Name type, int numVertices = 0);

				CollisionShape::Name	mType;
				float					mRadius;
				float					mConstant;
				glm::vec3				mNormal;
				VertexStream			mVertexStream;
		};
	};
};