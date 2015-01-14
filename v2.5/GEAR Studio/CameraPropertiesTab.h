#pragma once
#include "ui_CameraPropertiesTab.h"
#include "PropertiesTab.h"

#include <G2/EffectImporter.h>
#include <G2/FBXImporter.h>

#include <QtWidgets/QWidget>


/** This class implements the ManagedEntity property regarding cameras.
* @created	2015/01/14
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class CameraPropertiesTab : public QWidget, public PropertiesTab
{
	Q_OBJECT
	public:
		CameraPropertiesTab(QWidget *parent = 0);

		~CameraPropertiesTab();

	protected:
		/** This callback function is called whenever the PropertiesTab
		 * should be initialized with the given ManagedEntity.
		 * @param entity A pointer to the entity to initialize the PropertiesTab with. This pointer might be a nullptr in case no ManagedEntity has focus!
		 * @param properties A pointer to the properties of the given entity in regards to this PropertiesTab or a nullptr in case entity is a nullptr.
		 * @note the given pointer entity is the exact same one as the mEntity member variable.
		 */
		virtual void _initUiWithEntity(ManagedEntity* entity) override;
		/** This is a callback function which will be invoked whenever a new ManagedEntity is created and should be instantiated according to the contained entityDesc.
		 * @param entity A pointer to the ManagedEntity which is selected.
		 * @note Call the entity->getEntityDescription() to get the description.
		 */
		virtual void _instantiateFromDescription(ManagedEntity* entity) override;
	private slots:
		/** Toggles the folding state of the tab.
		 */
		void toggleTab();

		void zNearChanged(double value);
		void zFarChanged(double value);
		void fieldOfViewChanged(double value);
		void mainCameraFlagChanged(int state);
		void nameChanged(QString const& name);

	private:

		void _onGenerateCppCodeForManagedEntity(ManagedEntity const* entity, std::string const& entityVar, std::ostream& out);

		bool					mOpen;				// The flag if the tab is opened or not.
		Ui::CameraPropertiesTab	ui;					// The Qt UI class for the project creation dialog

		G2::EffectImporter  mEffectImporter;			// The effect importer used for the editor geometry
		G2::FBXImporter mFbxImporter;
};
