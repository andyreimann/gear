#include "MaterialPropertiesTab.h"
#include "GEARStudioEvents.h"
#include "TextureSelector.h"
#include "Defines.h"

#include <G2/RenderComponent.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>

// TODO Should these identifier be compiled into a separated DLL?
static const std::string FX = "fx";
static const std::string FX_PATH = "path";
static const std::string MAT_AMBIENT = "amb";
static const std::string MAT_DIFFUSE = "dif";
static const std::string MAT_SPECULAR = "spe";
static const std::string MAT_SHININESS = "sh";
static const std::string TEXTURES = "tex";
static const std::string TEXTURES_PATH = "path";
static const std::string TEXTURES_SAMPLER = "sampler";
static const std::string MAT_COL_R = "r";
static const std::string MAT_COL_G = "g";
static const std::string MAT_COL_B = "b";
static const std::string MAT_COL_A = "a";

MaterialPropertiesTab::MaterialPropertiesTab(QWidget *parent /*= 0*/)
	: QWidget(parent),
	PropertiesTab("mat","Material")
{
	ui.setupUi(this);
	ui.tabToggle->setText(mTabName.c_str()); // set display name on tab toggle

	connect(ui.tabToggle, SIGNAL(clicked()), this, SLOT(toggleTab()));
	connect(ui.shininessSlider, SIGNAL(valueChanged(int)), this, SLOT(shininessSliderChanged(int)));
	connect(ui.shininessValue, SIGNAL(valueChanged(double)), this, SLOT(shininessValueChanged(double)));
	connect(ui.addTextureSelector, SIGNAL(clicked()), this, SLOT(addTextureSelector()));

	mAmbientSelector = std::shared_ptr<ColorSelector>(new ColorSelector(G2::Material::AMBIENT_DEFAULT, Json::Value(), this));
	mAmbientSelector->onColorSelected.hook(this, &MaterialPropertiesTab::_ambientColorSelected);
	ui.ambientColorRoot->layout()->addWidget(mAmbientSelector.get());

	mDiffuseSelector = std::shared_ptr<ColorSelector>(new ColorSelector(G2::Material::DIFFUSE_DEFAULT, Json::Value(), this));
	mDiffuseSelector->onColorSelected.hook(this, &MaterialPropertiesTab::_diffuseColorSelected);
	ui.diffuseColorRoot->layout()->addWidget(mDiffuseSelector.get());

	mSpecularSelector = std::shared_ptr<ColorSelector>(new ColorSelector(G2::Material::SPECULAR_DEFAULT, Json::Value(), this));
	mSpecularSelector->onColorSelected.hook(this, &MaterialPropertiesTab::_specularColorSelected);
	ui.specularColorRoot->layout()->addWidget(mSpecularSelector.get());

	mEffectSelector = std::shared_ptr<FileSelector>(new FileSelector("/assets/shader", Json::Value(), mProjectDirectory, "GEAR Effect (*.g2fx)", this));
	mEffectSelector->onFileSelected.hook(this, &MaterialPropertiesTab::_effectSelected);
	ui.effectFileRoot->layout()->addWidget(mEffectSelector.get());

	GEARStudioEvents::onGenerateCppCodeForManagedEntity.hook(this, &MaterialPropertiesTab::_onGenerateCppCodeForManagedEntity);
}

MaterialPropertiesTab::~MaterialPropertiesTab()
{
	mAmbientSelector->onColorSelected.unHookAll(this);
	mDiffuseSelector->onColorSelected.unHookAll(this);
	mSpecularSelector->onColorSelected.unHookAll(this);
	GEARStudioEvents::onGenerateCppCodeForManagedEntity.unHookAll(this);
}

void MaterialPropertiesTab::_initUiWithEntity(ManagedEntity* entity)
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

		// here we check for every member we expect inside of our property.
		// If one is absent, we just initialize it to it's default value.

		// init colors
		_initColorSelector(entity, mAmbientSelector, MAT_AMBIENT);
		_initColorSelector(entity, mDiffuseSelector, MAT_DIFFUSE);
		_initColorSelector(entity, mSpecularSelector, MAT_SPECULAR);

		float shininess = 0.f; // default shininess
		if (props.isMember(MAT_SHININESS))
		{
			shininess = props[MAT_SHININESS].asFloat();
		}
		GEAR_QTNOEVENT(ui.shininessValue, ui.shininessValue->setValue(shininess));

		if (props.isMember(FX))
		{
			mEffectSelector->setData(props[FX]);
		}
		else
		{
			mEffectSelector->setData(Json::Value());
		}
		removeAllTextureSelectors();

		// import textures
		if (props.isMember(TEXTURES))
		{
			Json::Value const& textures = props[TEXTURES];

			for (unsigned int i = 0; i < textures.size(); ++i)  // Iterates over the sequence elements.
			{
				addTextureSelector(entity, textures[i]);
			}
		}
	}
}

void MaterialPropertiesTab::_instantiateFromDescription(ManagedEntity* entity)
{
	// do everything the MeshPropertiesTab should do on a ManagedEntity
	_reimportMaterial(entity, true);
}

QColor MaterialPropertiesTab::_toQColor(glm::vec4 const& gearColor) const
{
	return QColor(
		(int)(gearColor.r * 255.f),
		(int)(gearColor.g * 255.f),
		(int)(gearColor.b * 255.f),
		(int)(gearColor.a * 255.f)
		);
}

void MaterialPropertiesTab::_initColorSelector(ManagedEntity* entity, std::shared_ptr<ColorSelector>& colorSelector, std::string const& propertyMember)
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

void MaterialPropertiesTab::toggleTab()
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

void MaterialPropertiesTab::_reimportMaterial(ManagedEntity* target, bool reimportEffect)
{
	if (!target->hasProperties(mTechnicalName))
	{
		return; // we are not responsible for that entity
	}

	Json::Value& props = target->getProperties(mTechnicalName);
	auto renderComp = target->addComponent<G2::RenderComponent>();



	if (reimportEffect)
	{
		// do load the effect
		std::string fxPath = props.get(FX_PATH, "").asString();
		std::transform(fxPath.begin(), fxPath.end(), fxPath.begin(), ::tolower);

		// TODO Check if file exists
		if (boost::algorithm::ends_with(fxPath, "g2fx"))
		{
			// WE DON'T LOAD THE SHADERS IN THE EDITOR!
			//renderComp->setEffect(mFxImporter.import(mProjectDirectory + props.get(FX_PATH, "").asString()));
		}
		else
		{
			// TODO log warning
		}
	}

	/************************************************************************
	* The property of this tabs technical name is present.					*
	* We still assume the absence of every property this tab operates on.	*
	* Hence we will have a fall back on the default values for every single	*
	* property we need for initialization of the UI of this tab.			*
	************************************************************************/


	// set all the colors
	_reloadColors(target);

	if (props.isMember(MAT_SHININESS))
	{
		renderComp->material.setShininess(props[MAT_SHININESS].asFloat());
	}

	/*
	if (props.isMember(FX_PATH))
	{
		auto effect = mFxImporter.import(mProjectDirectory + props.get(FX_PATH, "").asString());
		renderComp->setEffect(effect);
		if (effect.get() == nullptr)
		{
			// TODO Log warning?
		}
	}
	*/

	removeAllTextureSelectors();

	if (props.isMember(TEXTURES))
	{
		Json::Value const& textures = props[TEXTURES];

		for (unsigned int i = 0; i < textures.size(); ++i)  // Iterates over the sequence elements.
		{
			std::string samplerStr = textures[i][TEXTURES_SAMPLER].asString();
			G2::Sampler::Name sampler = G2::Sampler::getSampler(samplerStr);
			
			std::string fullPath = mProjectDirectory + textures[i][TEXTURES_PATH].asString();
			auto tex = mTextureImporter.import(
				fullPath,
				G2Core::DataFormat::Internal::R32G32B32A32_F,
				G2Core::FilterMode::LINEAR,
				G2Core::FilterMode::LINEAR
			);
			renderComp->material.setTexture(sampler, tex);

			addTextureSelector(target, textures[i]);
		}
	}
	show();
}

void
MaterialPropertiesTab::_ambientColorSelected(ColorSelector* colorSelector)
{
	_colorSelected(colorSelector, MAT_AMBIENT);
}

void
MaterialPropertiesTab::_diffuseColorSelected(ColorSelector* colorSelector)
{
	_colorSelected(colorSelector, MAT_DIFFUSE);
}

void
MaterialPropertiesTab::_specularColorSelected(ColorSelector* colorSelector)
{
	_colorSelected(colorSelector, MAT_SPECULAR);
}

void MaterialPropertiesTab::_colorSelected(ColorSelector* colorSelector, std::string const& targetProperty)
{
	if (hasEntity() && colorSelector != nullptr)
	{
		auto* renderComp = mEntity->getComponent<G2::RenderComponent>();
		Json::Value& props = mEntity->getProperties(mTechnicalName);
		props[targetProperty] = colorSelector->getData();

		_reloadColors(mEntity);

		mProject->getCurrentScene()->save();
	}
}

void MaterialPropertiesTab::_effectSelected(FileSelector* fileSelector)
{
	if (hasEntity() && fileSelector != nullptr)
	{
		Json::Value& props = mEntity->getProperties(mTechnicalName);
		props[FX] = fileSelector->getData();
		// release the caching entry for the effect to reimport it from scratch
		mFxImporter.clearCache(mProjectDirectory + props[FX][FX_PATH].asString());
		mProject->getCurrentScene()->save();
	}
}

void MaterialPropertiesTab::_reloadColors(ManagedEntity* target)
{
	auto* renderComp = target->getComponent<G2::RenderComponent>();

	renderComp->material
		.setAmbient(_getColorFromProperties(target, MAT_AMBIENT, G2::Material::AMBIENT_DEFAULT))
		.setDiffuse(_getColorFromProperties(target, MAT_DIFFUSE, G2::Material::DIFFUSE_DEFAULT))
		.setSpecular(_getColorFromProperties(target, MAT_SPECULAR, G2::Material::SPECULAR_DEFAULT));
}

glm::vec4 MaterialPropertiesTab::_getColorFromProperties(ManagedEntity const* target, std::string const& propertyMember, glm::vec4 const& defaultValue) const
{
	Json::Value const& props = target->getProperties(mTechnicalName);
	if (props.isMember(propertyMember))
	{
		if (props[propertyMember].isMember(MAT_COL_R) &&
			props[propertyMember].isMember(MAT_COL_G) &&
			props[propertyMember].isMember(MAT_COL_B) &&
			props[propertyMember].isMember(MAT_COL_A))
		{
			return glm::vec4(
				props[propertyMember][MAT_COL_R].asInt() / 255.f,
				props[propertyMember][MAT_COL_G].asInt() / 255.f,
				props[propertyMember][MAT_COL_B].asInt() / 255.f,
				props[propertyMember][MAT_COL_A].asInt() / 255.f
				);
		}
	}
	return defaultValue;
}

void MaterialPropertiesTab::shininessSliderChanged(int value)
{
	// sync
	GEAR_QTNOEVENT(ui.shininessValue, ui.shininessValue->setValue((double)value));

	_serializeShininess();

	_reimportMaterial(mEntity, false);
	mProject->getCurrentScene()->save();
}

void MaterialPropertiesTab::shininessValueChanged(double value)
{
	// sync
	GEAR_QTNOEVENT(ui.shininessSlider, ui.shininessSlider->setValue((int)value));
	
	_serializeShininess();

	_reimportMaterial(mEntity, false);
	mProject->getCurrentScene()->save();
}

void MaterialPropertiesTab::_serializeShininess()
{
	Json::Value& props = mEntity->getProperties(mTechnicalName);
	props[MAT_SHININESS] = (float)ui.shininessValue->value();
}

void
MaterialPropertiesTab::_onTextureSelected(TextureSelector* selector)
{
	// remove the changed textures entry from the cache to newly import it
	Json::Value const& changedTexture = selector->getData();
	mTextureImporter.clearCache(mProjectDirectory + changedTexture[TEXTURES_PATH].asString());

	// save and reimport all textures - the changed one will be read newly
	_rebuildTextures();

	// save entire scene
	mProject->getCurrentScene()->save();
}

void
MaterialPropertiesTab::_onSamplerSelected(TextureSelector* selector)
{
	if (selector->getData().isMember(TEXTURES_PATH))
	{
		// save and reimport all textures using the cache
		_rebuildTextures();

		// save entire scene
		mProject->getCurrentScene()->save();
	}
}

void
MaterialPropertiesTab::_onRemoveTexture(TextureSelector* selector)
{
	G2::Sampler::Name sampler = G2::Sampler::getSampler(selector->getData()[TEXTURES_SAMPLER].asString());
	// remove the texture selector from our list
	for (int i = 0; i < mTextureSelector.size(); ++i)
	{
		if (mTextureSelector[i] == selector)
		{
			std::swap(mTextureSelector[i], mTextureSelector.back());
			mTextureSelector.back()->onTextureSelected.unHookAll(this);
			mTextureSelector.back()->onSamplerSelected.unHookAll(this);
			mTextureSelector.back()->onRemoveSelector.unHookAll(this);
			
			ui.textureSelectorRoot->layout()->removeWidget(mTextureSelector.back());
			mTextureSelector.back()->deleteLater();
			mTextureSelector.pop_back();
			break;
		}
	}
	// save and reimport all textures using the cache
	auto* renderComponent = mEntity->getComponent<G2::RenderComponent>();
	renderComponent->material.setTexture(sampler, std::shared_ptr<G2::Texture>());

	_rebuildTextures();

	// save entire scene
	mProject->getCurrentScene()->save();
}

void
MaterialPropertiesTab::_rebuildTextures()
{
	Json::Value& props = mEntity->getProperties(mTechnicalName);

	Json::Value textures(Json::arrayValue);
	auto* renderComponent = mEntity->getComponent<G2::RenderComponent>();
	for (int i = 0; i < G2::Sampler::NUM_SAMPLERS; ++i)
	{
		// reset every texture
		renderComponent->material.setTexture(static_cast<G2::Sampler::Name>(i), std::shared_ptr<G2::Texture>());
	}

	// build the json array new from scratch and import the images
	for (auto it = mTextureSelector.begin(); it != mTextureSelector.end(); ++it)
	{
		Json::Value const& texData = (*it)->getData();
		G2::Sampler::Name sampler = G2::Sampler::getSampler(texData[TEXTURES_SAMPLER].asString());
		if (sampler != G2::Sampler::SAMPLER_INVALID)
		{
			auto tex = mTextureImporter.import(
				mProjectDirectory + texData[TEXTURES_PATH].asString(),
				G2Core::DataFormat::Internal::R32G32B32A32_F,
				G2Core::FilterMode::LINEAR,
				G2Core::FilterMode::LINEAR);
			renderComponent->material.setTexture(sampler, tex);
		}
		textures.append(texData);
	}
	// save the textures array
	props[TEXTURES] = textures;
}

void MaterialPropertiesTab::addTextureSelector()
{
	if (hasEntity())
	{
		Json::Value& props = mEntity->getProperties(mTechnicalName);
		Json::Value newTextureData;
		props[TEXTURES].append(newTextureData);
		addTextureSelector(mEntity, newTextureData);
	}
}

void MaterialPropertiesTab::addTextureSelector(ManagedEntity* entity, Json::Value const& target)
{
	mTextureSelector.push_back(new TextureSelector(target, mProjectDirectory, this));
	ui.textureSelectorRoot->layout()->addWidget(mTextureSelector.back());
	mTextureSelector.back()->onTextureSelected.hook(this, &MaterialPropertiesTab::_onTextureSelected);
	mTextureSelector.back()->onSamplerSelected.hook(this, &MaterialPropertiesTab::_onSamplerSelected);
	mTextureSelector.back()->onRemoveSelector.hook(this, &MaterialPropertiesTab::_onRemoveTexture);
}

void
MaterialPropertiesTab::removeAllTextureSelectors()
{
	for (auto it = mTextureSelector.begin(); it != mTextureSelector.end(); ++it)
	{
		// unregister all texture selectors from the layout
		ui.textureSelectorRoot->layout()->removeWidget(*it);
		delete (*it);
	}
	// delete all texture selector instances
	mTextureSelector.clear();
}

void MaterialPropertiesTab::_onGenerateCppCodeForManagedEntity(ManagedEntity const* entity, std::string const& entityVar, std::ostream& out)
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
	out << "		{" << std::endl;
	{
		out << indention << "// Material" << std::endl;
		out << indention << "auto* rc = " << entityVar << ".addComponent<RenderComponent>();" << std::endl;
		if (props.isMember(FX) && props[FX].isMember(FX_PATH))
		{
			out << indention << "rc->setEffect(mFxImporter.import(mProjectRoot + \"" << props[FX][FX_PATH].asString() << "\"));" << std::endl;
		}
		if (props.isMember(MAT_AMBIENT))
		{
			glm::vec4 c = _getColorFromProperties(entity, MAT_AMBIENT, G2::Material::AMBIENT_DEFAULT);
			out << indention << "rc->material.setAmbient(glm::vec4(" << c.r << "f," << c.g << "f," << c.b << "f," << c.a << "f));" << std::endl;
		}
		if (props.isMember(MAT_DIFFUSE))
		{
			glm::vec4 c = _getColorFromProperties(entity, MAT_DIFFUSE, G2::Material::DIFFUSE_DEFAULT);
			out << indention << "rc->material.setDiffuse(glm::vec4(" << c.r << "f," << c.g << "f," << c.b << "f," << c.a << "f));" << std::endl;
		}
		if (props.isMember(MAT_SPECULAR))
		{
			glm::vec4 c = _getColorFromProperties(entity, MAT_SPECULAR, G2::Material::SPECULAR_DEFAULT);
			out << indention << "rc->material.setSpecular(glm::vec4(" << c.r << "f," << c.g << "f," << c.b << "f," << c.a << "f));" << std::endl;
		}
		if (props.isMember(MAT_SHININESS))
		{
			out << indention << "rc->material.setShininess(" << props[MAT_SHININESS].asFloat() << "f);" << std::endl;
		}
		if (props.isMember(TEXTURES))
		{
			Json::Value const& textures = props[TEXTURES];

			for (unsigned int i = 0; i < textures.size(); ++i)  // Iterates over the sequence elements.
			{
				std::string samplerStr = textures[i][TEXTURES_SAMPLER].asString();
				G2::Sampler::Name sampler = G2::Sampler::getSampler(samplerStr);
				if (sampler != G2::Sampler::SAMPLER_INVALID)
				{
					std::string texVar = "t" + std::to_string(i);
					out << indention << "auto " << texVar << " = mTexImporter.import(mProjectRoot + \"" << textures[i][TEXTURES_PATH].asString() << "\",G2Core::DataFormat::Internal::R32G32B32A32_F,G2Core::FilterMode::LINEAR,G2Core::FilterMode::LINEAR);" << std::endl;
					out << indention << "rc->material.setTexture(Sampler::" << samplerStr << "," << texVar << "); " << std::endl;
				}
			}
		}
	}
	out << "		}" << std::endl;
}
