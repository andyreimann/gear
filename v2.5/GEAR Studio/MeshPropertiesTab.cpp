#include "MeshPropertiesTab.h"
#include "GEARStudioEvents.h"
#include "EditorGeometryManager.h"

#include <G2/RenderComponent.h>

#include <QtWidgets/QFileDialog>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>

// TODO Should these identifier be compiled into a separated DLL?
static const std::string IMPORT_NORMALS = "normals";
static const std::string IMPORT_TEX_COORDS = "tex_coords";
static const std::string FLIP_TEX_U = "flip_tex_u";
static const std::string FLIP_TEX_V = "flip_tex_v";
static const std::string MESH_PATH = "path";
static const std::string RENDER_LAYER = "render_layer";

MeshPropertiesTab::MeshPropertiesTab(QWidget *parent /*= 0*/)
	: QWidget(parent),
	PropertiesTab("mesh","Mesh")
{
	ui.setupUi(this);
	ui.tabToggle->setText(mTabName.c_str()); // set display name on tab toggle

	connect(ui.tabToggle, SIGNAL(clicked()), this, SLOT(toggleTab()));

	mMeshSelector = std::shared_ptr<FileSelector>(new FileSelector("/assets/meshes", Json::Value(), mProjectDirectory, "FBX (*.fbx);;MD5 (*.md5)", this));
	mMeshSelector->onFileSelected.hook(this, &MeshPropertiesTab::_meshSelected);
	ui.meshFileRoot->layout()->addWidget(mMeshSelector.get());

	G2S::onGenerateCppCodeForManagedEntity.hook(this, &MeshPropertiesTab::_onGenerateCppCodeForManagedEntity);
}

MeshPropertiesTab::~MeshPropertiesTab()
{
	G2S::onGenerateCppCodeForManagedEntity.unHookAll(this);
}

void MeshPropertiesTab::_initUiWithEntity(ManagedEntity* entity)
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

		mMeshSelector->setData(props);

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

void MeshPropertiesTab::_meshSelected(FileSelector* fileSelector)
{
	if (hasEntity() && fileSelector != nullptr)
	{
		Json::Value& props = mEntity->getProperties(mTechnicalName);
		props[MESH_PATH] = fileSelector->getData()[MESH_PATH];
		// release the caching entry for the effect to reimport it from scratch
		mFbxImporter.clearCache(mProjectDirectory + props[MESH_PATH].asString());
		mMd5Importer.clearCache(mProjectDirectory + props[MESH_PATH].asString());
		// after mesh is selected, reimport the mesh to update the RenderComponent
		_reimportMesh(mEntity);

		mProject->getCurrentScene()->save();
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
		// clear any meshes already loaded into the RenderComponent 
		auto renderComponent = target->getComponent<G2::RenderComponent>();
		if (renderComponent != nullptr)
		{
			renderComponent->removeAllMeshes();
		}
		// load with FBX importer and let him attach the imported mesh to the entity pointer
		mFbxImporter.import(
			mProjectDirectory + props.get(MESH_PATH, "").asString(),
			props.get(IMPORT_NORMALS, true).asBool(),
			props.get(IMPORT_TEX_COORDS, true).asBool(),
			false, // NO ANIMATIONS SO FAR!
			props.get(FLIP_TEX_U, false).asBool(),
			props.get(FLIP_TEX_V, false).asBool(),
			nullptr,
			target);
	}
	else if (boost::algorithm::ends_with(meshPath, "md5"))
	{
		// clear any meshes already loaded into the RenderComponent 
		auto renderComponent = target->getComponent<G2::RenderComponent>();
		if (renderComponent != nullptr)
		{
			renderComponent->removeAllMeshes();
		}
		// TODO MD5Importer does not yet have support for tex coord flip!
		mMd5Importer.import(
			mProjectDirectory + props.get(MESH_PATH, "").asString(),
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

	if (target->hasComponent<G2::RenderComponent>())
	{
		// make sure the geometry in the scene does not share any render layer with the editor geometry 
		// by assigning all layers except the used ones of the editor
		auto renderComponent = target->getComponent<G2::RenderComponent>();
		renderComponent->setRenderLayerMask(~EditorGeometryManager::gEditorGeometryLayers);
	}
}

void MeshPropertiesTab::_onGenerateCppCodeForManagedEntity(ManagedEntity const* entity, std::string const& entityVar, std::ostream& out)
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

	if (props.isMember(MESH_PATH))
	{
		// good practise to enclose the generated code in {}
		out << "		{" << std::endl;
		{
			out << indention << "// Mesh" << std::endl;
			out << indention <<
				"mFbxImporter.import(" <<
					"mProjectRoot + \"" << props[MESH_PATH].asString() << "\"," <<
					props.get(IMPORT_NORMALS, true).asString() << "," <<
					props.get(IMPORT_TEX_COORDS, true).asString() << "," <<
					"false" << "/* no animations so far! */, " <<
					props.get(FLIP_TEX_U, false).asString() << "," <<
					props.get(FLIP_TEX_V, false).asString() << "," <<
					"nullptr" << ", " <<
					"&" << entityVar <<
				");" << std::endl;
		}
		out << "		}" << std::endl;
	}
}