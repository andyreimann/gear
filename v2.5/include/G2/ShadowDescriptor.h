#pragma once

#include "Frustum.h"
#include "Logger.h"
#include <glm/glm.hpp>

#include <vector>


namespace G2 
{
	/** An AdvancedShadowTechnique indicates the engine, which exact technique to use while rendering
	 * to create the appropriate shadow rendering. Since every different shadowing algorithm is very different, 
	 * GEAR implements a certain amount of shadowing algorithms directly.
	 */
	namespace AdvancedShadowTechnique
	{
		enum Name {
			CASCADED_SHADOW_MAPS = 0,
			NUM_TECHNIQUES,
			NO_SHADOWS = -1,
		};
	};
	/// Describes the configuration of shadows and how they behave at runtime.
	/// @created:	2014/06/02
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class ShadowDescriptor 
	{
			friend class LightComponent;
			friend class RenderSystem;
		public:
			/** Creates a new ShadowDescriptor, which effectively disables the shadowing.
			 */
			static ShadowDescriptor noShadows();
			/** Creates a new ShadowDescriptor describing a Cascaded Shadow Mapping algorithm to use.
			 * @param numCascades The number of cascades to use while rendering. The allowed range is [1,4].
			 * @param customEffect A custom Effect file to use. If no file is given, the internal predefined Effect is used.
			 */
			static ShadowDescriptor cascadedShadowMaps(unsigned int numCascades, std::string const& customEffect);
		private:
			/// This constructs a new ShadowDescriptor.
			ShadowDescriptor(AdvancedShadowTechnique::Name technique);
		
			AdvancedShadowTechnique::Name mTechnique;	// The shadow technique to use
			std::string	 mCustomEffect;					// The path to a custom effect to load and use for rendering the shadows
			unsigned int mNumCascades;					// The number of cascades to use (not needed for every type of shadow mapping implemented). Default value is 0
			
			float mSplitWeight;							// This factor tells where the hole Frustum has to be split. A higher value will split the Frustum near the Camera and a lower value will split the frustum farer away from the Camera
			float mSplitDistFactor;						// This factor tells how many the single cascades will overlap each other
	};
};