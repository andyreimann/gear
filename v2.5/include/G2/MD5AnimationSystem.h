// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "BaseSystem.h"

namespace G2
{
	class RenderSystem;
	class RenderComponent;
	class MD5AnimationComponent;
	/** This class is responsible for the animation of the loaded MD5AnimationComponents.
	 * The animation itself takes place in the 'postUpdate' phase during rendering.
	 * @created:	2014/01/30
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class MD5AnimationSystem : public BaseSystem<MD5AnimationSystem,MD5AnimationComponent> 
	{
		public:

			MD5AnimationSystem() 
				: mRenderSystem(nullptr) 
			{}

			void runPhase(std::string const& name, FrameInfo const& frameInfo);

		private:

			bool animate(MD5AnimationComponent* component, FrameInfo const& frameInfo);
			void interpolateFrames(MD5AnimationComponent* component, float interpolationFactor);
			void updatePose(MD5AnimationComponent* animationComponent, RenderComponent* renderComponent);

			RenderSystem*	mRenderSystem;
	};
};