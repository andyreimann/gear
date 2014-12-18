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

		void load();
		/** This function will try to create a new entity in the scene with the given name.
		 * @param name The name of the entity to create.
		 * @return A pointer to the newly created entity or nullptr, if an entity with the given name already exists.
		 */
		ManagedEntity* createNewEntity(std::string const& name);
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
