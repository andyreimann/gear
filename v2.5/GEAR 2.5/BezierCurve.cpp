#include "BezierCurve.h"
#include <utility>

#include <glm/ext.hpp>

using namespace G2;

BezierCurve::BezierCurve(InterpolationDescription const& interpolationDescription, std::vector<CurveSample> const& curveSamples)
	: Curve(interpolationDescription, curveSamples)
{}

const static glm::mat4 bezierMatrix = glm::mat4(
	-1.f,  3.f, -3.f,  1.f,
	 3.f, -6.f,  0.f,  4.f,
	-3.f,  3.f,  3.f,  1.f,
	 1.f,  0.f,  0.f,  0.f 
) * (1.0f / 6.0f);

void
BezierCurve::interpolate(double const& timeStep) 
{
	if(mCurveSamples.size() < 4 || (!mInterpolationDesc.animateOrientation && !mInterpolationDesc.animatePosition))
	{
		return;
	}

	// step the abstract curve internally forward
	Curve::interpolate(timeStep);

	// do the interpolation
	float slopeX, slopeY, slopeZ;
	_interpolate(bezierMatrix, glm::vec4(mSample0->point.x, mSample1->point.x, mSample2->point.x, mSample3->point.x), (float)mInterpolationValue, mCurrentPosition.x, slopeX);
	_interpolate(bezierMatrix, glm::vec4(mSample0->point.y, mSample1->point.y, mSample2->point.y, mSample3->point.y), (float)mInterpolationValue, mCurrentPosition.y, slopeY);
	_interpolate(bezierMatrix, glm::vec4(mSample0->point.z, mSample1->point.z, mSample2->point.z, mSample3->point.z), (float)mInterpolationValue, mCurrentPosition.z, slopeZ);
	mCurrentRotation = glm::lookAt(glm::vec3(), glm::vec3(slopeZ+0.000001f, slopeY, slopeX+0.000001f), glm::vec3(0.f,1.f,0.f));
}

bool
BezierCurve::usePoints() 
{
	return true;
}

bool
BezierCurve::useTangents() 
{
	return false;
}