#include "MeshPropertiesTab.h"
#include "GEARStudioEvents.h"

#include <G2/RenderComponent.h>

#include <QtWidgets/QFileDialog>
#include <boost/algorithm/string/predicate.hpp>
#include <algorithm>

// TODO Should these identifier be compiled into a separated DLL?
static const std::string IMPORT_NORMALS = "normals";
static const std::string IMPORT_TEX_COORDS = "tex_coords";
static const std::string FLIP_TEX_U = "flip_tex_u";
static const std::string FLIP_TEX_V = "flip_tex_v";
static const std::string MESH_PATH = "mesh_path";
static const std::string RENDER_LAYER = "render_layer";

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
		ui.meshPath->setText(props.get(MESH_PATH, "").asCString());
		ui.flipUTexCoord->setChecked(props.get(FLIP_TEX_U, false).asBool());
		ui.flipVTexCoord->setChecked(props.get(FLIP_TEX_V, false).asBool());
	}
}

void MeshPropertiesTab::_instantiateFromDescription(ManagedEntity* entity)
{
	// do everything the MeshPropertiesTab should do on a ManagedEntity
	_reimportMesh(entity);
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
			Json::Value& props = mEntity->getProperties(mTechnicalName);
			props[MESH_PATH] = meshPath.toStdString();
			
			// after mesh is selected, reimport the mesh to update the RenderComponent
			_reimportMesh(mEntity);
		}
	}
}

void MeshPropertiesTab::_reimportMesh(ManagedEntity* target)
{
	if (!target->hasProperties(mTechnicalName))
	{
		return; // we are not responsible for that entity
	}

	Json::Value const& props = target->getProperties(mTechnicalName);
	// do load the mesh
	std::string meshPath = props.get(MESH_PATH, "").asString();
	std::transform(meshPath.begin(), meshPath.end(), meshPath.begin(), ::tolower);

	// TODO Check if file exists
	if (boost::algorithm::ends_with(meshPath, "fbx"))
	{
		// load with FBX importer and let him attach the imported mesh to the entity pointer
		mFbxImporter.import(
			mProjectDirectory + "/" + props.get(MESH_PATH, "").asString(),
			props.get(IMPORT_NORMALS, true).asBool(),
			props.get(IMPORT_TEX_COORDS, true).asBool(),
			true,
			props.get(FLIP_TEX_U, false).asBool(),
			props.get(FLIP_TEX_V, false).asBool(),
			nullptr,
			target);
	}
	else if (boost::algorithm::ends_with(meshPath, "md5"))
	{
		// TODO How to remove a previously assigned mesh? Clear function on RenderComponent is missing!
		// TODO MD5Importer does not yet have support for tex coord flip!
		mMd5Importer.import(
			mProjectDirectory + "/" + props.get(MESH_PATH, "").asString(),
			props.get(IMPORT_NORMALS, true).asBool(),
			props.get(IMPORT_TEX_COORDS, true).asBool(),
			true,
			std::vector<std::string>(),
			target);
	}
	else
	{
		// TODO log warning
	}
}