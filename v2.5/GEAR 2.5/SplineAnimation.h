// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "SplineAnimationSystem.h"

#include <G2Core/BaseComponent.h>
#include <glm/glm.hpp>
#include <vector>

namespace G2 
{
	namespace SplineInterpolationType
	{
		enum Name {
			CATMULL_ROM,
			BASIS_SPLINE,
			HERMITE_SPLINE
		};
	};

	struct AnimationDescription
	{
		AnimationDescription() : loops(true), interpolationType(SplineInterpolationType::CATMULL_ROM), animateOrientation(false), animatePosition(true) {}
		bool loops;											// Flag indicating whether the animation should loop.
		SplineInterpolationType::Name interpolationType;	// The interpolation type to use
		bool animateOrientation;
		bool animatePosition;
	};

	struct AnimationSample
	{
		AnimationSample() : speed(1.0) {}
		glm::vec3 point;			// The point of the animation sample
		double speed;				// The speed of the animation at this point.
	};

	struct SplineAnimationState
	{
		SplineAnimationState() : sampleIndex(0), interpolationValue(0.f) {}
		int		sampleIndex;			// The index of the currently active AnimationSample
		double	interpolationValue;	// The current interpolation value in the range [0,1]
	};
	class TransformComponent;
	/** This class defines the data for an animation using splines.
	 * @created:	2014/06/29
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class SplineAnimation : public BaseComponent<SplineAnimationSystem> 
	{
		public:
			/** Creates a new SplineAnimation with the given AnimationSamples and a description for the animation.
			 * @warning Currently only the SplineInterpolationType CATMULL_ROM works as expected!
			 * @param animationDescription The AnimationDescription containing general animation configuration.
			 * @param animationSamples The known samples of the animation to generate a SplineAnimation from.
			 */
			SplineAnimation(AnimationDescription const& animationDescription, std::vector<AnimationSample> const& animationSamples);
			/** This function will step the animation and write the new state into the given TransformComponent
			 * @param timeSinceLastFrame The step width to step the SplineAnimation.
			 * @param transformComponent The target TransformComponent to write the current state to.
			 */
			void animate(double const& timeSinceLastFrame, TransformComponent* transformComponent);
			/** This function will sample the complete spline with the given sampling rate and write all sampled points into 
			 * a returned vector.
			 * @param samplingRate The sampling rate to sample the spline with.
			 * @return The sampled points from the spline.
			 */
			std::vector<glm::vec3> debugSamplePath(float samplingRate);
		private:

			AnimationDescription			mAnimationDescription;
			std::vector<AnimationSample>	mAnimationSamples;

			SplineAnimationState			mCurrentState;
			glm::mat4						mInterpolationMatrix;
	};
};