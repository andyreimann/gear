// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "KeyCodes.h"
#include "MouseButton.h"
#include "Event.h"
#include "FrameInfo.h"

#include <glm/glm.hpp>
#include <string>

namespace G2 
{
	/** This class can distribute all global Events.
	 */
	class EventDistributer 
	{
		friend class RenderController;

		public:
			
			// INPUT EVENTS
			COREDLL_API static Event<G2::KeyCode>									onKeyUp;
			COREDLL_API static Event<G2::KeyCode>									onKeyDown;
			COREDLL_API static Event<MouseButton,glm::detail::tvec2<int> const&>	onMouseUp;
			COREDLL_API static Event<MouseButton,glm::detail::tvec2<int> const&>	onMouseDown;
			COREDLL_API static Event<glm::detail::tvec2<int> const&>				onMouseMove;
			COREDLL_API static Event<int>											onMouseWheel;
			
			// RENDER CONTEXT EVENTS
			COREDLL_API static Event<int,int>										onViewportResize;
			

			/**
			 * Because
			 */
			COREDLL_API static Event<std::string const&>							terraFormingModuleChanged;		//!< The Event emittd when a new terra forming module was selected
			
			COREDLL_API static Event<FrameInfo const&>								onRenderFrame;
			COREDLL_API static Event<FrameInfo const&>								onFrameRendered;
		private:

			

			COREDLL_API EventDistributer() {}
			COREDLL_API ~EventDistributer() {}


	};

};
