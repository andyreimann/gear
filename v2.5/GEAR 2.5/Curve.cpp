#include "Curve.h"

using namespace G2;

void
Curve::interpolate(double const& timeStep) 
{
	if(mCurveSamples.size() < 1)
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
	mSample0 = nullptr;
	mSample1 = &mCurveSamples[mCurrentIndex];
	mSample2 = nullptr;
	mSample3 = nullptr;
	if(mInterpolationDesc.loops)
	{
		if(mCurrentIndex-1 < 0)
		{
			mSample0 = &mCurveSamples[mCurveSamples.size()-1];
		}
		else 
		{
			mSample0 = &mCurveSamples[mCurrentIndex-1];
		}
		if(mCurrentIndex+1 >= mCurveSamples.size())
		{
			mSample2 = &mCurveSamples[0];
		}
		else
		{
			mSample2 = &mCurveSamples[mCurrentIndex+1];
		}
		if(mCurrentIndex+2 >= mCurveSamples.size())
		{
			if(mCurrentIndex+1 >= mCurveSamples.size())
			{
				mSample3 = &mCurveSamples[std::min(1,(int)mCurveSamples.size())];
			}
			else
			{
				mSample3 = &mCurveSamples[0];
			}
		}
		else
		{
			mSample3 = &mCurveSamples[mCurrentIndex+2];
		}
	}
	else
	{
		mSample0 = &mCurveSamples[std::max(0,mCurrentIndex-1)];
		mSample2 = &mCurveSamples[std::min((int)mCurveSamples.size()-1,mCurrentIndex+1)];
		mSample3 = &mCurveSamples[std::min((int)mCurveSamples.size()-1,mCurrentIndex+2)];
	}
}

std::vector<glm::vec3>
Curve::getPointsOnCurve(float samplingRate) 
{
	// dump state of curve
	int currentIndex = mCurrentIndex;
	double interpolationValue = mInterpolationValue;
	glm::vec3 currentPosition = mCurrentPosition;
	glm::mat4 currentRotation = mCurrentRotation;

	// reset curve
	mCurrentIndex = 0;
	mInterpolationValue = 0.;

	// sample
	std::vector<glm::vec3> samples;
	for(float s = 0.f; s <= 1.00001f * mCurveSamples.size(); )
	{
		interpolate(samplingRate);
		samples.push_back(mCurrentPosition);
		s = s + (samplingRate*(float)((1.0 - mInterpolationValue) * mSample1->speed)) + (samplingRate*(float)mInterpolationValue * (float)mSample2->speed);
	}


	// revert modifications
	mCurrentIndex = currentIndex;
	mInterpolationValue = interpolationValue;
	mCurrentPosition = currentPosition;
	mCurrentRotation = currentRotation;

	return samples;
}


void Curve::_interpolate(
		glm::mat4 const& matrix,
		glm::vec4 const& geometry,
		float ip,
		float& valueOut,
		float& slopeOut) {

	glm::vec4 T = glm::vec4(0.f,0.f,0.f,0.f);
	glm::vec4 ABCD = glm::vec4(0.f,0.f,0.f,0.f);

	// Calculate Cubic Coefficients
	ABCD = matrix * geometry;

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