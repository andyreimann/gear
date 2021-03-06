// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Intersection.h"
#include <G2Core/Entity.h>

using namespace G2;

Intersection::Intersection() :
	mState(NO_INTERSECTION),
	mEntityId(G2::Entity::UNINITIALIZED_ENTITY_ID)
{
}

Intersection::Intersection(glm::vec3 const& point, glm::vec3 const& normal) :
	mPoint(point), 
	mNormal(normal), 
	mState(INTERSECTION),
	mEntityId(G2::Entity::UNINITIALIZED_ENTITY_ID)
{
}

Intersection&
Intersection::set(glm::vec3 const& point, glm::vec3 const& normal)
{
	mPoint = point;
	mNormal = normal;
	mState = INTERSECTION;
	return *this;
}

bool Intersection::rayTriangle(
	Intersection& intersection,
	glm::mat4 const& worldSpaceMatrix,
	G2::Ray const& worldSpaceRay,
	G2::Ray const& modelSpaceRay,
	glm::vec3 const& p1,
	glm::vec3 const& p2,
	glm::vec3 const& p3)
{
	glm::vec3 barycentricPos;
	bool intersects = glm::intersectRayTriangle(modelSpaceRay.getOrigin(), glm::vec3(modelSpaceRay.getDir()), p1, p2, p3, barycentricPos);
	if(!intersects)
	{
		return false;
	}
	else
	{
		glm::vec3 v1 = p2 - p1;
		glm::vec3 v2 = p3 - p1;
		// transform point and normal vector into world space (https://github.com/g-truc/glm/issues/6)
		intersection.set(
			glm::vec3(worldSpaceMatrix * glm::vec4(modelSpaceRay.getOrigin() + glm::vec3(modelSpaceRay.getDir()) * barycentricPos.z, 1.f)),
			glm::vec3(worldSpaceMatrix * glm::vec4(glm::cross(v1, v2),0.f)));
		return true;
	}
}

bool Intersection::rayTriangle(
	Intersection& intersection,
	glm::mat4 const& worldSpaceMatrix,
	G2::Ray const& worldSpaceRay,
	G2::Ray const& modelSpaceRay,
	glm::vec4 const& p1,
	glm::vec4 const& p2,
	glm::vec4 const& p3)
{
	glm::vec3 p1v3(p1);
	glm::vec3 p2v3(p2);
	glm::vec3 p3v3(p3);
	return rayTriangle(intersection, worldSpaceMatrix, worldSpaceRay, modelSpaceRay, p1v3, p2v3, p3v3);
}