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
		/// This class describes a collision shape.
		/// @created:	2014/05/29
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class CollisionShapeDescriptor
		{
			public:
				/// Creates a description for a sphere as a collision shape
				/// @param radius The radius of the sphere
				/// @return The created collision shape description
				static CollisionShapeDescriptor sphere(float radius);
				/// Creates a description for a static plane as a collision shape
				/// @param normal The normal vector of the static plane
				/// @param constant The constant of the static plane
				/// @return The created collision shape description
				static CollisionShapeDescriptor staticPlane(glm::vec3 const& normal, float constant);
				/// Creates a description for an axis aligned box as a collision shape
				/// @param halfExtends The half extends vector of the box
				/// @return The created collision shape description
				static CollisionShapeDescriptor box(glm::vec3 const& halfExtends);
				/// Creates a description for a cylinder as a collision shape
				/// @param halfExtends The half extends vector of the cylinder
				/// @return The created collision shape description
				static CollisionShapeDescriptor cylinder(glm::vec3 const& halfExtends);
				/// Creates a description for a capsule as a collision shape
				/// @param radius The radius of the capsule
				/// @param radius The height of the capsule
				/// @return The created collision shape description
				static CollisionShapeDescriptor capsule(float radius, float height);
				/// Creates a description for a cone as a collision shape
				/// @param radius The radius of the cone
				/// @param radius The height of the cone
				/// @return The created collision shape description
				static CollisionShapeDescriptor cone(float radius, float height);
				/// Creates a description for a convex hull as a collision shape
				/// @param vaos The vertex array objects to build a convex hull from
				/// @return The created collision shape description
				static CollisionShapeDescriptor convexHull(std::vector<G2::VertexArrayObject>& vaos);
				/// Creates a description for a triangle mesh as a collision shape
				/// @param vaos The vertex array objects containing the triangles for the triangle mesh
				/// @return The created collision shape description
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