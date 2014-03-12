// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "AbstractWindow.h"
#include "KeyCodes.h"
#include "FrameInfo.h"

#include <glm/glm.hpp>
#include <SDL/SDL.h>

namespace G2 
{
	/** This class defines...
	 * @created:	2014/02/17
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class SDLWindow : public AbstractWindow
	{
		public:
			/** This constructs a new SDLWindow.
			 */
			SDLWindow(unsigned int width, unsigned int height, std::string const& title);
			/** This function triggers the rendering of one single frame.
			 * @return False if the rendering should be stopped, true if not.
			 */
			virtual bool renderSingleFrame() override;
			/** normal destructor
			 */
			~SDLWindow();
		private:

			/** This function receives callbacks from the EventDistributer::onKeyUp Event
			 * @note It only receives callbacks if the EditableScene is prepared for editing.
			 */
			void onKeyUp(G2::KeyCode keyCode);
			/** This function receives callbacks from the EventDistributer::onKeyDown Event
			 * @note It only receives callbacks if the EditableScene is prepared for editing.
			 */
			void onKeyDown(G2::KeyCode keyCode);

			SDL_Window* mSDLWindow;
			SDL_GLContext mMainContext;
			FrameInfo	mFrameInfo;

			glm::detail::tvec2<int>	mMousePosition;

			// NSIGHT SUPPORT
			bool		mZDown;
			bool		mCtrlDown;
			bool		mNsightActive;
	};
};