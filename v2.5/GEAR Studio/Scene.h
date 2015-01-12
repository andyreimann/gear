#pragma once
#include "JsonSerializer.h"
#include "JsonDeserializer.h"
#include "ManagedEntity.h"

#include <G2/EffectImporter.h>

#include <unordered_map>
#include <fstream>

class QProgressDialog;
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
		/** Returns the name of the Scene.
		 * @return The name of the Scene.
		 */
		std::string getName() const;
		/** Loads the Scene into the editor creating all GEAR engine resources.
		 */
		void load();
		/** Saves the Scene in it's current state into it's Json file.
		 */
		void save();
		/** This function will generate the C++ code to instantiate the entities contained in the Scene.
		 * @param out The file stream to write the code to.
		 * @param progress The instance to a progress dialog.
		 */
		void generateEntityInitializationCode(std::ofstream& out, QProgressDialog* progress) const;
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
		/** Returns a pointer to a ManagedEntity of the Scene.
		* @param entityId The Entity ID of the ManagedEntity to get a pointer for.
		* @return A pointer to the ManagedEntity if one could be find, nullptr otherwise.
		*/
		ManagedEntity* get(unsigned int entityId);
	private:

		void _init3D();
		void _initEntityFromJson(Json::Value const& entityDesc);
		
		std::string mProjectDirectory;
		std::unordered_map<std::string, ManagedEntity> mLoadedEntities;
		std::unordered_map<unsigned int, std::string> mLoadedEntitiesIdToNameMapping;

		// importer 
		G2::EffectImporter			mEffectImporter;
};
