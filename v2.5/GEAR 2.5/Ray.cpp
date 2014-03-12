// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Ray.h"
#include <iostream>
#include <glm/ext.hpp>
#include "Defines.h"

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
Ray::createMouseProjectionRay(int mouseX, int mouseY, 
							  glm::vec3 const& camViewDir, 
							  glm::mat4 const& camModelView, 
							  glm::mat4 const& camProjection, 
							  glm::detail::tvec4<int> const& viewport) {
	double ox, oy, oz;
		
	double mv[16];
	float const* mvValues = glm::value_ptr(camModelView);
	for (int i = 0; i < 16 ; ++i)
	{
		mv[i] = mvValues[i];
	}
	double p[16];
	float const* pValues = glm::value_ptr(camProjection);
	for (int i = 0; i < 16 ; ++i)
	{
		p[i] = pValues[i];
	}
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, &(vp[0]));

	gluUnProject(mouseX, /*invert y coord*/viewport.z-mouseY, 0, mv, p, vp, &ox, &oy, &oz);

	glm::vec3 dir = camViewDir;

	return Ray(glm::vec3(ox,oy,oz), glm::vec4(dir, 0.f));
}