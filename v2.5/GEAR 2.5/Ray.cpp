// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Ray.h"
#include <iostream>
#include <glm/ext.hpp>
#include "Defines.h"

#include "ProjectionTools.h"

using namespace G2;

Ray::Ray(glm::vec3 const& origin, glm::vec4 const& dir)
	: mOrigin(origin),
	mDir(dir) 
{
	mDir = glm::normalize(mDir);
}

Ray& 
Ray::rotate(glm::mat4 const& martix) 
{
	
	mDir = mDir * martix;
	mDir = glm::normalize(mDir);
	//std::cout << " dir " << getDir().x << "," << getDir().y << "," << getDir().z << "\n";
		
	return *this;
}

glm::vec3 const& 
Ray::getOrigin() const 
{
	return mOrigin;
}

glm::vec4 const& 
Ray::getDir() const 
{
	return mDir;
}

G2::Ray
Ray::createScreenProjectionRay(int screenX, int screenY,
							  glm::vec3 const& camViewDir, 
							  glm::mat4 const& camModelView, 
							  glm::mat4 const& camProjection, 
							  glm::detail::tvec4<int> const& viewport) {
	glm::vec3 obj;
	G2::Tools::Projection::unProject(glm::vec3((float)screenX, /*invert y coord*/(float)(viewport.z - screenY), 0.f), camModelView, camProjection, viewport, obj);
	glm::vec3 dir = camViewDir;
	return Ray(obj, glm::vec4(dir, 0.f));
}