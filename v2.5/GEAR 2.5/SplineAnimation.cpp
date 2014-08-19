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
	: mCurve(curve),
	mPaused(false)
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
	return mCurve->getPointsOnCurve(samplingRate);
}