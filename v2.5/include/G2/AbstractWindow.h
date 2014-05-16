// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "TimeTracker.h"

#include <G2Core/Event.h>
#include <G2Core/NonCopyable.h>

#include <string>

namespace G2 
{
	/** This abstract class provides an interface for a window.
	 * A window class is needed to inject the window and input related events into the engine.
	 * Also it may be responsible for the main loop of the rendering.
	 * AbstractWindows and all of it's derivations are only responsible for managing their internal events and 
	 * emitting them into the appropriate Events in the EventDistributor.
	 * Connect to these events to also receive updates on these Events.
	 * @created:	2014/02/17
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class AbstractWindow : public NonCopyable
	{
		public:
			/** This constructs a new AbstractWindow with the given parameters.
			 * @param width the width of the window
			 * @param height The height of the window
			 * @param title The title of the window
			 */
			AbstractWindow(unsigned int width, unsigned int height, std::string const& title);
			/** This function will return the Width. 
			* @return The current Width.
			*/
			unsigned int const& getWidth() const { return mWidth; }
			/** This function will return the Height. 
			* @return The current Height.
			*/
			unsigned int const& getHeight() const { return mHeight; }
			/** This function will return the Title. 
			* @return The current Title.
			*/
			std::string const& getTitle() const { return mTitle; }
			/** This function triggers the rendering of one single frame.
			 * @return False if the rendering should be stopped, true if not.
			 */
			virtual bool renderSingleFrame() = 0;
		protected:

			unsigned int		mWidth;
			unsigned int		mHeight;
			std::string			mTitle;

			TimeTracker			mFrameTimer;
			TimeTracker			mRenderTimer;

		private:
			AbstractWindow(AbstractWindow const& rhs) {}
			AbstractWindow& operator=(AbstractWindow const& rhs) { return *this; }
	};
};