#include "CatmullRomCurve.h"
#include <utility>

#include <glm/ext.hpp>

using namespace G2;

const static glm::mat4 catmullRomMatrix = glm::mat4(
	-0.5f,  1.0f, -0.5f,  0.0f,
	 1.5f, -2.5f,  0.0f,  1.0f,
	-1.5f,  2.0f,  0.5f,  0.0f,
	 0.5f, -0.5f,  0.0f,  0.0f
);

void CatmullRomCurve::_catmullRom(
		glm::vec4 const& geometry,
		float ip,
		float& valueOut,
		float& slopeOut) {

	glm::vec4 T = glm::vec4(0.f,0.f,0.f,0.f);
	glm::vec4 ABCD = glm::vec4(0.f,0.f,0.f,0.f);

	// Calculate Cubic Coefficients
	ABCD = catmullRomMatrix * geometry;

	// T Vector
	T.w = 1.f;
	T.z = ip;
	T.y = T.z * T.z;
	T.x = T.y * T.z;

	// Solve Cubic for Height
	valueOut = glm::dot(T,ABCD);

	// T Vector for Derivative
	T.w = 0.f;
	T.z = 1.f;
	T.y = 2.f * ip;
	T.x = 3.f * ip * ip;

	// Solve Quadratic for Slope
	slopeOut = glm::dot(T,ABCD);
}

CatmullRomCurve::CatmullRomCurve(InterpolationDescription const& interpolationDescription, std::vector<CurveSample> const& curveSamples)
	: Curve(interpolationDescription, curveSamples)
{
}

void
CatmullRomCurve::interpolate(double const& timeStep) 
{

	if(mCurveSamples.size() < 4 || (!mInterpolationDesc.animateOrientation && !mInterpolationDesc.animatePosition))
	{
		return;
	}

	Curve::interpolate(timeStep);

	

	// do the interpolation
	float slopeX, slopeY, slopeZ;
	_catmullRom(glm::vec4(mSample0->point.x, mSample1->point.x, mSample2->point.x, mSample3->point.x), (float)mInterpolationValue, mCurrentPosition.x, slopeX);
	_catmullRom(glm::vec4(mSample0->point.y, mSample1->point.y, mSample2->point.y, mSample3->point.y), (float)mInterpolationValue, mCurrentPosition.y, slopeY);
	_catmullRom(glm::vec4(mSample0->point.z, mSample1->point.z, mSample2->point.z, mSample3->point.z), (float)mInterpolationValue, mCurrentPosition.z, slopeZ);
	mCurrentRotation = glm::lookAt(glm::vec3(), glm::vec3(slopeZ+0.000001f, slopeY, slopeX+0.000001f), glm::vec3(0.f,1.f,0.f));
}

bool
CatmullRomCurve::usePoints() 
{
	return true;
}

bool
CatmullRomCurve::useTangents() 
{
	return false;
}