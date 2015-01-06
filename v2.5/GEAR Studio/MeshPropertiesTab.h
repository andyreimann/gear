#pragma once
#include "ui_MeshPropertiesTab.h"
#include "PropertiesTab.h"

#include <G2/FBXImporter.h>
#include <G2/MD5Importer.h>

#include <QtWidgets/QWidget>


/** This class implements the ManagedEntity property regarding meshes.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class MeshPropertiesTab : public QWidget, public PropertiesTab
{
	Q_OBJECT
	public:
		MeshPropertiesTab(QWidget *parent = 0);

		~MeshPropertiesTab();

	protected:
		/** This callback function is called whenever the PropertiesTab
		 * should be initialized with the given ManagedEntity.
		 * @param entity A pointer to the entity to initialize the PropertiesTab with. This pointer might be a nullptr in case no ManagedEntity has focus!
		 * @param properties A pointer to the properties of the given entity in regards to this PropertiesTab or a nullptr in case entity is a nullptr.
		 * @note the given pointer entity is the exact same one as the mEntity member variable.
		 */
		virtual void _initWithEntity(ManagedEntity* entity) override;
		/** This is a callback function which will be invoked whenever a new ManagedEntity is created and should be instantiated according to the contained entityDesc.
		 * @param entity A pointer to the ManagedEntity which is selected.
		 * @note Call the entity->getEntityDescription() to get the description.
		 */
		virtual void _instantiateFromDescription(ManagedEntity* entity) override;
	private slots:
		/** Toggles the folding state of the tab.
		 */
		void toggleTab();
		/** Openes a file select dialog to select the permitted mesh types.
		 * After the selection, the mesh is tried to be loaded and assigned to the current ManagedEntity if one is set.
		 */
		void selectMesh();

	private:
		/** Reimports the given target ManagedEntity according to it's document description.
		 * This function will reimport the mesh with the defined settings.
		 * Make sure to have all settings saved in the description inside the ManagedEntity before you call this function.
		 */
		void _reimportMesh(ManagedEntity* target);

		G2::FBXImporter			mFbxImporter;		// The importer for FBX-Meshes to use.
		G2::MD5Importer			mMd5Importer;		// The importer for MD5-Meshes to use.
		bool					mOpen;				// The flag if the tab is opened or not.
		Ui::MeshPropertiesTab	ui;					// The Qt UI class for the project creation dialog
};
