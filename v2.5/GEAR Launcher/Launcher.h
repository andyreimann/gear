#pragma once
#include "GameRuntime.h"

#include <G2/GEAR.h>
#include <G2SdlContext/SDLWindow.h>

namespace G2
{
	class Launcher
	{
		public:
			Launcher(G2::SDL::Window& window);
			~Launcher();
		private:

			G2::SDL::Window&			mWindow;

			GameRuntime					mGameRuntime;
	};
};
