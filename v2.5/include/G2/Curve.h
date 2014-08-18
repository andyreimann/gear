#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace G2 
{
	/** This struct holds all informations describing a sample of a curve.
	 * It can be used to create different kind of Curve objects.
	 * @note Normally the transition from one CurveSample to the next one is performed within one second (speed=1.0),
	 * however you can change this behavior by just assigning a distinct value. Higher values will result in faster movement and vice versa.
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
			/** Creates a new abstract Curve with the informations about how to perform interpolation and which samples to use.
			 * @param interpolationDescription The description for the interpolation to perform.
			 * @param curveSamples The samples of the abstract Curve.
			 */
			Curve(InterpolationDescription const& interpolationDescription, std::vector<CurveSample> const& curveSamples)
				: mInterpolationDesc(interpolationDescription),
				mCurveSamples(curveSamples), 
				mCurrentIndex(0), 
				mInterpolationValue(0.),
				mSample0(nullptr),
				mSample1(nullptr),
				mSample2(nullptr),
				mSample3(nullptr) {}

			/** This function steps the Curve forward and calculates the position and rotation accordingly.
			 * @param timeStep The time step to forward the Curve.
			 * 
			 */
			virtual void interpolate(double const& timeStep);
			/** Checks if the specialized Curve implementation uses the given points of the CurveSample objects.
			 * @return True if the specialized Curve implementation uses the given points of the CurveSample objects, false if not.
			 */
			virtual bool usePoints() = 0;
			/** Checks if the specialized Curve implementation uses the given tangents of the CurveSample objects.
			 * @return True if the specialized Curve implementation uses the given tangents of the CurveSample objects, false if not.
			 */
			virtual bool useTangents() = 0;

			/** This function will sample the complete curve with the given sampling rate and write all sampled points into 
			 * a returned vector.
			 * @param samplingRate The sampling rate to sample the curve with.
			 * @return The sampled points from the spline.
			 */
			std::vector<glm::vec3> getPointsOnCurve(float samplingRate);

			CurveSample const* getCurrentSample() const { return &mCurveSamples[mCurrentIndex]; }
			glm::vec3 const& getPosition() const { return mCurrentPosition; }
			glm::mat4 const& getRotation() const { return mCurrentRotation; }
			
	protected:

			static void _interpolate(
				glm::mat4 const& matrix,
				glm::vec4 const& geometry,
				float ip,
				float& valueOut,
				float& slopeOut
			);

			InterpolationDescription			mInterpolationDesc;		// The description of the interpolation to perform
			std::vector<CurveSample>			mCurveSamples;			// The samples of this curve
			
			CurveSample const*					mSample0;
			CurveSample const*					mSample1;
			CurveSample const*					mSample2;
			CurveSample const*					mSample3;
			
			int									mCurrentIndex;			// The current index of the animation
			double								mInterpolationValue;	// The current interpolation value of the animation [0.0,1.0]
			glm::vec3							mCurrentPosition;		// The current interpolated position on the Curve if calculated
			glm::mat4							mCurrentRotation;		// The current interpolated rotation on the Curve if calculated
	};
};