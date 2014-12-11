#pragma once
#include "Defines.h"
#include <string>
#include <memory>

#include <G2/GEAR.h>

namespace G2
{
	class Game
	{
		public:

			/** Get a reference to one single instance.
			 * @return A reference of one single instance.
			 */
			static Game& getShared();
			/** Destroys the one single instance.
			 */
			static void destroy();


			static void startSceneAsync(std::string const& name);

		private:
			// don't allow instances
			Game();
			Game(Game const&) {}
			Game& operator=(Game const&) { return *this; }
			~Game();

			void _scheduleSceneLoading(std::string const& sceneName);
			void _onRenderFrame(G2::FrameInfo const&);


			static std::shared_ptr<Game>	mInstance_;		// The one single instance

			std::string mCurrentScene;
			bool 		mLoadScene;
	};
};