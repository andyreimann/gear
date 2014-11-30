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
	 * It holds several engine internal Event objects for different G2Core features, 
	 * everyone can attach itself to.
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
			
			COREDLL_API static Event<FrameInfo const&>								onRenderFrame;
			COREDLL_API static Event<FrameInfo const&>								onFrameRendered;
			COREDLL_API static Event<std::string const&, FrameInfo const&>			onPhaseStarted;
			COREDLL_API static Event<std::string const&, FrameInfo const&>			onPhaseEnded;
		private:

			

			COREDLL_API EventDistributer() {}
			COREDLL_API ~EventDistributer() {}


	};

};
