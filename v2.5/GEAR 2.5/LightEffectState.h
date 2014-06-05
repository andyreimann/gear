#pragma once
#include "ShadowDescriptor.h"

#include <glm/glm.hpp>
#include <vector>

namespace G2 
{
	/// This class defines...
	/// @created:	2014/06/05
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	struct LightEffectState 
	{
		/// This constructs a new LightEffectState.
		LightEffectState()
			: shadowTechnique(AdvancedShadowTechnique::NO_SHADOWS),
			cascades(0),
			splitWeight(0.f),
			splitDistFactor(0.f)
		{}


		// GENERAL
		AdvancedShadowTechnique::Name shadowTechnique;

		// EFFECT STATES FOR CSM
		unsigned int			cascades;				// The number of cascades to use (not needed for every type of shadow mapping implemented). Default value is 0
		float					splitWeight;			// This factor tells where the hole Frustum has to be split. A higher value will split the Frustum near the Camera and a lower value will split the frustum farer away from the Camera
		float					splitDistFactor;		// This factor tells how many the single cascades will overlap each other
		std::vector<float>		nearClips;				// This array contains the newarPlane for each slice frustum
		std::vector<float>		farClips;				// This array contains the far plane for each slice frustum
		std::vector<float>		farClipsHomogenous;		// This array contains the far plane for each slice frustum in camera homogeneous coordinates
		std::vector<glm::vec4>	frustumPoints;			// The frustum points of all slice frustum
		std::vector<glm::mat4>	eyeToLightClip;			// This is the full shadow transformation matrix = cropmatrix * projection * modelview
		// EFFECT STATES FOR CSM END
	};
};