// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "SplineAnimationSystem.h"

#include <G2Core/BaseComponent.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

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

	struct SplineAnimationState
	{
		SplineAnimationState() : sampleIndex(0), interpolationValue(0.f) {}
		int		sampleIndex;			// The index of the currently active AnimationSample
		double	interpolationValue;	// The current interpolation value in the range [0,1]
	};
	class TransformComponent;
	class Curve;
	/** This class defines the data for an animation using splines.
	 * @created:	2014/06/29
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class SplineAnimation : public BaseComponent<SplineAnimationSystem> 
	{
		public:
			/** Creates a new SplineAnimation with the given 
			 * @param curve The Curve to use for the animation.
			 */
			SplineAnimation(std::shared_ptr<Curve> const& curve);
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
			std::shared_ptr<Curve>	mCurve;
	};
};