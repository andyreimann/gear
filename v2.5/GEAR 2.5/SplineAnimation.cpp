#include "SplineAnimation.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "Curve.h"

#include <G2Core/ECSManager.h>

#include <glm/gtx/spline.hpp>
#include <glm/ext.hpp>

using namespace G2;

	/*
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
	 1.f,  0.f,  0.f,  0.f) * (1.0f / 6.0f);

const static glm::mat4 hermiteMatrix = glm::mat4(
	 2.f, -3.f,  0.f,  1.f,
	-2.f,  3.f,  0.f,  0.f,
	 1.f, -2.f,  1.f,  0.f,
	 1.f, -1.f,  0.f,  0.f);
	
	http://www.cubic.org/docs/hermite.htm
	http://www.cubic.org/docs/bezier.htm

	// BEZIER IS DIFFERENT!

	const static glm::mat4 bezierMatrix = glm::mat4(
		-1,  3, -3,  1,
		 3, -6,  3,  0,
		-3,  3,  0,  0,
		 1,  0,  0,  0);
	*/

SplineAnimation::SplineAnimation(std::shared_ptr<Curve> const& curve)
	: mCurve(curve)
{
}

void
SplineAnimation::animate(double const& timeSinceLastFrame, TransformComponent* transformComponent) 
{
	if(mCurve.get() != nullptr)
	{
		// step the curve
		mCurve->interpolate(timeSinceLastFrame);
		// transfer state to TransformComponent
		transformComponent->setPosition(mCurve->getPosition());
	}

	//if(mAnimationDescription.animateOrientation)
	//{
	//	transformComponent->setRotation(glm::lookAt(glm::vec3(), glm::vec3(slopeZ+0.000001f, slopeY, slopeX+0.000001f), glm::vec3(0.f,1.f,0.f)));
	//}
	//if(mAnimationDescription.animatePosition)
	//{
	//}
}

std::vector<glm::vec3>
SplineAnimation::debugSamplePath(float samplingRate) 
{
	std::vector<glm::vec3> geometry;
	/*
	for(int i = 0; i < mAnimationSamples.size(); ++i)
	{
			
		CurveSample* s0 = nullptr;
		CurveSample* s1 = &mAnimationSamples[i];
		CurveSample* s2 = nullptr;
		CurveSample* s3 = nullptr;
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
	}*/
	return geometry;
}