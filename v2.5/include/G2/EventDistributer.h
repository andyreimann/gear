// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"
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
		friend class Editor;
		friend class RenderController;

		public:
			
			// INPUT EVENTS
			static Event<G2::KeyCode>									onKeyUp;
			static Event<G2::KeyCode>									onKeyDown;
			static Event<MouseButton,glm::detail::tvec2<int> const&>	onMouseUp;
			static Event<MouseButton,glm::detail::tvec2<int> const&>	onMouseDown;
			static Event<glm::detail::tvec2<int> const&>				onMouseMove;
			
			// RENDER CONTEXT EVENTS
			static Event<int,int>										onViewportResize;
			

			/**
			 * Because
			 */
			static Event<std::string const&>							terraFormingModuleChanged;		//!< The Event emittd when a new terra forming module was selected
			
			static Event<FrameInfo const&>								onRenderFrame;
		private:

			

			EventDistributer() {}
			~EventDistributer() {}


	};

};
