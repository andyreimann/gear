#include "HermiteCurve.h"
#include <utility>

#include <glm/ext.hpp>

using namespace G2;

HermiteCurve::HermiteCurve(InterpolationDescription const& interpolationDescription, std::vector<CurveSample> const& curveSamples)
	: Curve(interpolationDescription, curveSamples)
{}

const static glm::mat4 hermiteMatrix = glm::mat4(
	 2.f, -3.f,  0.f,  1.f,
	-2.f,  3.f,  0.f,  0.f,
	 1.f, -2.f,  1.f,  0.f,
	 1.f, -1.f,  0.f,  0.f
);

void
HermiteCurve::interpolate(double const& timeStep) 
{
	if(mCurveSamples.size() < 4 || (!mInterpolationDesc.animateOrientation && !mInterpolationDesc.animatePosition))
	{
		return;
	}

	// step the abstract curve internally forward
	Curve::interpolate(timeStep);

	// do the interpolation
	float slopeX, slopeY, slopeZ;
	_interpolate(hermiteMatrix, glm::vec4(mSample1->point.x, mSample2->point.x, mSample1->tangent.x, mSample2->tangent.x), (float)mInterpolationValue, mCurrentPosition.x, slopeX);
	_interpolate(hermiteMatrix, glm::vec4(mSample1->point.y, mSample2->point.y, mSample1->tangent.y, mSample2->tangent.y), (float)mInterpolationValue, mCurrentPosition.y, slopeY);
	_interpolate(hermiteMatrix, glm::vec4(mSample1->point.z, mSample2->point.z, mSample1->tangent.z, mSample2->tangent.z), (float)mInterpolationValue, mCurrentPosition.z, slopeZ);
	mCurrentRotation = glm::lookAt(glm::vec3(), glm::vec3(slopeZ+0.000001f, slopeY, slopeX+0.000001f), glm::vec3(0.f,1.f,0.f));
}

bool
HermiteCurve::usePoints() 
{
	return true;
}

bool
HermiteCurve::useTangents() 
{
	return true;
}