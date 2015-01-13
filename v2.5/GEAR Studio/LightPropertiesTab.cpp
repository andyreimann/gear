#include "LightPropertiesTab.h"
#include "GEARStudioEvents.h"
#include "Defines.h"

#include <G2/RenderComponent.h>


static const std::string LIGHT_TYPE = "t";
static const std::string LIGHT_ENABLED = "enabled";
static const std::string LIGHT_AMBIENT = "amb";
static const std::string LIGHT_DIFFUSE = "dif";
static const std::string LIGHT_SPECULAR = "spe";
static const std::string LIGHT_COL_R = "r";
static const std::string LIGHT_COL_G = "g";
static const std::string LIGHT_COL_B = "b";
static const std::string LIGHT_COL_A = "a";
static const std::string LIGHT_ATT_CONST = "att_c";
static const std::string LIGHT_ATT_LIN = "att_l";
static const std::string LIGHT_ATT_EXP = "att_e";

LightPropertiesTab::LightPropertiesTab(QWidget *parent /*= 0*/)
	: QWidget(parent),
	PropertiesTab("light","Light")
{
	ui.setupUi(this);
	ui.tabToggle->setText(mTabName.c_str()); // set display name on tab toggle

	connect(ui.tabToggle, SIGNAL(clicked()), this, SLOT(toggleTab()));
	connect(ui.lightTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLightType(int)));
	connect(ui.constAttenuationSlider, SIGNAL(valueChanged(int)), this, SLOT(constAttSliderChanged(int)));
	connect(ui.constAttenuationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(constAttValueChanged(double)));
	connect(ui.linearAttenuationSlider, SIGNAL(valueChanged(int)), this, SLOT(linearAttSliderChanged(int)));
	connect(ui.linearAttenuationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(linearAttValueChanged(double)));
	connect(ui.exponentialAttenuationSlider, SIGNAL(valueChanged(int)), this, SLOT(exponentialAttSliderChanged(int)));
	connect(ui.exponentialAttenuationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(exponentialAttValueChanged(double)));

	mAmbientSelector = std::shared_ptr<ColorSelector>(new ColorSelector(G2::LightComponent::AMBIENT_DEFAULT, Json::Value(), this));
	mAmbientSelector->onColorSelected.hook(this, &LightPropertiesTab::_ambientColorSelected);
	ui.ambientColorRoot->layout()->addWidget(mAmbientSelector.get());

	mDiffuseSelector = std::shared_ptr<ColorSelector>(new ColorSelector(G2::LightComponent::DIFFUSE_DEFAULT, Json::Value(), this));
	mDiffuseSelector->onColorSelected.hook(this, &LightPropertiesTab::_diffuseColorSelected);
	ui.diffuseColorRoot->layout()->addWidget(mDiffuseSelector.get());

	mSpecularSelector = std::shared_ptr<ColorSelector>(new ColorSelector(G2::LightComponent::SPECULAR_DEFAULT, Json::Value(), this));
	mSpecularSelector->onColorSelected.hook(this, &LightPropertiesTab::_specularColorSelected);
	ui.specularColorRoot->layout()->addWidget(mSpecularSelector.get());

	// init light type mapping
	mLightTypeToIndex["POSITIONAL"] = 0;
	mLightTypeToIndex["DIRECTIONAL"] = 1;
	mLightTypeToIndex["SPOT"] = 2;

	mIndexToLightType[0] = "POSITIONAL";
	mIndexToLightType[1] = "DIRECTIONAL";
	mIndexToLightType[2] = "SPOT";

	GEARStudioEvents::onGenerateCppCodeForManagedEntity.hook(this, &LightPropertiesTab::_onGenerateCppCodeForManagedEntity);
}

LightPropertiesTab::~LightPropertiesTab()
{
	mAmbientSelector->onColorSelected.unHookAll(this);
	mDiffuseSelector->onColorSelected.unHookAll(this);
	mSpecularSelector->onColorSelected.unHookAll(this);
	GEARStudioEvents::onGenerateCppCodeForManagedEntity.unHookAll(this);
}

void LightPropertiesTab::_initUiWithEntity(ManagedEntity* entity)
{
	if (!hasEntity() || !entity->hasProperties(mTechnicalName))
	{
		// either no ManagedEntity has a focus, or the current ManagedEntity does not have the tabs property and hence do not have the property assigned.
		hide();
	}
	else
	{
		/************************************************************************
		 * The property of this tabs technical name is present.					*
		 * We still assume the absence of every property this tab operates on.	*
		 * Hence we will have a fallback on the default values for every single	*
		 * property we need for initialization of the UI of this tab.			*
		 ************************************************************************/
		show();
		Json::Value const& props = entity->getProperties(mTechnicalName);

		_initEnabledFlag(entity, LIGHT_ENABLED);
		// init colors
		_initColorSelector(entity, mAmbientSelector, LIGHT_AMBIENT);
		_initColorSelector(entity, mDiffuseSelector, LIGHT_DIFFUSE);
		_initColorSelector(entity, mSpecularSelector, LIGHT_SPECULAR);
		// init attenuations
		_initColorSelector(entity, mSpecularSelector, LIGHT_SPECULAR);

		_initLightType(entity);
	}
}

void LightPropertiesTab::_instantiateFromDescription(ManagedEntity* entity)
{
	// do everything the LightPropertiesTab should do on a ManagedEntity
	if (entity == nullptr || !entity->hasProperties(mTechnicalName))
	{
		return; // we are not responsible for that entity
	}

	Json::Value& props = entity->getProperties(mTechnicalName);
	// remove any existing light
	entity->removeComponent<G2::LightComponent>();
	// add a new light depending on it's defined type
	auto* lightComp = entity->addComponent<G2::LightComponent>(_getLightTypeFromProperties(entity));

	// set all the colors
	_reloadColors(entity);

	// process enabled flag
	if (props.isMember(LIGHT_ENABLED))
	{
		lightComp->setEnabled(props[LIGHT_ENABLED].asBool());
	}
}

void LightPropertiesTab::_initColorSelector(ManagedEntity* entity, std::shared_ptr<ColorSelector>& colorSelector, std::string const& propertyMember)
{
	Json::Value const& props = entity->getProperties(mTechnicalName);
	if (props.isMember(propertyMember))
	{
		colorSelector->setData(props[propertyMember]);
	}
	else
	{
		colorSelector->setData(Json::Value());
	}
}

void LightPropertiesTab::_initLightType(ManagedEntity* entity)
{
	Json::Value& props = entity->getProperties(mTechnicalName);
	if (!props.isMember(LIGHT_TYPE))
	{
		props[LIGHT_TYPE] = "POSITIONAL";
	}
	if (mLightTypeToIndex.count(props[LIGHT_TYPE].asString()) > 0)
	{
		ui.lightTypeComboBox->setCurrentIndex(mLightTypeToIndex[props[LIGHT_TYPE].asString()]);
	}
}

void LightPropertiesTab::_initEnabledFlag(ManagedEntity* entity, std::string const& propertyMember)
{
	Json::Value const& props = entity->getProperties(mTechnicalName);
	if (props.isMember(propertyMember))
	{
		ui.enabledCheckBox->setChecked(props[propertyMember].asBool());
	}
	else
	{
		ui.enabledCheckBox->setChecked(true);
	}
}

void LightPropertiesTab::_initAttenuation(float value, QDoubleSpinBox* spinBox, QSlider* slider)
{
	spinBox->setValue((double)value);
	slider->setValue((int)value);
}

void LightPropertiesTab::changeLightType(int idx)
{
	if (hasEntity())
	{
		Json::Value& props = mEntity->getProperties(mTechnicalName);
		props[LIGHT_TYPE] = mIndexToLightType[idx];

		mProject->getCurrentScene()->save();

		_initUiWithEntity(mEntity);
		_instantiateFromDescription(mEntity);
	}
}

void LightPropertiesTab::constAttSliderChanged(int value)
{
	// sync
	GEAR_QTNOEVENT(ui.constAttenuationSpinBox, ui.constAttenuationSpinBox->setValue((double)value));

	_serializeAttenuation();
	_reloadAttenuation(mEntity);
	mProject->getCurrentScene()->save();
}

void LightPropertiesTab::constAttValueChanged(double value)
{
	// sync
	GEAR_QTNOEVENT(ui.constAttenuationSlider, ui.constAttenuationSlider->setValue((int)value));

	_serializeAttenuation();
	_reloadAttenuation(mEntity);
	mProject->getCurrentScene()->save();
}

void LightPropertiesTab::linearAttSliderChanged(int value)
{
	// sync
	GEAR_QTNOEVENT(ui.linearAttenuationSpinBox, ui.linearAttenuationSpinBox->setValue((double)value));

	_serializeAttenuation();
	_reloadAttenuation(mEntity);
	mProject->getCurrentScene()->save();
}

void LightPropertiesTab::linearAttValueChanged(double value)
{
	// sync
	GEAR_QTNOEVENT(ui.linearAttenuationSlider, ui.linearAttenuationSlider->setValue((int)value));

	_serializeAttenuation();
	_reloadAttenuation(mEntity);
	mProject->getCurrentScene()->save();
}

void LightPropertiesTab::exponentialAttSliderChanged(int value)
{
	// sync
	GEAR_QTNOEVENT(ui.exponentialAttenuationSpinBox, ui.exponentialAttenuationSpinBox->setValue((double)value));

	_serializeAttenuation();
	_reloadAttenuation(mEntity);
	mProject->getCurrentScene()->save();
}

void LightPropertiesTab::exponentialAttValueChanged(double value)
{
	// sync
	GEAR_QTNOEVENT(ui.exponentialAttenuationSlider, ui.exponentialAttenuationSlider->setValue((int)value));

	_serializeAttenuation();
	_reloadAttenuation(mEntity);
	mProject->getCurrentScene()->save();
}

void LightPropertiesTab::_serializeAttenuation()
{
	Json::Value& props = mEntity->getProperties(mTechnicalName);
	props[LIGHT_ATT_CONST] = (float)ui.constAttenuationSpinBox->value();
	props[LIGHT_ATT_LIN] = (float)ui.linearAttenuationSpinBox->value();
	props[LIGHT_ATT_EXP] = (float)ui.exponentialAttenuationSpinBox->value();
}

void LightPropertiesTab::_ambientColorSelected(ColorSelector* colorSelector)
{
	_colorSelected(colorSelector, LIGHT_AMBIENT);
}

void LightPropertiesTab::_diffuseColorSelected(ColorSelector* colorSelector)
{
	_colorSelected(colorSelector, LIGHT_DIFFUSE);
}

void LightPropertiesTab::_specularColorSelected(ColorSelector* colorSelector)
{
	_colorSelected(colorSelector, LIGHT_SPECULAR);
}

void LightPropertiesTab::_colorSelected(ColorSelector* colorSelector, std::string const& targetProperty)
{
	if (hasEntity() && colorSelector != nullptr)
	{
		auto* lightComp = mEntity->getComponent<G2::LightComponent>();
		Json::Value& props = mEntity->getProperties(mTechnicalName);
		props[targetProperty] = colorSelector->getData();

		_reloadColors(mEntity);

		mProject->getCurrentScene()->save();
	}
}

void LightPropertiesTab::_reloadColors(ManagedEntity* target)
{
	auto* lightComp = target->getComponent<G2::LightComponent>();

	lightComp->setupColor(
		_getColorFromProperties(target, LIGHT_AMBIENT, G2::LightComponent::AMBIENT_DEFAULT),
		_getColorFromProperties(target, LIGHT_DIFFUSE, G2::LightComponent::DIFFUSE_DEFAULT),
		_getColorFromProperties(target, LIGHT_SPECULAR, G2::LightComponent::SPECULAR_DEFAULT)
	);
}

void LightPropertiesTab::_reloadAttenuation(ManagedEntity* target)
{
	auto* lightComp = target->getComponent<G2::LightComponent>();

	Json::Value const& props = target->getProperties(mTechnicalName);
	if (props.isMember(LIGHT_ATT_CONST))
	{
		lightComp->attenuation = props[LIGHT_ATT_CONST].asFloat();
	}
	if (props.isMember(LIGHT_ATT_LIN))
	{
		lightComp->linearAttenuation = props[LIGHT_ATT_LIN].asFloat();
	}
	if (props.isMember(LIGHT_ATT_EXP))
	{
		lightComp->exponentialAttenuation = props[LIGHT_ATT_EXP].asFloat();
	}
}

G2::LightType::Name LightPropertiesTab::_getLightTypeFromProperties(ManagedEntity* target) const
{
	Json::Value const& props = target->getProperties(mTechnicalName);
	if (props.isMember(LIGHT_TYPE))
	{
		std::string typeStr = props[LIGHT_TYPE].asString();
		if (typeStr == "POSITIONAL")
		{
			return G2::LightType::POSITIONAL;
		}
		else if (typeStr == "DIRECTIONAL")
		{
			return G2::LightType::DIRECTIONAL;
		}
		else if (typeStr == "SPOT")
		{
			return G2::LightType::SPOT;
		}
		else
		{
			std::stringstream log;
			log << "Unknown light type '" << typeStr << "' for " << target->getName() << ". Defaulting to Positional Light!";
			GEARStudioEvents::onLog(WARNING, log.str());
		}
	}
	return G2::LightType::POSITIONAL;
}

glm::vec4 LightPropertiesTab::_getColorFromProperties(ManagedEntity const* target, std::string const& propertyMember, glm::vec4 const& defaultValue) const
{
	Json::Value const& props = target->getProperties(mTechnicalName);
	if (props.isMember(propertyMember))
	{
		if (props[propertyMember].isMember(LIGHT_COL_R) &&
			props[propertyMember].isMember(LIGHT_COL_G) &&
			props[propertyMember].isMember(LIGHT_COL_B) &&
			props[propertyMember].isMember(LIGHT_COL_A))
		{
			return glm::vec4(
				props[propertyMember][LIGHT_COL_R].asInt() / 255.f,
				props[propertyMember][LIGHT_COL_G].asInt() / 255.f,
				props[propertyMember][LIGHT_COL_B].asInt() / 255.f,
				props[propertyMember][LIGHT_COL_A].asInt() / 255.f
			);
		}
	}
	return defaultValue;
}

void LightPropertiesTab::toggleTab()
{
	if (mOpen)
	{
		ui.groupBox->hide();
	}
	else
	{
		ui.groupBox->show();
	}
	mOpen = !mOpen;
}

void LightPropertiesTab::_onGenerateCppCodeForManagedEntity(ManagedEntity const* entity, std::string const& entityVar, std::ostream& out)
{
	if (!entity->hasProperties(mTechnicalName))
	{
		return; // we are not responsible for that entity
	}
	/************************************************************************
	* Here we generate all the code this PropertiesTab is responsible for.	*
	************************************************************************/
	std::string indention = "			";

	Json::Value const& props = entity->getProperties(mTechnicalName);



	// good practise to enclose the generated code in {}

	if (props.isMember(LIGHT_TYPE))
	{
		std::string typeStr = props[LIGHT_TYPE].asString();
		out << "		{" << std::endl;
		{
			out << indention << "// Light" << std::endl;
			out << indention << "auto* lc = " << entityVar << ".addComponent<LightComponent>(LightType::" << typeStr << "); " << std::endl;
			if (props.isMember(LIGHT_AMBIENT) || props.isMember(LIGHT_DIFFUSE) || props.isMember(LIGHT_SPECULAR))
			{
				glm::vec4 a = _getColorFromProperties(entity, LIGHT_AMBIENT, G2::Material::AMBIENT_DEFAULT);
				glm::vec4 d = _getColorFromProperties(entity, LIGHT_DIFFUSE, G2::Material::DIFFUSE_DEFAULT);
				glm::vec4 s = _getColorFromProperties(entity, LIGHT_SPECULAR, G2::Material::SPECULAR_DEFAULT);
				out << indention << "lc->setupColor(glm::vec4(" << a.r << "f," << a.g << "f," << a.b << "f," << a.a << "f),glm::vec4(" << d.r << "f," << d.g << "f," << d.b << "f," << d.a << "f),glm::vec4(" << s.r << "f," << s.g << "f," << s.b << "f," << s.a << "f));" << std::endl;
			}
			if (props.isMember(LIGHT_ENABLED) && props[LIGHT_ENABLED].asBool() == false)
			{
				out << indention << "lc->setEnabled(false);" << std::endl;
			}
			if (props.isMember(LIGHT_ATT_CONST))
			{
				out << indention << "lc->attenuation = " << props[LIGHT_ATT_CONST].asFloat() << "f;" << std::endl;
			}
			if (props.isMember(LIGHT_ATT_LIN))
			{
				out << indention << "lc->linearAttenuation = " << props[LIGHT_ATT_LIN].asFloat() << "f;" << std::endl;
			}
			if (props.isMember(LIGHT_ATT_EXP))
			{
				out << indention << "lc->exponentialAttenuation = " << props[LIGHT_ATT_EXP].asFloat() << "f;" << std::endl;
			}
		}
		out << "		}" << std::endl;
	}
}