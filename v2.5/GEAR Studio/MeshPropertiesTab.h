#pragma once
#include "ui_MeshPropertiesTab.h"
#include "PropertiesTab.h"
#include <QtWidgets/QWidget>


/** This is the main class of the GEAR Editor Studio.
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
		virtual void _initWithEntity(ManagedEntity* entity, Json::Value* properties) override;
		/** This is a callback function which will be invoked whenever a new ManagedEntity is created and should be instantiated according to the contained entityDesc.
		 * @param entity A pointer to the ManagedEntity which is selected.
		 * @note Call the entity->getEntityDescription() to get the description.
		 */
		virtual void _instantiateFromDescription(ManagedEntity* entity) override;
	private slots:

		void toggleTab();
		void selectMesh();

	private:

		bool mOpen;
		Ui::MeshPropertiesTab ui;					// The Qt UI class for the project creation dialog
};
