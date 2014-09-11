// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "EventDistributer.h"

using namespace G2;

// INPUT EVENTS
Event<G2::KeyCode>									EventDistributer::onKeyUp;
Event<G2::KeyCode>									EventDistributer::onKeyDown;
Event<MouseButton,glm::detail::tvec2<int> const&>	EventDistributer::onMouseUp;
Event<MouseButton,glm::detail::tvec2<int> const&>	EventDistributer::onMouseDown;
Event<glm::detail::tvec2<int> const&>				EventDistributer::onMouseMove;
Event<int>											EventDistributer::onMouseWheel; 
			
// RENDER CONTEXT EVENTS
Event<int,int>										EventDistributer::onViewportResize;
Event<FrameInfo const&>								EventDistributer::onRenderFrame;
Event<FrameInfo const&>								EventDistributer::onFrameRendered;
