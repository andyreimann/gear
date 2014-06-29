#include "SplineAnimation.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

#include <G2Core/ECSManager.h>

#include <glm/gtx/spline.hpp>
#include <glm/ext.hpp>

using namespace G2;

const static glm::mat4 catmullRomMatrix = glm::mat4(
	-0.5f,  1.0f, -0.5f,  0.0f,
	 1.5f, -2.5f,  0.0f,  1.0f,
	-1.5f,  2.0f,  0.5f,  0.0f,
	 0.5f, -0.5f,  0.0f,  0.0f
);

const static glm::mat4 basisSplineMatrix = glm::mat4(
	-1.f,  3.f, -3.f,  1.f,
	 3.f, -6.f,  0.f,  4.f,
	-3.f,  3.f,  3.f,  1.f,
	 1.f,  0.f,  0.f,  0.f)/* * (1.0f / 6.0f)*/;

const static glm::mat4 hermiteMatrix = glm::mat4(
	 2.f, -3.f,  0.f,  1.f,
	-2.f,  3.f,  0.f,  0.f,
	 1.f, -2.f,  1.f,  0.f,
	 1.f, -1.f,  0.f,  0.f);
	
	/*
	http://www.cubic.org/docs/hermite.htm
	http://www.cubic.org/docs/bezier.htm

	// BEZIER IS DIFFERENT!

	const static glm::mat4 bezierMatrix = glm::mat4(
		-1,  3, -3,  1,
		 3, -6,  3,  0,
		-3,  3,  0,  0,
		 1,  0,  0,  0);
	*/

SplineAnimation::SplineAnimation(AnimationDescription const& animationDescription, std::vector<AnimationSample> const& animationSamples)
	: mAnimationDescription(animationDescription),
	mAnimationSamples(animationSamples)
{
	mCurrentState.sampleIndex = 0;
	mCurrentState.interpolationValue = 0.f;

	switch(mAnimationDescription.interpolationType)
	{
	case SplineInterpolationType::BASIS_SPLINE:
		mInterpolationMatrix = basisSplineMatrix;
		break;
	case SplineInterpolationType::CATMULL_ROM:
		mInterpolationMatrix = catmullRomMatrix;
		break;
	case SplineInterpolationType::HERMITE_SPLINE:
		mInterpolationMatrix = hermiteMatrix;
		break;
	default:
		mInterpolationMatrix = catmullRomMatrix;
	}
}

void interpolation(
		glm::mat4 const& matCubic,
		glm::vec4 const& geometry,
		float ip,
		float& heightOut,
		float& slopeOut) {

	glm::vec4 T = glm::vec4(0.f,0.f,0.f,0.f);
	glm::vec4 ABCD = glm::vec4(0.f,0.f,0.f,0.f);

	// Calculate Cubic Coefficients
	ABCD = matCubic * geometry;

	// T Vector
	T.w = 1;
	T.z = ip;
	T.y = T.z * T.z;
	T.x = T.y * T.z;

	// Solve Cubic for Height
	heightOut = glm::dot(T,ABCD);

	// T Vector for Derivative
	T.w = 0;
	T.z = 1;
	T.y = 2.0f * ip;
	T.x = 3.0f * ip * ip;

	// Solve Quadratic for Slope
	slopeOut = glm::dot(T,ABCD);
}

void
SplineAnimation::animate(double const& timeSinceLastFrame, TransformComponent* transformComponent) 
{
	if(mAnimationSamples.size() < 1 || (!mAnimationDescription.animateOrientation && !mAnimationDescription.animatePosition))
	{
		return;
	}
	double i1 = 1.0 - mCurrentState.interpolationValue;
	double i2 = mCurrentState.interpolationValue;
	AnimationSample* currentSample = &mAnimationSamples[mCurrentState.sampleIndex];
	AnimationSample* nextSample = &mAnimationSamples[std::min((int)mAnimationSamples.size()-1,mCurrentState.sampleIndex+1)];
	if(mAnimationDescription.loops && mCurrentState.sampleIndex+1 > mAnimationSamples.size()-1)
	{
		nextSample = &mAnimationSamples[0];
	}
	double delta = ((i1 * currentSample->speed) + (i2 * nextSample->speed)) * timeSinceLastFrame;
	mCurrentState.interpolationValue += delta;
	if(mCurrentState.interpolationValue > 1.0)
	{
		// check if we would step out of range now
		if(mAnimationSamples.size()-1 < mCurrentState.sampleIndex+1)
		{
			// we are out of range
			if(mAnimationDescription.loops)
			{
				// step back to 0 in case we loop
				mCurrentState.sampleIndex = 0;
			}
		}
		else
		{
			++mCurrentState.sampleIndex;
		}
		mCurrentState.interpolationValue -= 1.f;
	}

	// calculate where the sampling points are depending on the animation mode to use
	AnimationSample* s0 = nullptr;
	AnimationSample* s1 = &mAnimationSamples[mCurrentState.sampleIndex];
	AnimationSample* s2 = nullptr;
	AnimationSample* s3 = nullptr;
	if(mAnimationDescription.loops)
	{
		if(mCurrentState.sampleIndex-1 < 0)
		{
			s0 = &mAnimationSamples[mAnimationSamples.size()-1];
		}
		else 
		{
			s0 = &mAnimationSamples[mCurrentState.sampleIndex-1];
		}
		if(mCurrentState.sampleIndex+1 >= mAnimationSamples.size())
		{
			s2 = &mAnimationSamples[0];
		}
		else
		{
			s2 = &mAnimationSamples[mCurrentState.sampleIndex+1];
		}
		if(mCurrentState.sampleIndex+2 >= mAnimationSamples.size())
		{
			if(mCurrentState.sampleIndex+1 >= mAnimationSamples.size())
			{
				s3 = &mAnimationSamples[std::min(1,(int)mAnimationSamples.size())];
			}
			else
			{
				s3 = &mAnimationSamples[0];
			}
		}
		else
		{
			s3 = &mAnimationSamples[mCurrentState.sampleIndex+2];
		}
	}
	else
	{
		s0 = &mAnimationSamples[std::max(0,mCurrentState.sampleIndex-1)];
		s2 = &mAnimationSamples[std::min((int)mAnimationSamples.size()-1,mCurrentState.sampleIndex+1)];
		s3 = &mAnimationSamples[std::min((int)mAnimationSamples.size()-1,mCurrentState.sampleIndex+2)];
	}

	// do the animation
	//return glm::catmullRom(s0->point, s1->point, s2->point, s3->point, (float)mCurrentState.interpolationValue);
	//return glm::hermite(s0->point, s1->point, s2->point, s3->point, (float)(2.0*mCurrentState.interpolationValue-1.0));
	float x, y, z, slopeX, slopeY, slopeZ;
	interpolation(mInterpolationMatrix, glm::vec4(s0->point.x, s1->point.x, s2->point.x, s3->point.x), (float)mCurrentState.interpolationValue, x, slopeX);
	interpolation(mInterpolationMatrix, glm::vec4(s0->point.y, s1->point.y, s2->point.y, s3->point.y), (float)mCurrentState.interpolationValue, y, slopeY);
	interpolation(mInterpolationMatrix, glm::vec4(s0->point.z, s1->point.z, s2->point.z, s3->point.z), (float)mCurrentState.interpolationValue, z, slopeZ);
	glm::vec3 pos(x,y,z);
	if(mAnimationDescription.animateOrientation)
	{
		transformComponent->setRotation(glm::lookAt(glm::vec3(), glm::vec3(slopeZ+0.000001f, slopeY, slopeX+0.000001f), glm::vec3(0.f,1.f,0.f)));
	}
	if(mAnimationDescription.animatePosition)
	{
		transformComponent->setPosition(pos);
	}
}

std::vector<glm::vec3>
SplineAnimation::debugSamplePath(float samplingRate) 
{
	std::vector<glm::vec3> geometry;

	for(int i = 0; i < mAnimationSamples.size(); ++i)
	{
			
		AnimationSample* s0 = nullptr;
		AnimationSample* s1 = &mAnimationSamples[i];
		AnimationSample* s2 = nullptr;
		AnimationSample* s3 = nullptr;
		if(mAnimationDescription.loops)
		{
			if(i-1 < 0)
			{
				s0 = &mAnimationSamples[mAnimationSamples.size()-1];
			}
			else 
			{
				s0 = &mAnimationSamples[i-1];
			}
			if(i+1 >= mAnimationSamples.size())
			{
				s2 = &mAnimationSamples[0];
			}
			else
			{
				s2 = &mAnimationSamples[i+1];
			}
			if(i+2 >= mAnimationSamples.size())
			{
				if(i+1 >= mAnimationSamples.size())
				{
					s3 = &mAnimationSamples[std::min(1,(int)mAnimationSamples.size())];
				}
				else
				{
					s3 = &mAnimationSamples[0];
				}
			}
			else
			{
				s3 = &mAnimationSamples[i+2];
			}
		}
		else
		{
			s0 = &mAnimationSamples[std::max(0,i-1)];
			s2 = &mAnimationSamples[std::min((int)mAnimationSamples.size()-1,i+1)];
			s3 = &mAnimationSamples[std::min((int)mAnimationSamples.size()-1,i+2)];
		}

		float x, y, z, slopeX, slopeY, slopeZ;
		for(float s = 0.f; s <= 1.00001f; s+=samplingRate)
		{
			interpolation(mInterpolationMatrix, glm::vec4(s0->point.x, s1->point.x, s2->point.x, s3->point.x), s, x, slopeX);
			interpolation(mInterpolationMatrix, glm::vec4(s0->point.y, s1->point.y, s2->point.y, s3->point.y), s, y, slopeY);
			interpolation(mInterpolationMatrix, glm::vec4(s0->point.z, s1->point.z, s2->point.z, s3->point.z), s, z, slopeZ);
			geometry.push_back(glm::vec3(x,y,z));
		}
	}
	return geometry;
}