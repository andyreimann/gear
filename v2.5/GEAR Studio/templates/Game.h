#pragma once
#include "Defines.h"
#include <string>
#include <memory>
#include <unordered_map>

#include <G2/GEAR.h>
#include <G2/FBXImporter.h>
#include <G2/MD5Importer.h>
#include <G2/EffectImporter.h>
#include <G2/TextureImporter.h>

namespace G2
{
	/** This singleton class defines an environment to access the currently
	 * loaded scene objects and to load new scenes.
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

			std::string 			mProjectRoot;		// The project root directory not ending with a slash
			G2::FBXImporter			mFbxImporter;		// The importer for FBX-Meshes to use.
			G2::MD5Importer			mMd5Importer;		// The importer for MD5-Meshes to use.
			G2::EffectImporter		mFxImporter;		// The importer for Effects to use.
			G2::TextureImporter		mTexImporter;		// The importer for textures to use.
	};
};