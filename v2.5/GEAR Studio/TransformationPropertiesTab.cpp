#include "TransformationPropertiesTab.h"
#include "GEARStudioEvents.h"

#include <G2/TransformComponent.h>

#include <QtWidgets/QFileDialog>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <glm/ext.hpp>

// TODO Should these identifier be compiled into a separated DLL?
static const std::string TRANS_TYPE = "type";
static const std::string TRANS_POSITION = "pos";
static const std::string TRANS_ORIENTATION = "rot";
static const std::string TRANS_SCALE = "scale";

TransformationPropertiesTab::TransformationPropertiesTab(QWidget *parent /*= 0*/)
	: QWidget(parent),
	PropertiesTab("transf", "Transformation"),
	mTranslationHandleChanged(false)
{
	ui.setupUi(this);
	ui.tabToggle->setText(mTabName.c_str()); // set display name on tab toggle

	connect(ui.tabToggle, SIGNAL(clicked()), this, SLOT(toggleTab()));

	// connect to all input boxes to serialize on changes
	connect(ui.posX, SIGNAL(valueChanged(double)), this, SLOT(posChanged(double)));
	connect(ui.posY, SIGNAL(valueChanged(double)), this, SLOT(posChanged(double)));
	connect(ui.posZ, SIGNAL(valueChanged(double)), this, SLOT(posChanged(double)));
	connect(ui.rotX, SIGNAL(valueChanged(double)), this, SLOT(rotChanged(double)));
	connect(ui.rotY, SIGNAL(valueChanged(double)), this, SLOT(rotChanged(double)));
	connect(ui.rotZ, SIGNAL(valueChanged(double)), this, SLOT(rotChanged(double)));
	connect(ui.scaleX, SIGNAL(valueChanged(double)), this, SLOT(scaleChanged(double)));
	connect(ui.scaleY, SIGNAL(valueChanged(double)), this, SLOT(scaleChanged(double)));
	connect(ui.scaleZ, SIGNAL(valueChanged(double)), this, SLOT(scaleChanged(double)));

	// register to Editor events
	GEARStudioEvents::onTranslationHandleMoved.hook(this, &TransformationPropertiesTab::_onTranslationHandleMoved);
	GEARStudioEvents::onTranslationHandleReleased.hook(this, &TransformationPropertiesTab::_onTranslationHandleReleased);
}


TransformationPropertiesTab::~TransformationPropertiesTab()
{
	GEARStudioEvents::onTranslationHandleMoved.unHookAll(this);
	GEARStudioEvents::onTranslationHandleReleased.unHookAll(this);
}

void TransformationPropertiesTab::_initUiWithEntity(ManagedEntity* entity)
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
		 * Hence we will have a fall back on the default values for every single	*
		 * property we need for initialization of the UI of this tab.			*
		 ************************************************************************/
		show();
		Json::Value const& props = entity->getProperties(mTechnicalName);

		// here we check for every member we expect inside of our property.
		// If one is absent, we just initialize it to it's default value.
		if (props.isMember(TRANS_POSITION))
		{
			ui.posX->blockSignals(true); ui.posX->setValue(props[TRANS_POSITION].get("x", "0.0").asFloat()); ui.posX->blockSignals(false);
			ui.posY->blockSignals(true); ui.posY->setValue(props[TRANS_POSITION].get("y", "0.0").asFloat()); ui.posY->blockSignals(false);
			ui.posZ->blockSignals(true); ui.posZ->setValue(props[TRANS_POSITION].get("z", "0.0").asFloat()); ui.posZ->blockSignals(false);
		}
		else
		{
			// init UI with default
			ui.posX->blockSignals(true); ui.posX->setValue(0.f); ui.posX->blockSignals(false);
			ui.posY->blockSignals(true); ui.posY->setValue(0.f); ui.posY->blockSignals(false);
			ui.posZ->blockSignals(true); ui.posZ->setValue(0.f); ui.posZ->blockSignals(false);
		}
		if (props.isMember(TRANS_ORIENTATION))
		{
			ui.rotX->blockSignals(true); ui.rotX->setValue(props[TRANS_ORIENTATION].get("x", "0.0").asFloat()); ui.rotX->blockSignals(false);
			ui.rotY->blockSignals(true); ui.rotY->setValue(props[TRANS_ORIENTATION].get("y", "0.0").asFloat()); ui.rotY->blockSignals(false);
			ui.rotZ->blockSignals(true); ui.rotZ->setValue(props[TRANS_ORIENTATION].get("z", "0.0").asFloat()); ui.rotZ->blockSignals(false);
		}
		else
		{
			// init UI with default
			ui.rotX->blockSignals(true); ui.rotX->setValue(0.f); ui.rotX->blockSignals(false);
			ui.rotY->blockSignals(true); ui.rotY->setValue(0.f); ui.rotY->blockSignals(false);
			ui.rotZ->blockSignals(true); ui.rotZ->setValue(0.f); ui.rotZ->blockSignals(false);
		}
		if (props.isMember(TRANS_SCALE))
		{
			ui.scaleX->blockSignals(true); ui.scaleX->setValue(props[TRANS_SCALE].get("x", "1.0").asFloat()); ui.scaleX->blockSignals(false);
			ui.scaleY->blockSignals(true); ui.scaleY->setValue(props[TRANS_SCALE].get("y", "1.0").asFloat()); ui.scaleY->blockSignals(false);
			ui.scaleZ->blockSignals(true); ui.scaleZ->setValue(props[TRANS_SCALE].get("z", "1.0").asFloat()); ui.scaleZ->blockSignals(false);
		}
		else
		{
			// init UI with default
			ui.scaleX->blockSignals(true); ui.scaleX->setValue(1.f); ui.scaleX->blockSignals(false);
			ui.scaleY->blockSignals(true); ui.scaleY->setValue(1.f); ui.scaleY->blockSignals(false);
			ui.scaleZ->blockSignals(true); ui.scaleZ->setValue(1.f); ui.scaleZ->blockSignals(false);
		}
	}
}

void TransformationPropertiesTab::_instantiateFromDescription(ManagedEntity* entity)
{
	// do everything the TransformationPropertiesTab should do on a ManagedEntity
	if (entity == nullptr)
	{
		// TODO Disable everything?
		return;
	}

	if (!entity->hasProperties(mTechnicalName))
	{
		entity->removeComponent<G2::TransformComponent>();
	}
	else
	{
		Json::Value const& props = entity->getProperties(mTechnicalName);

		auto* trans = entity->addComponent<G2::TransformComponent>();
		if (props.isMember(TRANS_POSITION))
		{
			trans->setPosition(glm::vec3(props[TRANS_POSITION]["x"].asFloat(), props[TRANS_POSITION]["y"].asFloat(), props[TRANS_POSITION]["z"].asFloat()));
		}
		if (props.isMember(TRANS_ORIENTATION))
		{
			glm::quat rotation;
			rotation = glm::cross(rotation, glm::angleAxis(props[TRANS_ORIENTATION]["x"].asFloat(), glm::vec3(1.f, 0.f, 0.f)));
			rotation = glm::cross(rotation, glm::angleAxis(props[TRANS_ORIENTATION]["y"].asFloat(), glm::vec3(0.f, 1.f, 0.f)));
			rotation = glm::cross(rotation, glm::angleAxis(props[TRANS_ORIENTATION]["z"].asFloat(), glm::vec3(0.f, 0.f, 1.f)));

			trans->setRotation(rotation);
		}
		if (props.isMember(TRANS_SCALE))
		{
			trans->setScale(glm::vec3(props[TRANS_SCALE]["x"].asFloat(), props[TRANS_SCALE]["y"].asFloat(), props[TRANS_SCALE]["z"].asFloat()));
		}
	}
}

void TransformationPropertiesTab::toggleTab()
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

void TransformationPropertiesTab::posChanged(double)
{
	if (hasEntity())
	{
		_serializeValue(TRANS_POSITION, "x", ui.posX->value());
		_serializeValue(TRANS_POSITION, "y", ui.posY->value());
		_serializeValue(TRANS_POSITION, "z", ui.posZ->value());
		_instantiateFromDescription(mEntity);
		// TODO WARNING: Saving everytime we change a single value will not work on large projects -> dirty flag and ctrl + S support needed!
		mProject->getCurrentScene()->save();
	}
}

void TransformationPropertiesTab::rotChanged(double)
{
	if (hasEntity())
	{
		_serializeValue(TRANS_ORIENTATION, "x", ui.rotX->value());
		_serializeValue(TRANS_ORIENTATION, "y", ui.rotY->value());
		_serializeValue(TRANS_ORIENTATION, "z", ui.rotZ->value());
		_instantiateFromDescription(mEntity);
		// TODO WARNING: Saving everytime we change a single value will not work on large projects -> dirty flag and ctrl + S support needed!
		mProject->getCurrentScene()->save();
	}
}

void TransformationPropertiesTab::scaleChanged(double)
{
	if (hasEntity())
	{
		_serializeValue(TRANS_SCALE, "x", ui.scaleX->value());
		_serializeValue(TRANS_SCALE, "y", ui.scaleY->value());
		_serializeValue(TRANS_SCALE, "z", ui.scaleZ->value());
		_instantiateFromDescription(mEntity);
		// TODO WARNING: Saving everytime we change a single value will not work on large projects -> dirty flag and ctrl + S support needed!
		mProject->getCurrentScene()->save();
	}
}

void TransformationPropertiesTab::_serializeValue(std::string const& group, std::string const& component, double value)
{
	Json::Value& props = mEntity->getProperties(mTechnicalName);
	props[group][component] = (float)value;
}

void
TransformationPropertiesTab::_onTranslationHandleMoved()
{
	Json::Value& props = mEntity->getProperties(mTechnicalName);
	
	auto* tc = mEntity->getComponent<G2::TransformComponent>();

	_serializeValue(TRANS_POSITION, "x", tc->getPosition().x);
	_serializeValue(TRANS_POSITION, "y", tc->getPosition().y);
	_serializeValue(TRANS_POSITION, "z", tc->getPosition().z);

	_initUiWithEntity(mEntity);

	mTranslationHandleChanged = true;
}

void
TransformationPropertiesTab::_onTranslationHandleReleased()
{
	if (mTranslationHandleChanged)
	{
		mTranslationHandleChanged = false;
		mProject->getCurrentScene()->save();
	}
}