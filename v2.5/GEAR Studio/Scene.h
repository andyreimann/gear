#pragma once
#include "JsonSerializer.h"
#include "JsonDeserializer.h"
#include "ManagedEntity.h"

#include <G2/EffectImporter.h>
#include <G2/FBXImporter.h>

#include <unordered_map>


/** This class holds all information about a Scene.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class Scene : public JsonDeserializer, JsonSerializer
{
	public:
		/** This will create a new Scene instance from a given scene file.
		 * @param projectDirectory The project directory to load.
		 * @param sceneFile The scene file to load.
		 */
		Scene(std::string projectDirectory, std::string const& sceneFile);
		std::string getName() const;
		/** Loads the Scene into the editor creating all GEAR engine resources.
		 */
		void load();
		/** Saves the Scene in it's current state into it's Json file.
		 */
		void save();
		/** This function will try to create a new entity in the scene with the given name.
		 * @param name The name of the entity to create.
		 * @return A pointer to the newly created entity or nullptr, if an entity with the given name already exists.
		 */
		ManagedEntity* createNewEntity(std::string const& name);
		/** Returns a pointer to a ManagedEntity of the Scene.
		 * @param name The name of the ManagedEntity to get a pointer for.
		 * @return A pointer to the ManagedEntity if one could be find, nullptr otherwise.
		 */
		ManagedEntity* get(std::string const& name);
	private:

		void _init3D();
		void _initEntityFromJson(Json::Value const& entityDesc);
		void _initComponentFromJson(Json::Value const& componentDesc, ManagedEntity* entity);

		std::string mProjectDirectory;
		std::unordered_map<std::string, ManagedEntity> mLoadedEntities;

		// importer 
		G2::EffectImporter			mEffectImporter;
		G2::FBXImporter				mFbxImporter;
};
