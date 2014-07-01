#pragma once
#include "Curve.h"

namespace G2 
{
	/** This class implements a CurveInterpolator using catmull rom interpolation
	 * @created:	2014/07/01
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class CatmullRomCurve : public Curve
	{
		public:
			/** Constructs a new CatmullRomCurve using the given curve samples.
			 * @note In order to provide enough informations for the catmull rom spline, 
			 * you only have to provide the points and speed for the CurveSample objects.
			 * Also keep in mind that you have to provide at least 4 point samples!
			 * @param curveSamples The samples of the catmull rom curve.
			 */
			CatmullRomCurve(InterpolationDescription const& interpolationDescription, std::vector<CurveSample> const& curveSamples);
			
			virtual void interpolate(double const& timeStep) override;
			
			virtual bool usePoints() override;
			virtual bool useTangents() override;
		protected:

			static void _catmullRom(
				glm::vec4 const& geometry,
				float ip,
				float& valueOut,
				float& slopeOut
			);
	};
};