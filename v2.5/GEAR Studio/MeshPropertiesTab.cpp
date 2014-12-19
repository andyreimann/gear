#include "MeshPropertiesTab.h"
#include "GEARStudioEvents.h"

#include <G2/RenderComponent.h>

#include <QtWidgets/QFileDialog>


MeshPropertiesTab::MeshPropertiesTab(QWidget *parent /*= 0*/)
	: QWidget(parent),
	PropertiesTab("mesh")
{
	ui.setupUi(this);
	ui.tabToggle->setText("Mesh"); // set display name on tab toggle

	connect(ui.tabToggle, SIGNAL(clicked()), this, SLOT(toggleTab()));
	connect(ui.selectMesh, SIGNAL(clicked()), this, SLOT(selectMesh()));

	//GEARStudioEvents::onManagedEntitySelected.hook(this, &MeshPropertiesTab::_init);
}

MeshPropertiesTab::~MeshPropertiesTab()
{
	//GEARStudioEvents::onManagedEntitySelected.unHookAll(this);
}

void MeshPropertiesTab::_initWithEntity(ManagedEntity* entity)
{
	if (!hasEntity() || !entity->hasProperties(mTechnicalName))
	{
		hide();
	}
	else
	{
		show();
		Json::Value const& props = entity->getProperties(mTechnicalName);

		if (props.isMember("mesh_path"))
		{
			ui.meshPath->setText(props["mesh_path"]);
		}
		else
		{
			ui.meshPath->setText(""); // no mesh selected yet
		}

		if (props.isMember("flip_tex_u"))
		{
			ui.flipUTexCoord->setChecked(props["flip_tex_u"].asBool());
		}
		else { ui.flipUTexCoord->setChecked(false); }

		if (props.isMember("flip_tex_v"))
		{
			ui.flipVTexCoord->setChecked(props["flip_tex_v"].asBool());
		}
		else { ui.flipVTexCoord->setChecked(false); }
	}
}

void MeshPropertiesTab::_instantiateFromDescription(ManagedEntity* entity)
{
	Json::Value const& props = entity->getProperties(mTechnicalName);
	// do everything the MeshPropertiesTab should do on a ManagedEntity
	// 1. load a mesh
	if (props.isMember("mesh_path"))
	{
		// do load the mesh
	}
}

void MeshPropertiesTab::toggleTab()
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

void MeshPropertiesTab::selectMesh()
{
	if (hasEntity())
	{
		QString meshPath = QFileDialog::getOpenFileName(this, "Select mesh file", "", "FBX (*.fbx);;MD5 (*.md5)");
		if (!meshPath.isNull())
		{
			Json::Value const& props = mEntity->getProperties(mTechnicalName);
			props["mesh_path"] = meshPath.toStdString();
			// load the mesh and attach it to the RenderComponent
			// TODO How to remove a previously assigned mesh? Clear function on RenderComponent is missing!
		}
	}
}