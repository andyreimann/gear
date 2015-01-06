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
	PropertiesTab("transf")
{
	ui.setupUi(this);
	ui.tabToggle->setText("Transformation"); // set display name on tab toggle

	connect(ui.tabToggle, SIGNAL(clicked()), this, SLOT(toggleTab()));

	connect(ui.posX, SIGNAL(valueChanged(double)), this, SLOT(posXChanged(double)));
	connect(ui.posY, SIGNAL(valueChanged(double)), this, SLOT(posYChanged(double)));
	connect(ui.posZ, SIGNAL(valueChanged(double)), this, SLOT(posZChanged(double)));
	connect(ui.rotX, SIGNAL(valueChanged(double)), this, SLOT(rotXChanged(double)));
	connect(ui.rotY, SIGNAL(valueChanged(double)), this, SLOT(rotYChanged(double)));
	connect(ui.rotZ, SIGNAL(valueChanged(double)), this, SLOT(rotZChanged(double)));
	connect(ui.scaleX, SIGNAL(valueChanged(double)), this, SLOT(scaleXChanged(double)));
	connect(ui.scaleY, SIGNAL(valueChanged(double)), this, SLOT(scaleYChanged(double)));
	connect(ui.scaleZ, SIGNAL(valueChanged(double)), this, SLOT(scaleZChanged(double)));
}

TransformationPropertiesTab::~TransformationPropertiesTab()
{
	//GEARStudioEvents::onManagedEntitySelected.unHookAll(this);
}

void TransformationPropertiesTab::_initWithEntity(ManagedEntity* entity)
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
			ui.posX->setValue(props[TRANS_POSITION].get("x", "0.0").asFloat());
			ui.posY->setValue(props[TRANS_POSITION].get("y", "0.0").asFloat());
			ui.posZ->setValue(props[TRANS_POSITION].get("z", "0.0").asFloat());
		}
		if (props.isMember(TRANS_ORIENTATION))
		{
			ui.rotX->setValue(props[TRANS_ORIENTATION].get("x", "0.0").asFloat());
			ui.rotY->setValue(props[TRANS_ORIENTATION].get("y", "0.0").asFloat());
			ui.rotZ->setValue(props[TRANS_ORIENTATION].get("z", "0.0").asFloat());
		}
		if (props.isMember(TRANS_SCALE))
		{
			ui.scaleX->setValue(props[TRANS_SCALE].get("x", "1.0").asFloat());
			ui.scaleY->setValue(props[TRANS_SCALE].get("y", "1.0").asFloat());
			ui.scaleZ->setValue(props[TRANS_SCALE].get("z", "1.0").asFloat());
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
		auto* trans = entity->addComponent<G2::TransformComponent>();
		trans->setPosition(glm::vec3((float)ui.posX->value(), (float)ui.posY->value(), (float)ui.posZ->value()));

		glm::quat rotation;
		rotation = glm::cross(rotation, glm::angleAxis((float)ui.rotX->value(), glm::vec3(1.f, 0.f, 0.f)));
		rotation = glm::cross(rotation, glm::angleAxis((float)ui.rotY->value(), glm::vec3(0.f, 1.f, 0.f)));
		rotation = glm::cross(rotation, glm::angleAxis((float)ui.rotZ->value(), glm::vec3(0.f, 0.f, 1.f)));

		trans->setRotation(rotation);

		trans->setScale(glm::vec3((float)ui.scaleX->value(), (float)ui.scaleY->value(), (float)ui.scaleZ->value()));

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

void TransformationPropertiesTab::posXChanged(double)
{
	if (hasEntity())
	{
		_serializeValue(TRANS_POSITION, "x", ui.posX->value());
		_instantiateFromDescription(mEntity);
		mProject->getCurrentScene()->save();
	}
}

void TransformationPropertiesTab::posYChanged(double)
{
	if (hasEntity())
	{
		_serializeValue(TRANS_POSITION, "y", ui.posY->value());
		_instantiateFromDescription(mEntity);
		mProject->getCurrentScene()->save();
	}
}

void TransformationPropertiesTab::posZChanged(double)
{
	if (hasEntity())
	{
		_serializeValue(TRANS_POSITION, "z", ui.posZ->value());
		_instantiateFromDescription(mEntity);
		mProject->getCurrentScene()->save();
	}
}

void TransformationPropertiesTab::rotXChanged(double)
{
	if (hasEntity())
	{
		_serializeValue(TRANS_ORIENTATION, "x", ui.rotX->value());
		_instantiateFromDescription(mEntity);
		mProject->getCurrentScene()->save();
	}
}

void TransformationPropertiesTab::rotYChanged(double)
{
	if (hasEntity())
	{
		_serializeValue(TRANS_ORIENTATION, "y", ui.rotY->value());
		_instantiateFromDescription(mEntity);
		mProject->getCurrentScene()->save();
	}
}

void TransformationPropertiesTab::rotZChanged(double)
{
	if (hasEntity())
	{
		_serializeValue(TRANS_ORIENTATION, "z", ui.rotZ->value());
		_instantiateFromDescription(mEntity);
		mProject->getCurrentScene()->save();
	}
}

void TransformationPropertiesTab::scaleXChanged(double)
{
	if (hasEntity())
	{
		_serializeValue(TRANS_SCALE, "x", ui.scaleX->value());
		_instantiateFromDescription(mEntity);
		mProject->getCurrentScene()->save();
	}
}

void TransformationPropertiesTab::scaleYChanged(double)
{
	if (hasEntity())
	{
		_serializeValue(TRANS_SCALE, "y", ui.scaleY->value());
		_instantiateFromDescription(mEntity);
		mProject->getCurrentScene()->save();
	}
}

void TransformationPropertiesTab::scaleZChanged(double)
{
	if (hasEntity())
	{
		_serializeValue(TRANS_SCALE, "z", ui.scaleZ->value());
		_instantiateFromDescription(mEntity);
		mProject->getCurrentScene()->save();
	}
}

void TransformationPropertiesTab::_serializeValue(std::string const& group, std::string const& component, double value)
{
	Json::Value& props = mEntity->getProperties(mTechnicalName);
	props[group][component] = (float)value;
}