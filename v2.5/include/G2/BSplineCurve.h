#pragma once
#include "Curve.h"

namespace G2 
{
	/** This class implements a CurveInterpolator using B-Spline interpolation
	 * @created:	2014/07/01
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class BSplineCurve : public Curve
	{
		public:
			/** Constructs a new BSplineCurve using the given curve samples.
			 * @note In order to provide enough informations for the B-Spline, 
			 * you have to provide the points and speed for the CurveSample objects.
			 * Also keep in mind that you have to provide at least 4 samples!
			 * @param curveSamples The samples of the B-Spline curve.
			 */
			BSplineCurve(InterpolationDescription const& interpolationDescription, std::vector<CurveSample> const& curveSamples);
			/** This function steps the Curve forward and calculates the position and rotation accordingly.
			 * @param timeStep The time step to forward the Curve.
			 */
			virtual void interpolate(double const& timeStep) override;
			/** Checks if the specialized Curve implementation uses the given points of the CurveSample objects.
			 * @return True if the specialized Curve implementation uses the given points of the CurveSample objects, false if not.
			 */
			virtual bool usePoints() override;
			/** Checks if the specialized Curve implementation uses the given tangents of the CurveSample objects.
			 * @return True if the specialized Curve implementation uses the given tangents of the CurveSample objects, false if not.
			 */
			virtual bool useTangents() override;
	};
};