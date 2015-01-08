#pragma once
#include "ui_MaterialPropertiesTab.h"
#include "PropertiesTab.h"

#include <G2/TextureImporter.h>
#include <G2/Sampler.h>

#include <QtWidgets/QWidget>
#include <memory>
#include <vector>

class TextureSelector;
/** This class implements the ManagedEntity property regarding materials.
* @created	2015/01/06
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class MaterialPropertiesTab : public QWidget, public PropertiesTab
{
	Q_OBJECT
	public:
		MaterialPropertiesTab(QWidget *parent = 0);

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


		void selectAmbient();
		void selectDiffuse();
		void selectSpecular();
		void shininessSliderChanged(int value);
		void shininessValueChanged(double value);
		void selectEffect();
		void addTextureSelector();

	private:
		
		void removeAllTextureSelectors();

		void addTextureSelector(ManagedEntity* target, std::string const& imagePath, std::string const& samplerStr);

		void _serializeShininess();
		/** Reimports the given target ManagedEntity according to it's document description.
		 * This function will reimport the material with the defined settings.
		 * Make sure to have all settings saved in the description inside the ManagedEntity before you call this function.
		 */
		void _reimportMaterial(ManagedEntity* target, bool reimportEffect);

		void _onTextureSelected(TextureSelector*);

		G2::EffectImporter		mFxImporter;		// The importer for effect files to use.
		G2::TextureImporter 	mTextureImporter;	// The importer for textures to use.
		bool					mOpen;				// The flag if the tab is opened or not.
		Ui::MaterialPropertiesTab	ui;					// The Qt UI class for the project creation dialog

		std::vector<std::shared_ptr<TextureSelector>>	mTextureSelector;
};
