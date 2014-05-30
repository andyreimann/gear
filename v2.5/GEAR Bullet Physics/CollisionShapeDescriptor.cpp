#include "CollisionShapeDescriptor.h"

#include <utility>

using namespace G2::Physics;

CollisionShapeDescriptor::CollisionShapeDescriptor(CollisionShape::Name type, int numVertices) 
	: mType(type),
	mVertexStream(numVertices,3)
{
}

CollisionShapeDescriptor 
CollisionShapeDescriptor::sphere(float radius) 
{
	CollisionShapeDescriptor desc(CollisionShape::SPHERE);
	desc.mRadius = radius;
	return std::move(desc);
}

CollisionShapeDescriptor 
CollisionShapeDescriptor::staticPlane(glm::vec3 const& normal, float constant)
{
	CollisionShapeDescriptor desc(CollisionShape::STATIC_PLANE);
	desc.mNormal = glm::normalize(normal);
	desc.mConstant = constant;
	return std::move(desc);
}

CollisionShapeDescriptor 
CollisionShapeDescriptor::box(glm::vec3 const& halfExtends) 
{
	CollisionShapeDescriptor desc(CollisionShape::BOX);
	desc.mNormal = halfExtends;
	return std::move(desc);
}

CollisionShapeDescriptor 
CollisionShapeDescriptor::cylinder(glm::vec3 const& halfExtends) 
{
	CollisionShapeDescriptor desc(CollisionShape::CYLINDER);
	desc.mNormal = halfExtends;
	return std::move(desc);
}

CollisionShapeDescriptor 
CollisionShapeDescriptor::capsule(float radius, float height) 
{
	CollisionShapeDescriptor desc(CollisionShape::CAPSULE);
	desc.mRadius = radius;
	desc.mConstant = height;
	return std::move(desc);
}

CollisionShapeDescriptor 
CollisionShapeDescriptor::cone(float radius, float height) 
{
	CollisionShapeDescriptor desc(CollisionShape::CONE);
	desc.mRadius = radius;
	desc.mConstant = height;
	return std::move(desc);
}

CollisionShapeDescriptor
CollisionShapeDescriptor::convexHull(std::vector<G2::VertexArrayObject>& vaos) 
{
	int numVertices = 0;
	for(int i = 0; i < vaos.size(); ++i)
	{
		numVertices += vaos[i].getNumElements();
	}
	CollisionShapeDescriptor desc(CollisionShape::CONVEX_HULL, numVertices);
	
	for(int i = 0; i < vaos.size(); ++i)
	{
		desc.mVertexStream.write(vaos[i],Semantics::POSITION);
	}
	return std::move(desc);
}

CollisionShapeDescriptor
CollisionShapeDescriptor::triangleMesh(std::vector<G2::VertexArrayObject>& vaos, glm::mat4 const& worldSpaceMatrix) 
{
	int numVertices = 0;
	for(int i = 0; i < vaos.size(); ++i)
	{
		numVertices += vaos[i].getNumElements();
	}
	CollisionShapeDescriptor desc(CollisionShape::TRIANGLE_MESH, numVertices);
	
	for(int i = 0; i < vaos.size(); ++i)
	{
		desc.mVertexStream.writeTriangles(vaos[i], worldSpaceMatrix);
	}
	return std::move(desc);
}

CollisionShapeDescriptor::CollisionShapeDescriptor(CollisionShapeDescriptor && rhs)
	: mVertexStream(0,0)
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

CollisionShapeDescriptor&
CollisionShapeDescriptor::operator=(CollisionShapeDescriptor && rhs) 
{
	// 1. Stage: delete maybe allocated resources on target type
	// 2. Stage: transfer data from src to target
	mType = rhs.mType;
	mRadius = rhs.mRadius;
	mConstant = rhs.mConstant;
	mNormal = std::move(rhs.mNormal);
	mVertexStream = std::move(rhs.mVertexStream);
	// 3. Stage: modify src to a well defined state
	rhs.mType = CollisionShape::INVALID_COLLISION_SHAPE;

	return *this;
}