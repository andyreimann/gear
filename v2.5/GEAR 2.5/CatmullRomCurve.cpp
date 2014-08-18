#include "CatmullRomCurve.h"
#include <utility>

#include <glm/ext.hpp>

using namespace G2;

CatmullRomCurve::CatmullRomCurve(InterpolationDescription const& interpolationDescription, std::vector<CurveSample> const& curveSamples)
	: Curve(interpolationDescription, curveSamples)
{}

const static glm::mat4 catmullRomMatrix = glm::mat4(
	-0.5f,  1.0f, -0.5f,  0.0f,
	 1.5f, -2.5f,  0.0f,  1.0f,
	-1.5f,  2.0f,  0.5f,  0.0f,
	 0.5f, -0.5f,  0.0f,  0.0f
);

void
CatmullRomCurve::interpolate(double const& timeStep) 
{
	if(mCurveSamples.size() < 4 || (!mInterpolationDesc.animateOrientation && !mInterpolationDesc.animatePosition))
	{
		return;
	}

	// step the abstract curve internally forward
	Curve::interpolate(timeStep);

	// do the interpolation
	float slopeX, slopeY, slopeZ;
	_interpolate(catmullRomMatrix, glm::vec4(mSample0->point.x, mSample1->point.x, mSample2->point.x, mSample3->point.x), (float)mInterpolationValue, mCurrentPosition.x, slopeX);
	_interpolate(catmullRomMatrix, glm::vec4(mSample0->point.y, mSample1->point.y, mSample2->point.y, mSample3->point.y), (float)mInterpolationValue, mCurrentPosition.y, slopeY);
	_interpolate(catmullRomMatrix, glm::vec4(mSample0->point.z, mSample1->point.z, mSample2->point.z, mSample3->point.z), (float)mInterpolationValue, mCurrentPosition.z, slopeZ);
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