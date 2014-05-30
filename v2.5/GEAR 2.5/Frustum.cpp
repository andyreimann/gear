#include "Frustum.h"

#include <utility>
#include <glm/ext.hpp>

using namespace G2;

Frustum::Frustum() 
{
}

Frustum::Frustum(Frustum const& rhs) 
{
	// do copy here
}

Frustum::Frustum(Frustum && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

Frustum&
Frustum::operator=(Frustum const& rhs) 
{
	// do assignment here
	return *this;
}

Frustum&
Frustum::operator=(Frustum && rhs) 
{
	// 1. Stage: delete maybe allocated resources on target type
	// 2. Stage: transfer data from src to target
	// 3. Stage: modify src to a well defined state
	return *this;
}

bool
Frustum::inside(glm::vec3 const& p) const 
{
	assert(0); // not implemented
	return false;
}

bool
Frustum::inside(AABB const& aabb) const 
{
	glm::vec3 minimum = aabb.getMin();
	glm::vec3 maximum = aabb.getMax();
	//for each plane do ...
	glm::vec3 pos;
	glm::vec3 neg;
	bool result = true;
	for(int i = 0; i < FrustumPlane::NUM_PLANES; ++i) {
		pos = minimum;
		neg = maximum;
		if (mPlanes[i].normal.x > 0.f) { pos.x = maximum.x; neg.x = minimum.x; }
		if (mPlanes[i].normal.y > 0.f) { pos.y = maximum.y; neg.y = minimum.y; }
		if (mPlanes[i].normal.z > 0.f) { pos.z = maximum.z; neg.z = minimum.z; }
		// is the positive vertex outside?
		if (mPlanes[i].distance(pos) < 0.f) {
			return false;
		}
		// is the negative vertex outside?	
		else if (mPlanes[i].distance(neg) < 0.f)
			result = true; // intersection
	}
	return result;
}

float
Frustum::Plane::distance(glm::vec3 const& pt) const 
{
	return (a * pt.x + b*pt.y + c * pt.z + d);
}

void 
Frustum::setup(glm::mat4 const& mvp) {

	float a, b, c, d;
	float const* mvpPtr = glm::value_ptr(mvp);

	a = mvpPtr[3] - mvpPtr[1];
	b = mvpPtr[7] - mvpPtr[5];
	c = mvpPtr[11] - mvpPtr[9];
	d = mvpPtr[15] - mvpPtr[13];
	mPlanes[FrustumPlane::TOP].setCoefficients( a, b, c, d );

	a = mvpPtr[3] + mvpPtr[1];
	b = mvpPtr[7] + mvpPtr[5];
	c = mvpPtr[11] + mvpPtr[9];
	d = mvpPtr[15] + mvpPtr[13];
	mPlanes[FrustumPlane::BOTTOM].setCoefficients( a, b, c, d );

	a = mvpPtr[3] + mvpPtr[0];
	b = mvpPtr[7] + mvpPtr[4];
	c = mvpPtr[11] + mvpPtr[8];
	d = mvpPtr[15] + mvpPtr[12];
	mPlanes[FrustumPlane::LEFT].setCoefficients( a, b, c, d );

	a = mvpPtr[3] - mvpPtr[0];
	b = mvpPtr[7] - mvpPtr[4];
	c = mvpPtr[11] - mvpPtr[8];
	d = mvpPtr[15] - mvpPtr[12];
	mPlanes[FrustumPlane::RIGHT].setCoefficients( a, b, c, d );

	a = mvpPtr[3] + mvpPtr[2];
	b = mvpPtr[7] + mvpPtr[6];
	c = mvpPtr[11] + mvpPtr[10];
	d = mvpPtr[15] + mvpPtr[14];
	mPlanes[FrustumPlane::NEARP].setCoefficients( a, b, c, d );

	a = mvpPtr[3] - mvpPtr[2];
	b = mvpPtr[7] - mvpPtr[6];
	c = mvpPtr[11] - mvpPtr[10];
	d = mvpPtr[15] - mvpPtr[14];
	mPlanes[FrustumPlane::FARP].setCoefficients( a, b, c, d );
}

void
	G2::Frustum::Plane::setCoefficients(float a, float b, float c, float d) 
{
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
	// set the normal vector
	normal = glm::vec3(a,b,c);
	//compute the inverse length of the vector
	float l = 1.0f / normal.length();
	// and multiply by the inverse length (division is more expensive)
	this->a = a*l;
	this->b = b*l;
	this->c = c*l;
	this->d = d*l;
	normal = glm::normalize(normal);
}