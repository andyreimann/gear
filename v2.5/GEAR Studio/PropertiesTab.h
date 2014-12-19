#pragma once
#include <string>
#include "Project.h"

/** This class is the base of every properties tab, which is displayed in the GEAR Editor
* on the right side. A PropertiesTab provides functionality which can be attached to ManagedEntities
* in the GEAR Studio.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class PropertiesTab
{
	public:
		/** Creates a new PropertiesTab
		 * @param technicalName The technical name of this PropertiesTab.
		 * The technical name of a PropertiesTab is used when serializing/deserializing a PropertiesTabs
		 * settings into/from JSON.
		 */
		PropertiesTab(std::string const& technicalName);

		~PropertiesTab();

	private:
		/** This is a callback function which will be invoked whenever a new ManagedEntity is selected/gains focus.
		 * @param entity A pointer to the ManagedEntity which is selected.
		 */
		void _onManagedEntitySelected(ManagedEntity* entity);

		/** This is a callback function which will be invoked whenever a new ManagedEntity is created and should be instantiated according to the given entityDesc.
		* @param entity A pointer to the ManagedEntity which is selected.
		* @param desc The entity description containing it's name, properties and so on.
		*/
		void _onDeserializeManagedEntity(ManagedEntity* entity, Json::Value const& desc);

		/** This is a callback function which will be invoked whenever a new Project was opened.
		* @param project A pointer to the Project which was opened.
		*/
		void _onProjectOpened(Project* project);

	protected:
		/** Checks whether the PropertiesTab has a current ManagedEntity.
		 * @return True if a current ManagedEntity is available, false if not.
		 */
		bool hasEntity() const { return mEntity != nullptr; }
		/** This callback function is called whenever the PropertiesTab
		 * should be initialized with the contained description.
		 * @param entity A pointer to the entity to initialize the PropertiesTab with. This pointer might be a nullptr in case no ManagedEntity has focus!
		 * @note the given pointer entity is the exact same one as the mEntity member variable.
		 */
		virtual void _initWithEntity(ManagedEntity* entity) = 0;
		/** This is a callback function which will be invoked whenever a new ManagedEntity is created and should be instantiated according to the contained entityDesc.
		* @param entity A pointer to the ManagedEntity which is selected. 
		* @note Call the entity->getEntityDescription() to get the description.
		*/
		virtual void _instantiateFromDescription(ManagedEntity* entity) = 0;

		ManagedEntity*	mEntity;			// A pointer to the currently focused entity, might be nullptr - is automatically updated whenever a new ManagedEntity gets a focus
		std::string		mTechnicalName;		// The technical name of this PropertiesTab
		std::string		mProjectDirectory;	// The directory of the currently loaded project - is automatically updated whenever a new Project is loaded
};
