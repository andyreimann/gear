#pragma once
#include "ui_LightPropertiesTab.h"
#include "PropertiesTab.h"
#include "ColorSelector.h"

#include <G2/FBXImporter.h>
#include <G2/MD5Importer.h>
#include <G2/LightComponent.h>

#include <QtWidgets/QWidget>


/** This class implements the ManagedEntity property regarding lights.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class LightPropertiesTab : public QWidget, public PropertiesTab
{
	Q_OBJECT
	public:
		LightPropertiesTab(QWidget *parent = 0);

		~LightPropertiesTab();

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
		void changeLightType(int idx);

		void constAttSliderChanged(int value);
		void constAttValueChanged(double value);

		void linearAttSliderChanged(int value);
		void linearAttValueChanged(double value);

		void exponentialAttSliderChanged(int value);
		void exponentialAttValueChanged(double value);

		/** Toggles the folding state of the tab.
		*/
		void toggleTab();
	private:

		void _ambientColorSelected(ColorSelector* colorSelector);
		void _diffuseColorSelected(ColorSelector* colorSelector);
		void _specularColorSelected(ColorSelector* colorSelector);
		void _colorSelected(ColorSelector* colorSelector, std::string const& targetProperty);
		void _reloadColors(ManagedEntity* target);
		void _reloadAttenuation(ManagedEntity* target);

		void _initColorSelector(ManagedEntity* entity, std::shared_ptr<ColorSelector>& colorSelector, std::string const& propertyMember);
		void _initEnabledFlag(ManagedEntity* entity, std::string const& propertyMember);
		void _initLightType(ManagedEntity* entity);
		void _initAttenuation(float value, QDoubleSpinBox* spinBox, QSlider* slider);

		void _serializeAttenuation();

		G2::LightType::Name _getLightTypeFromProperties(ManagedEntity* target) const;
		glm::vec4 _getColorFromProperties(ManagedEntity const* target, std::string const& propertyMember, glm::vec4 const& defaultValue) const;

		void _onGenerateCppCodeForManagedEntity(ManagedEntity const* entity, std::string const& entityVar, std::ostream& out);

		bool					mOpen;				// The flag if the tab is opened or not.
		Ui::LightPropertiesTab	ui;					// The Qt UI class for the project creation dialog

		std::shared_ptr<ColorSelector> mAmbientSelector;
		std::shared_ptr<ColorSelector> mDiffuseSelector;
		std::shared_ptr<ColorSelector> mSpecularSelector;

		std::map<std::string, int>	   mLightTypeToIndex;
		std::map<int, std::string>	   mIndexToLightType;
};
