#include "CameraPropertiesTab.h"
#include "GEARStudioEvents.h"
#include "EditorGeometryManager.h"

#include <G2/CameraComponent.h>
#include <boost/algorithm/string/replace.hpp>


// TODO Should these identifier be compiled into a separated DLL?
static const std::string CAM_NAME = "n";
static const std::string CAM_NEAR_CLIP = "ncp";
static const std::string CAM_FAR_CLIP = "fcp";
static const std::string CAM_FOV = "fov";
static const std::string CAM_IS_MAIN = "main";

CameraPropertiesTab::CameraPropertiesTab(QWidget *parent /*= 0*/)
	: QWidget(parent),
	PropertiesTab("cam","Camera")
{
	ui.setupUi(this);
	ui.tabToggle->setText(mTabName.c_str()); // set display name on tab toggle

	connect(ui.tabToggle, SIGNAL(clicked()), this, SLOT(toggleTab()));

	connect(ui.zNear, SIGNAL(valueChanged(double)), this, SLOT(zNearChanged(double)));
	connect(ui.zFar, SIGNAL(valueChanged(double)), this, SLOT(zFarChanged(double)));
	connect(ui.fov, SIGNAL(valueChanged(double)), this, SLOT(fieldOfViewChanged(double)));
	connect(ui.mainCamera, SIGNAL(stateChanged(int)), this, SLOT(mainCameraFlagChanged(int)));
	connect(ui.name, SIGNAL(textChanged(QString const&)), this, SLOT(nameChanged(QString const&)));

	G2S::onGenerateCppCodeForManagedEntity.hook(this, &CameraPropertiesTab::_onGenerateCppCodeForManagedEntity);
}

CameraPropertiesTab::~CameraPropertiesTab()
{
	G2S::onGenerateCppCodeForManagedEntity.unHookAll(this);
}

void CameraPropertiesTab::_initUiWithEntity(ManagedEntity* entity)
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
		 * Hence we will have a fall back on the default values for every single*
		 * property we need for initialization of the UI of this tab.			*
		 ************************************************************************/
		show();
		Json::Value const& props = entity->getProperties(mTechnicalName);

		GEAR_QTNOEVENT(ui.name, ui.name->setText(props.get(CAM_NAME, "Camera").asCString()));
		GEAR_QTNOEVENT(ui.zNear, ui.zNear->setValue(props.get(CAM_NEAR_CLIP, 0.1f).asFloat()));
		GEAR_QTNOEVENT(ui.zFar, ui.zFar->setValue(props.get(CAM_FAR_CLIP, 100.f).asFloat()));
		GEAR_QTNOEVENT(ui.fov, ui.fov->setValue(props.get(CAM_FOV, 70.f).asFloat()));
		GEAR_QTNOEVENT(ui.mainCamera, ui.mainCamera->setChecked(props.get(CAM_IS_MAIN, false).asBool()));
	}
}

void CameraPropertiesTab::_instantiateFromDescription(ManagedEntity* entity)
{
	// do everything the TransformationPropertiesTab should do on a ManagedEntity
	if (entity == nullptr)
	{
		// TODO Disable everything?
		return;
	}

	if (!entity->hasProperties(mTechnicalName))
	{
		entity->removeComponent<G2::CameraComponent>();
	}
	else
	{
		Json::Value const& props = entity->getProperties(mTechnicalName);

		entity->addComponent<G2::TransformComponent>();
		entity->removeComponent<G2::CameraComponent>();
		auto* cam = entity->addComponent<G2::CameraComponent>(props.get(CAM_NAME, "Camera").asString());
		cam->setProjectionMatrix(100, 100,
			props.get(CAM_NEAR_CLIP, 0.1f).asFloat(),
			props.get(CAM_FAR_CLIP, 100.f).asFloat(),
			props.get(CAM_FOV, 70.f).asFloat());

		if (!entity->hasComponent<G2::RenderComponent>())
		{
			mFbxImporter.import(
				"meshes/camera_wire.fbx",
				false,
				false,
				false,
				false,
				false,
				nullptr,
				entity);

			auto* rc = entity->getComponent<G2::RenderComponent>();
			rc->setEffect(mEffectImporter.import("shader/Solid.g2fx"));
			rc->setRenderLayerMask(~EditorGeometryManager::gEditorGeometryLayers);
		}
	}
}


void CameraPropertiesTab::zNearChanged(double value)
{
	if (hasEntity())
	{
		Json::Value& props = mEntity->getProperties(mTechnicalName);
		props[CAM_NEAR_CLIP] = (float)value;
		_instantiateFromDescription(mEntity);

		if (value > ui.zFar->value())
		{
			ui.zFar->setValue(value + 0.01);
		}
		
		mProject->getCurrentScene()->save();
	}
}

void CameraPropertiesTab::zFarChanged(double value)
{
	if (hasEntity())
	{
		Json::Value& props = mEntity->getProperties(mTechnicalName);
		props[CAM_FAR_CLIP] = (float)value;
		_instantiateFromDescription(mEntity);

		if (value < ui.zNear->value())
		{
			ui.zNear->setValue(value - 0.01);
		}
		mProject->getCurrentScene()->save();
	}
}

void CameraPropertiesTab::fieldOfViewChanged(double value)
{
	if (hasEntity())
	{
		Json::Value& props = mEntity->getProperties(mTechnicalName);
		props[CAM_FOV] = (float)value;
		_instantiateFromDescription(mEntity);
		
		mProject->getCurrentScene()->save();
	}
}

void CameraPropertiesTab::mainCameraFlagChanged(int state)
{
	if (hasEntity())
	{
		Json::Value& props = mEntity->getProperties(mTechnicalName);
		props[CAM_IS_MAIN] = (state == Qt::Checked);
		_instantiateFromDescription(mEntity);
		
		mProject->getCurrentScene()->save();
	}
}

void CameraPropertiesTab::nameChanged(QString const& name)
{
	if (hasEntity())
	{
		Json::Value& props = mEntity->getProperties(mTechnicalName);
		props[CAM_NAME] = name.toStdString();
		_instantiateFromDescription(mEntity);

		mProject->getCurrentScene()->save();
	}
}

void CameraPropertiesTab::toggleTab()
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

void CameraPropertiesTab::_onGenerateCppCodeForManagedEntity(ManagedEntity const* entity, std::string const& entityVar, std::ostream& out)
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
		out << indention << "// Camera" << std::endl;
		std::string name = props.get(CAM_NAME, "Camera").asString();

		boost::replace_all(name, "\\", "\\\\");
		boost::replace_all(name, "\"", "\\\"");
		out << indention << "auto* cam = " << entityVar << ".addComponent<CameraComponent>(\"" << name << "\");" << std::endl;
		out << indention << "cam->setProjectionMatrix(1920,1080," << props.get(CAM_NEAR_CLIP, 0.1f).asFloat() << "f," << props.get(CAM_FAR_CLIP, 100.f).asFloat() << "f," << props.get(CAM_FOV, 70.f).asFloat() << "f);" << std::endl;
		if (props.isMember(CAM_IS_MAIN) && props[CAM_IS_MAIN].asBool() == true)
		{
			out << indention << "cam->setAsRenderCamera();" << std::endl;
		}
	}
	out << "		}" << std::endl;
}