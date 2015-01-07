#include "MaterialPropertiesTab.h"
#include "GEARStudioEvents.h"
#include "TextureSelector.h"

#include <G2/RenderComponent.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>

// TODO Should these identifier be compiled into a separated DLL?
static const std::string FX_PATH = "fx_path";
static const std::string MAT_AMBIENT = "amb";
static const std::string MAT_DIFFUSE = "dif";
static const std::string MAT_SPECULAR = "spe";
static const std::string MAT_SHININESS = "sh";
static const std::string TEX_DIFFUSE = "dif_tex";

MaterialPropertiesTab::MaterialPropertiesTab(QWidget *parent /*= 0*/)
	: QWidget(parent),
	PropertiesTab("mat","Material")
{
	ui.setupUi(this);
	ui.tabToggle->setText(mTabName.c_str()); // set display name on tab toggle

	connect(ui.tabToggle, SIGNAL(clicked()), this, SLOT(toggleTab()));
	connect(ui.ambientSelect, SIGNAL(clicked()), this, SLOT(selectAmbient()));
	connect(ui.diffuseSelect, SIGNAL(clicked()), this, SLOT(selectDiffuse()));
	connect(ui.specularSelect, SIGNAL(clicked()), this, SLOT(selectSpecular()));
	connect(ui.shininessSlider, SIGNAL(valueChanged(int)), this, SLOT(shininessSliderChanged(int)));
	connect(ui.shininessValue, SIGNAL(valueChanged(double)), this, SLOT(shininessValueChanged(double)));
	connect(ui.effectSelect, SIGNAL(clicked()), this, SLOT(selectEffect()));
	connect(ui.addTextureSelector, SIGNAL(clicked()), this, SLOT(addTextureSelector()));
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


		QPalette pal;
		QColor color(50, 50, 50, 255); // default ambient
		if (props.isMember(MAT_AMBIENT))
		{
			color = QColor(
				props[MAT_AMBIENT]["r"].asInt(),
				props[MAT_AMBIENT]["g"].asInt(),
				props[MAT_AMBIENT]["b"].asInt(),
				props[MAT_AMBIENT]["a"].asInt());
		}
		pal.setColor(QPalette::Background, color);
		ui.ambientSurf->blockSignals(true); ui.ambientSurf->setPalette(pal); ui.ambientSurf->blockSignals(false);

		pal = QPalette();
		color = QColor(255, 255, 255, 255); // default diffuse
		if (props.isMember(MAT_DIFFUSE))
		{
			color = QColor(
				props[MAT_DIFFUSE]["r"].asInt(),
				props[MAT_DIFFUSE]["g"].asInt(),
				props[MAT_DIFFUSE]["b"].asInt(),
				props[MAT_DIFFUSE]["a"].asInt());
		}
		pal.setColor(QPalette::Background, color);
		ui.diffuseSurf->blockSignals(true); ui.diffuseSurf->setPalette(pal); ui.diffuseSurf->blockSignals(false);

		pal = QPalette();
		color = QColor(0, 0, 0, 255); // default specular
		if (props.isMember(MAT_SPECULAR))
		{
			color = QColor(
				props[MAT_SPECULAR]["r"].asInt(),
				props[MAT_SPECULAR]["g"].asInt(),
				props[MAT_SPECULAR]["b"].asInt(),
				props[MAT_SPECULAR]["a"].asInt());
		}
		pal.setColor(QPalette::Background, color);
		ui.specularSurf->blockSignals(true); ui.specularSurf->setPalette(pal); ui.specularSurf->blockSignals(false);

		float shininess = 0.f; // default shininess
		if (props.isMember(MAT_SHININESS))
		{
			shininess = props[MAT_SHININESS].asFloat();
		}
		ui.shininessValue->blockSignals(true); ui.shininessValue->setValue(shininess); ui.shininessValue->blockSignals(false);
		if (props.isMember(FX_PATH))
		{
			ui.effectPath->setText(props[FX_PATH].asCString());
		}
		else
		{
			ui.effectPath->setText("");
		}
		/*
		if (props.isMember(TEX_DIFFUSE))
		{
			if (entity->imageCache.count(G2::Sampler::DIFFUSE) == 1)
			{
				// use cached file
				ui.diffuseTexSurf->setPixmap(QPixmap::fromImage(entity->imageCache[G2::Sampler::DIFFUSE]));
			}
			else
			{
				QImage image((mProjectDirectory + props[TEX_DIFFUSE].asString()).c_str());
				if (!image.isNull())
				{
					//std::cout << ui.label_6->width() << " --- " << ui.label_6->height() << std::endl;
					image = image.scaled(ui.diffuseTexSurf->width(), ui.diffuseTexSurf->width(), Qt::KeepAspectRatio);
					// cache imported image
					entity->imageCache[G2::Sampler::DIFFUSE] = image;
					ui.diffuseTexSurf->setPixmap(QPixmap::fromImage(image));
				}
				else
				{
					ui.diffuseTexSurf->setPixmap(QPixmap());
				}
			}
		}
		else
		{
			ui.diffuseTexSurf->setPixmap(QPixmap());
		}*/
	}
}

void MaterialPropertiesTab::_instantiateFromDescription(ManagedEntity* entity)
{
	// do everything the MeshPropertiesTab should do on a ManagedEntity
	_reimportMaterial(entity, true);
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
			// load with effect importer and attach effect
			renderComp->setEffect(mFxImporter.import(mProjectDirectory + props.get(FX_PATH, "").asString()));
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

	// here we check for every member we expect inside of our property.
	// If one is absent, we just initialize it to it's default value.
	if (props.isMember(MAT_AMBIENT))
	{
		glm::vec4 col(
			props[MAT_AMBIENT].get("r", "0.0").asInt() / 255.f,
			props[MAT_AMBIENT].get("g", "0.0").asInt() / 255.f,
			props[MAT_AMBIENT].get("b", "0.0").asInt() / 255.f,
			props[MAT_AMBIENT].get("a", "0.0").asInt() / 255.f);
		renderComp->material.setAmbient(col);
	}
	if (props.isMember(MAT_DIFFUSE))
	{
		glm::vec4 col(
			props[MAT_DIFFUSE].get("r", "0.0").asInt() / 255.f,
			props[MAT_DIFFUSE].get("g", "0.0").asInt() / 255.f,
			props[MAT_DIFFUSE].get("b", "0.0").asInt() / 255.f,
			props[MAT_DIFFUSE].get("a", "0.0").asInt() / 255.f);
		renderComp->material.setDiffuse(col);
	}
	if (props.isMember(MAT_SPECULAR))
	{
		glm::vec4 col(
			props[MAT_SPECULAR].get("r", "0.0").asInt() / 255.f,
			props[MAT_SPECULAR].get("g", "0.0").asInt() / 255.f,
			props[MAT_SPECULAR].get("b", "0.0").asInt() / 255.f,
			props[MAT_SPECULAR].get("a", "0.0").asInt() / 255.f);
		renderComp->material.setSpecular(col);
	}
	if (props.isMember(MAT_SHININESS))
	{
		renderComp->material.setShininess(props[MAT_SHININESS].asFloat());
	}
	if (props.isMember(FX_PATH))
	{
		auto effect = mFxImporter.import(mProjectDirectory + props.get(FX_PATH, "").asString());
		renderComp->setEffect(effect);
		if (effect.get() == nullptr)
		{
			// TODO Log warning?
		}
	}

	for (auto it = mTextureSelector.begin(); it != mTextureSelector.end(); ++it)
	{
		// unregister all texture selectors from the layout
		ui.textureSelectorRoot->layout()->removeWidget((*it).get());
	}
	// delete all texture selector instances
	mTextureSelector.clear();

	if (props.isMember(TEX_DIFFUSE))
	{
		auto tex = mTextureImporter.import(
			mProjectDirectory + props.get(TEX_DIFFUSE, "").asString(),
			G2Core::DataFormat::Internal::R32G32B32A32_F,
			G2Core::FilterMode::LINEAR,
			G2Core::FilterMode::LINEAR);

		renderComp->material.setTexture(G2::Sampler::DIFFUSE, tex);
	}
	show();
}

void
MaterialPropertiesTab::selectAmbient()
{
	Json::Value& props = mEntity->getProperties(mTechnicalName);

	// calculate the selected alpha separately, since it is not stored in the background palette.
	int alpha = 255;
	if (props.isMember(MAT_AMBIENT))
	{
		alpha = props[MAT_AMBIENT]["a"].asInt();
	}
	QColor color = ui.ambientSurf->palette().color(QPalette::Background);
	color.setAlpha(alpha);
	color = QColorDialog::getColor(color, this, "Select ambient color", QColorDialog::ShowAlphaChannel);

	if (color.isValid())
	{
		QPalette pal;
		pal.setColor(QPalette::Background, color);
		ui.diffuseSurf->setPalette(pal);

		props[MAT_AMBIENT]["r"] = color.red();
		props[MAT_AMBIENT]["g"] = color.green();
		props[MAT_AMBIENT]["b"] = color.blue();
		props[MAT_AMBIENT]["a"] = color.alpha();
		_reimportMaterial(mEntity, false);
		mProject->getCurrentScene()->save();
	}
}

void
MaterialPropertiesTab::selectDiffuse()
{
	Json::Value& props = mEntity->getProperties(mTechnicalName);

	// calculate the selected alpha separately, since it is not stored in the background palette.
	int alpha = 255;
	if (props.isMember(MAT_DIFFUSE))
	{
		alpha = props[MAT_DIFFUSE]["a"].asInt();
	}
	QColor color = ui.diffuseSurf->palette().color(QPalette::Background);
	color.setAlpha(alpha);
	color = QColorDialog::getColor(color, this, "Select diffuse color", QColorDialog::ShowAlphaChannel);

	if (color.isValid())
	{
		QPalette pal;
		pal.setColor(QPalette::Background, color);
		ui.diffuseSurf->setPalette(pal);

		props[MAT_DIFFUSE]["r"] = color.red();
		props[MAT_DIFFUSE]["g"] = color.green();
		props[MAT_DIFFUSE]["b"] = color.blue();
		props[MAT_DIFFUSE]["a"] = color.alpha();
		_reimportMaterial(mEntity, false);
		mProject->getCurrentScene()->save();
	}
}

void
MaterialPropertiesTab::selectSpecular()
{
	Json::Value& props = mEntity->getProperties(mTechnicalName);

	// calculate the selected alpha separately, since it is not stored in the background palette.
	int alpha = 255;
	if (props.isMember(MAT_SPECULAR))
	{
		alpha = props[MAT_SPECULAR]["a"].asInt();
	}
	QColor color = ui.specularSurf->palette().color(QPalette::Background);
	color.setAlpha(alpha);
	color = QColorDialog::getColor(color, this, "Select specular color", QColorDialog::ShowAlphaChannel);

	if (color.isValid())
	{
		QPalette pal;
		pal.setColor(QPalette::Background, color);
		ui.specularSurf->setPalette(pal);

		props[MAT_SPECULAR]["r"] = color.red();
		props[MAT_SPECULAR]["g"] = color.green();
		props[MAT_SPECULAR]["b"] = color.blue();
		props[MAT_SPECULAR]["a"] = color.alpha();
		_reimportMaterial(mEntity, false);
		mProject->getCurrentScene()->save();
	}
}

void MaterialPropertiesTab::shininessSliderChanged(int value)
{
	// sync
	ui.shininessValue->blockSignals(true); ui.shininessValue->setValue((double)value); ui.shininessValue->blockSignals(false);

	_serializeShininess();

	_reimportMaterial(mEntity, false);
	mProject->getCurrentScene()->save();
}

void MaterialPropertiesTab::shininessValueChanged(double value)
{
	// sync
	ui.shininessSlider->blockSignals(true); ui.shininessSlider->setValue((int)value); ui.shininessSlider->blockSignals(false);
	
	_serializeShininess();

	_reimportMaterial(mEntity, false);
	mProject->getCurrentScene()->save();
}

void MaterialPropertiesTab::_serializeShininess()
{
	Json::Value& props = mEntity->getProperties(mTechnicalName);
	props[MAT_SHININESS] = (float)ui.shininessValue->value();
}

/*
void MaterialPropertiesTab::selectDiffuseTex()
{
	if (hasEntity())
	{
		std::string dialogDir = mProjectDirectory + "/assets/textures";
		QString filePath = QFileDialog::getOpenFileName(this, "Select diffuse texture file", dialogDir.c_str());
		if (!filePath.isNull())
		{
			Json::Value& props = mEntity->getProperties(mTechnicalName);
			// 
			std::string fullPath = filePath.toStdString();

			if (!boost::algorithm::starts_with(fullPath, mProjectDirectory))
			{
				std::cout << "[Texture] Selected file is not contained in the project directory!" << std::endl;
			}
			else
			{

				// update ui
				QImage image(fullPath.c_str());
				if (!image.isNull())
				{
					//std::cout << ui.label_6->width() << " --- " << ui.label_6->height() << std::endl;
					image = image.scaled(ui.diffuseTexSurf->width(), ui.diffuseTexSurf->width(), Qt::KeepAspectRatio);
					mEntity->imageCache[G2::Sampler::DIFFUSE] = image;
					ui.diffuseTexSurf->setPixmap(QPixmap::fromImage(image));


					// strip project directory
					boost::replace_first(fullPath, mProjectDirectory, "");
					props[TEX_DIFFUSE] = fullPath;

					// release the caching entry for the texture to reimport it from scratch
					mTextureImporter.clearCache(mProjectDirectory + fullPath);
					_reimportMaterial(mEntity, false);
					mProject->getCurrentScene()->save();
				}
			}
		}
	}
}*/

void MaterialPropertiesTab::selectEffect()
{
	if (hasEntity())
	{
		std::string dialogDir = mProjectDirectory + "/assets/shader";
		QString effectPath = QFileDialog::getOpenFileName(this, "Select effect file", dialogDir.c_str(), "GEAR Effect (*.g2fx)");
		if (!effectPath.isNull())
		{
			Json::Value& props = mEntity->getProperties(mTechnicalName);
			// 
			std::string fullPath = effectPath.toStdString();

			if (!boost::algorithm::starts_with(fullPath, mProjectDirectory))
			{
				std::cout << "[Effect] Selected file is not contained in the project directory!" << std::endl;
			}
			else
			{
				// strip project directory
				boost::replace_first(fullPath, mProjectDirectory, "");
				props[FX_PATH] = fullPath;

				// update ui
				ui.effectPath->setText(fullPath.c_str());

				// release the caching entry for the effect to reimport it from scratch
				mFxImporter.clearCache(mProjectDirectory + fullPath);
				_reimportMaterial(mEntity, true);
				mProject->getCurrentScene()->save();
			}
		}
	}
}

void MaterialPropertiesTab::addTextureSelector()
{
	mTextureSelector.push_back(std::shared_ptr<TextureSelector>(new TextureSelector(mProjectDirectory, this)));
	ui.textureSelectorRoot->layout()->addWidget(mTextureSelector.back().get());
}

void MaterialPropertiesTab::selectTexture(std::shared_ptr<TextureSelector>& selector)
{

}