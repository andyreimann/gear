#include "ProjectCreation.h"
#include "GEARStudioEvents.h"

#include <json/json.h>

#include <QtWidgets/QFileDialog>

#include <G2/FileResource.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>


ProjectCreation::ProjectCreation(QWidget *parent /*= 0*/)
	: QDialog(parent),
	mStudioSettings(nullptr)
{
	ui.setupUi(this);

	connect(ui.buttons, SIGNAL(accepted()), this, SLOT(createProject()));
	connect(ui.buttons, SIGNAL(rejected()), this, SLOT(hide()));

	connect(ui.browse, SIGNAL(clicked()), SLOT(selectProjectFolder()));

}

void
ProjectCreation::createProject()
{
	// Checks:
	// -> Project Name regex
	QRegularExpression projectNameRegex("([A-z0-9._\\-&])+(\\s([A-z0-9._\\-&]+))*");
	QRegularExpressionMatch match = projectNameRegex.match(ui.projectName->text());
	if (!match.hasMatch()) 
	{
		// invalid project name
		return;
	}
	// -> folder exists?
	if (!QDir(ui.projectFolder->text()).exists())
	{
		// invalid folder 
		return;
	}
	// -> Namespace regex
	QRegularExpression namespaceRegex("[A-z]{2,10}");
	match = namespaceRegex.match(ui.projectNamespace->text());
	if (!match.hasMatch())
	{
		// invalid namespace
		return;
	}
	try
	{
		std::string templatesDir = (*mStudioSettings)["templatesdir"].asString();
		// start project creation
		std::string folder = ui.projectFolder->text().toStdString();
		// create all folders we might need
		boost::filesystem::create_directory(folder + "/bin");
		boost::filesystem::create_directory(folder + "/src");
		boost::filesystem::create_directory(folder + "/generated-src");
		boost::filesystem::create_directory(folder + "/scenes");
		boost::filesystem::create_directory(folder + "/assets");
		boost::filesystem::create_directory(folder + "/assets/textures");
		boost::filesystem::create_directory(folder + "/assets/meshes");
		boost::filesystem::create_directory(folder + "/assets/shader");

		boost::filesystem::copy_file(templatesDir + "Defines.h", folder + "/src/Defines.h", boost::filesystem::copy_option::overwrite_if_exists);
		boost::filesystem::copy_file(templatesDir + "Game.h", folder + "/src/Game.h", boost::filesystem::copy_option::overwrite_if_exists);
		boost::filesystem::copy_file(templatesDir + "Game.cpp", folder + "/src/Game.cpp", boost::filesystem::copy_option::overwrite_if_exists);
		boost::filesystem::copy_file(templatesDir + "resources.conf", folder + "/resources.conf", boost::filesystem::copy_option::overwrite_if_exists);
		boost::filesystem::copy_file(templatesDir + "Default.g2fx", folder + "/assets/shader/Default.g2fx", boost::filesystem::copy_option::overwrite_if_exists);

		// makefile has some placeholder, which need to be replaced
		generatePremakeFile(ui.projectName->text().toStdString(), folder + "/premake.lua");
		// generate a main scene
		generateSceneFile(ui.projectName->text().toStdString(), folder + "/scenes/main.json");
		// generate a project file
		generateProjectFile(ui.projectName->text().toStdString(), folder + "/project.json", "/scenes/main.json");

		GEARStudioEvents::onProjectCreated(folder);
	}
	catch (const boost::filesystem::filesystem_error& ex)
	{
		std::cout << ex.what() << std::endl;
	}
	hide();
}

void
ProjectCreation::selectProjectFolder()
{
	QString directory = QFileDialog::getExistingDirectory(this,
		tr("Select Project Directory"), QDir::currentPath());

	ui.projectFolder->setText(directory);

	if (ui.projectName->text().isEmpty())
	{
		//TODO extract project name as convenience
		int idx = directory.lastIndexOf("/")+1;
		QStringRef projectName(&directory, idx, directory.size()-idx);
		ui.projectName->setText(projectName.toString());
	}
}

void
ProjectCreation::generatePremakeFile(std::string const& projectName, std::string const& destinationFile)
{
	std::string templatesDir = (*mStudioSettings)["templatesdir"].asString();
	G2::FileResource premakeSrc(templatesDir + "premake.lua");

	std::string gearIncDir = (*mStudioSettings)["gearincdir"].asString();
	boost::replace_all(gearIncDir, "\\", "\\\\");
	std::string gearLibDir = (*mStudioSettings)["gearlibdir"].asString();
	boost::replace_all(gearLibDir, "\\", "\\\\");

	std::string premakeDst;

	if (premakeSrc.isOpen())
	{
		while (!premakeSrc.eof())
		{
			auto line = premakeSrc.getLine();

			boost::replace_all(line, "##PROJECTNAME##", projectName);
			boost::replace_all(line, "##GEARINCDIR##", gearIncDir);
			boost::replace_all(line, "##GEARLIBDIR##", gearLibDir);
			premakeDst += line;
			premakeDst += "\n";
		}
		std::ofstream out(destinationFile);
		out << premakeDst;
		out.close();
	}

}

void
ProjectCreation::generateSceneFile(std::string sceneName, std::string const& destinationFile)
{
	Json::Value root;
	root["gear_studio_ver"] = 1.0;
	root["name"] = sceneName;
	root["rendersystem"]["size_hint"] = 100;
	root["rendersystem"]["default_effect"] = "/assets/shader/Default.g2fx";
	root["rendersystem"]["default_color"]["x"] = 0.8f;
	root["rendersystem"]["default_color"]["y"] = 0.8f;
	root["rendersystem"]["default_color"]["z"] = 0.8f;
	root["rendersystem"]["default_color"]["a"] = 1.f;
	root["entities"] = Json::Value(Json::arrayValue);

	Json::Value sampleEntity;
	sampleEntity["name"] = "Sample Entity";

	Json::Value renderComponent;
	renderComponent["mesh_path"] = "/assets/meshes/unit-sphere.fbx";
	sampleEntity["properties"]["mesh"] = renderComponent;

	Json::Value lightComponent;
	lightComponent["type"] = "DIRECTIONAL";
	lightComponent["diffuse"]["x"] = 1.f;
	lightComponent["diffuse"]["y"] = 1.f;
	lightComponent["diffuse"]["z"] = 1.f;
	lightComponent["diffuse"]["a"] = 1.f;
	lightComponent["attenuation"]["constant"] = 1.f;
	lightComponent["attenuation"]["linear"] = 1.f;
	lightComponent["attenuation"]["exponential"] = 0.f;

	sampleEntity["properties"]["light"] = lightComponent;

	Json::Value transformComponent;
	transformComponent["type"] = "SRT";
	transformComponent["rotation"]["angle"] = -10.f;
	transformComponent["rotation"]["axis"]["x"] = 1.f;
	transformComponent["rotation"]["axis"]["y"] = 0.f;
	transformComponent["rotation"]["axis"]["z"] = 0.f;
	sampleEntity["properties"]["transf"] = transformComponent;

	root["entities"].append(sampleEntity);

	std::ofstream out(destinationFile);
	out << root;
	out.close();
}

void
ProjectCreation::generateProjectFile(std::string projectName, std::string const& destinationFile, std::string const& defaultSceneFile)
{
	Json::Value root;
	root["gear_studio_ver"] = 1.0;
	root["name"] = projectName;
	root["last_opened_scene"] = "";
	root["start_scene"] = defaultSceneFile;
	root["scenes"] = Json::Value(Json::arrayValue);
	root["scenes"].append(defaultSceneFile);

	std::ofstream out(destinationFile);
	out << root;
	out.close();
}