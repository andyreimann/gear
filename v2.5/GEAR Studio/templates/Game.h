#pragma once
#include "Defines.h"
#include <string>
#include <memory>
#include <unordered_map>

#include <G2/GEAR.h>

namespace G2
{
	/** This singleton class defines an environment to access the currently
	 * loaded scene objects and to load new scenes.
	 * @created	2014/12/11
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
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
			/** Schedules the async loading of a scene before the next frame is going to be rendered. 
			 * @param name The name of the scene to load.
			 */
			static void startSceneAsync(std::string const& name);
			/** Checks if an entity is registered with the given name.
			 * @param name The name of the entity to check existence for.
			 * @return True if an entity exists, false if not.
			 */
			bool hasEntity(std::string const& name) const;
			/** Returns a pointer to an entity registered with the given name.
			 * @param name The name of the Entity to get a pointer for.
			 * @return A pointer to the entity or nullptr, if no entity is registered for the given name.
			 * @warning Never assign the returned pointer to an instance of a G2::Entity, since this would invoke
			 * the move assignment operator and therefore corrupt the internal managed entities!
			 */
			G2::Entity* getEntity(std::string const& name);
			~Game();

		private:
			// don't allow instances
			Game();
			Game(Game const&) {}
			Game& operator=(Game const&) { return *this; }

			void _scheduleSceneLoading(std::string const& sceneName);
			void _onRenderFrame(G2::FrameInfo const&);

			void _auto_generated_loadCurrentScene();

			static std::shared_ptr<Game>	mInstance_;		// The one single instance

			std::string mCurrentScene;
			bool 		mLoadScene;

			std::unordered_map<std::string,G2::Entity> 	mManagedEntities;
	};
};