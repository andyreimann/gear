// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include <G2Core/BaseSystem.h>

namespace G2
{
	class SplineAnimation;
	/** This class defines the system animating all SplineAnimations contained in a scene.
	 * @created:	2014/01/30
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class SplineAnimationSystem : public BaseSystem<SplineAnimationSystem,SplineAnimation> 
	{
		public:

			void runPhase(std::string const& name, FrameInfo const& frameInfo);
	};
};