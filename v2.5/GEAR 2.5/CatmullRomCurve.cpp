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

	double i1 = 1.0 - mInterpolationValue;
	double i2 = mInterpolationValue;
	CurveSample const* currentSample = &mCurveSamples[mCurrentIndex];
	CurveSample const* nextSample = &mCurveSamples[std::min((int)mCurveSamples.size()-1,mCurrentIndex+1)];
	if(mInterpolationDesc.loops && mCurrentIndex+1 > mCurveSamples.size()-1)
	{
		nextSample = &mCurveSamples[0];
	}
	double delta = ((i1 * currentSample->speed) + (i2 * nextSample->speed)) * timeStep;
	mInterpolationValue += delta;
	if(mInterpolationValue > 1.0)
	{
		// check if we would step out of range now
		if(mCurveSamples.size()-1 < mCurrentIndex+1)
		{
			// we are out of range
			if(mInterpolationDesc.loops)
			{
				// step back to 0 in case we loop
				mCurrentIndex = 0;
			}
		}
		else
		{
			++mCurrentIndex;
		}
		mInterpolationValue -= 1.f;
	}

	// calculate where the sampling points are depending on the animation mode to use
	CurveSample const* s0 = nullptr;
	CurveSample const* s1 = &mCurveSamples[mCurrentIndex];
	CurveSample const* s2 = nullptr;
	CurveSample const* s3 = nullptr;
	if(mInterpolationDesc.loops)
	{
		if(mCurrentIndex-1 < 0)
		{
			s0 = &mCurveSamples[mCurveSamples.size()-1];
		}
		else 
		{
			s0 = &mCurveSamples[mCurrentIndex-1];
		}
		if(mCurrentIndex+1 >= mCurveSamples.size())
		{
			s2 = &mCurveSamples[0];
		}
		else
		{
			s2 = &mCurveSamples[mCurrentIndex+1];
		}
		if(mCurrentIndex+2 >= mCurveSamples.size())
		{
			if(mCurrentIndex+1 >= mCurveSamples.size())
			{
				s3 = &mCurveSamples[std::min(1,(int)mCurveSamples.size())];
			}
			else
			{
				s3 = &mCurveSamples[0];
			}
		}
		else
		{
			s3 = &mCurveSamples[mCurrentIndex+2];
		}
	}
	else
	{
		s0 = &mCurveSamples[std::max(0,mCurrentIndex-1)];
		s2 = &mCurveSamples[std::min((int)mCurveSamples.size()-1,mCurrentIndex+1)];
		s3 = &mCurveSamples[std::min((int)mCurveSamples.size()-1,mCurrentIndex+2)];
	}

	// do the interpolation
	float slopeX, slopeY, slopeZ;
	_catmullRom(glm::vec4(s0->point.x, s1->point.x, s2->point.x, s3->point.x), (float)mInterpolationValue, mCurrentPosition.x, slopeX);
	_catmullRom(glm::vec4(s0->point.y, s1->point.y, s2->point.y, s3->point.y), (float)mInterpolationValue, mCurrentPosition.y, slopeY);
	_catmullRom(glm::vec4(s0->point.z, s1->point.z, s2->point.z, s3->point.z), (float)mInterpolationValue, mCurrentPosition.z, slopeZ);
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