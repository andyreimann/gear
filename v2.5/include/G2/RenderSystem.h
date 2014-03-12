// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "BaseSystem.h"
#include "UberShader.h"

#include <memory>
#include <vector>

namespace G2 
{
	class RenderComponent;
	class CameraComponent;
	/** This class defines the whole render pipeline of the GEAR engine.
	 * It renders all registered RenderComponents with their settings.
	 * The rendering takes place in the 'render' phase.
	 * @created:	2014/01/30
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class RenderSystem : public BaseSystem<RenderSystem,RenderComponent> 
	{
		public:
			void runPhase(std::string const& name, FrameInfo const& frameInfo);
			/** This function will return the DefaultShader. 
			* @return The current DefaultShader.
			*/
			UberShader const& getDefaultShader() const { return defaultUberShader; }
			/** This function will set the DefaultShader to the given value.
			* @param value The current DefaultShader.
			*/
			void setDefaultShader(UberShader const& value) { defaultUberShader = value; }

		private:
			
			
			UberShader						defaultUberShader;		// The default UberShader to use for rendering
	};
};