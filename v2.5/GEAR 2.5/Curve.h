#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace G2 
{
	/** This struct holds all informations describing a sample of a curve.
	 * It can be used to create different kind of Curve objects.
	 */
	struct CurveSample
	{
		CurveSample() : speed(1.0) {}
		glm::vec3	point;				// The point of the curve sample
		glm::vec3	tangent;			// The tangent of the curve sample
		double		speed;				// The speed of the animation at this curve sample.
	};

	

	struct InterpolationDescription
	{
		InterpolationDescription() : loops(true), animateOrientation(false), animatePosition(true) {}
		bool loops;											// Flag indicating whether the animation should loop.
		bool animateOrientation;
		bool animatePosition;
	};

	/** This class defines the interface for a curve in 3 dimensional space.
	 * @created:	2014/07/01
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class Curve
	{
		public:
			Curve(InterpolationDescription const& interpolationDescription, std::vector<CurveSample> const& curveSamples)
				: mInterpolationDesc(interpolationDescription),
				mCurveSamples(curveSamples), 
				mCurrentIndex(0), 
				mInterpolationValue(0.0) {}

			virtual void interpolate(double const& timeStep) = 0;
			virtual bool usePoints() = 0;
			virtual bool useTangents() = 0;

			CurveSample const* getCurrentSample() const { return &mCurveSamples[mCurrentIndex]; }
			glm::vec3 const& getPosition() const { return mCurrentPosition; }
			glm::mat4 const& getRotation() const { return mCurrentRotation; }
			
	protected:

			InterpolationDescription			mInterpolationDesc;		// The description of the interpolation to perform
			std::vector<CurveSample>			mCurveSamples;			// The samples of this curve

			
			int									mCurrentIndex;			// The current index of the animation
			double								mInterpolationValue;	// The current interpolation value of the animation [0.0,1.0]
			glm::vec3							mCurrentPosition;		// The current interpolated position on the Curve if calculated
			glm::mat4							mCurrentRotation;		// The current interpolated rotation on the Curve if calculated
	};
};