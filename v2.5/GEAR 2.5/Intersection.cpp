// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Intersection.h"

using namespace G2;

Intersection::Intersection() :
	mState(NO_INTERSECTION)
{
}

Intersection::Intersection(glm::vec3 const& point, glm::vec3 const& normal) :
	mPoint(point), 
	mNormal(normal), 
	mState(INTERSECTION)
{
}

Intersection Intersection::rayTriangle(Ray const& r, glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3)
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
}

Intersection Intersection::rayTriangle(Ray const& r, glm::vec4 const& p1, glm::vec4 const& p2, glm::vec4 const& p3)
{
				glm::vec3 p1v3(p1);
				glm::vec3 p2v3(p2);
				glm::vec3 p3v3(p3);
				return rayTriangle(r, p1v3,p2v3,p3v3);
}