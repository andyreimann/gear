// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "AbstractWindow.h"

#include <G2Core/KeyCodes.h>

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
			/// This constructs a new SDLWindow.
			/// @param title The title of the SDLWindow
			/// @param width The width of the SDLWindow
			/// @param heightThe width of the SDLWindow
			/// @param hideMouse the Flag for hiding the mouse
			SDLWindow(std::string const& title, unsigned int width, unsigned int height, bool hideMouse);
			/** This function triggers the event processing of one single frame.
			 */
			virtual void processEvents(int frame) override;

			virtual void swapBuffer() override;

			virtual void setHideMouseMode(bool mode) override;
			/** normal destructor
			 */
			~SDLWindow();
		private:
			/// this function should be called whenever the hide mouse state is changed.
			void initHideMouseState();

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

			glm::detail::tvec2<int>	mMousePosition;

			// NSIGHT SUPPORT
			bool		mZDown;
			bool		mCtrlDown;
			bool		mNsightActive;
	};
};