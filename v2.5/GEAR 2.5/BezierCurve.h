#pragma once
#include "Curve.h"

namespace G2 
{
	/** This class implements a CurveInterpolator using bezier interpolation
	 * @created:	2014/07/01
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class BezierCurve : public Curve
	{
		public:
			/** Constructs a new BezierCurve using the given curve samples.
			 * @note In order to provide enough informations for the bezier spline, 
			 * you have to provide the points and speed for the CurveSample objects.
			 * Also keep in mind that you have to provide at least 4 samples!
			 * @param curveSamples The samples of the bezier curve.
			 */
			BezierCurve(InterpolationDescription const& interpolationDescription, std::vector<CurveSample> const& curveSamples);
			
			virtual void interpolate(double const& timeStep) override;
			
			virtual bool usePoints() override;
			virtual bool useTangents() override;
	};
};