#pragma once
#include "ui_TransformationPropertiesTab.h"
#include "PropertiesTab.h"

#include <QtWidgets/QWidget>

/** This class implements the ManagedEntity property regarding the transformation.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class TransformationPropertiesTab : public QWidget, public PropertiesTab
{
	Q_OBJECT
	public:
		TransformationPropertiesTab(QWidget *parent = 0);
		~TransformationPropertiesTab();
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
		/** Callback when position value is changed.
		 */
		void posChanged(double);
		/** Callback when rotation value is changed.
		*/
		void rotChanged(double);
		/** Callback when scale value is changed.
		*/
		void scaleChanged(double);

		void activateTranslationHandle();
		void activateScaleHandle();
		void activateRotationHandle();

		void _onTranslationHandleMoved();
		void _onScaleHandleMoved();
		void _onRotationHandleMoved();
	private:
		
		void _onTransformationHandleMoved(std::string const& propertyName, float x, float y, float z);
		void _onTransformationHandleReleased();

		void _onGenerateCppCodeForManagedEntity(ManagedEntity const* entity, std::string const& entityVar, std::ostream& out);

		/** Writes a given value into the selected entities property assigned to this Tab.
		 * @param group The group key to use for the write operation.
		 * @param component The component key to use for the write operation.
		 * @param value The value to write.
		 * @note The value is written into the following field: property[group][component].
		 */
		void _serializeValue(std::string const& group, std::string const& component, double value);

		bool							mTransformationHandleChanged;
		bool							mOpen;				// The flag if the tab is opened or not.
		Ui::TransformationPropertiesTab	ui;					// The Qt UI class for the project creation dialog
};
