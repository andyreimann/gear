#include "Project.h"
#include "GEARStudioEvents.h"

#include <G2/StringTools.h>
#include <fstream>
#include <QProgressDialog>

Project::Project(std::string const& projectDirectory) :
	JsonDeserializer(projectDirectory + "/project.json"),
	JsonSerializer(projectDirectory + "/project.json"),
	mProjectDirectory(projectDirectory)
{
}

Project::~Project()
{
	// unload existing scene and replace with new scene
	if (mCurrentScene.get() != nullptr)
	{
		G2S::onSceneUnloaded(mCurrentScene.get());
	}
}

void
Project::loadLastScene(QProgressDialog* progress)
{
	if (!error())
	{

		std::string lastSceneName;
		if (mResource.isMember("last_opened_scene") && G2::Tools::String::trim(mResource["last_opened_scene"].asString()).size() > 0)
		{
			lastSceneName = mResource["last_opened_scene"].asString();
		}
		else if (mResource.isMember("scenes"))
		{
			Json::Value const& scenes = mResource["scenes"];
			if (scenes.isArray() && scenes.isValidIndex(0))
			{
				lastSceneName = scenes[0].asString();
			}
		}
		lastSceneName = G2::Tools::String::trim(lastSceneName);
		if (lastSceneName.size() > 0)
		{
			loadScene(mProjectDirectory + lastSceneName, progress);
		}
	}
}

void
Project::loadScene(std::string const& sceneFile, QProgressDialog* progress)
{

	std::shared_ptr<Scene> scene(new Scene(mProjectDirectory, sceneFile));
	if (scene->error())
	{

	}
	else
	{
		// unload existing scene and replace with new scene
		if (mCurrentScene.get() != nullptr)
		{
			G2S::onSceneUnloaded(mCurrentScene.get());
		}
		mCurrentScene = scene;
		
		mCurrentScene->load(progress);
		if (mCurrentScene.get() != nullptr)
		{
			G2S::onSceneLoaded(mCurrentScene.get());
		}
	}
}

void Project::exportProject(QProgressDialog* progress)
{
	progress->setLabelText("Generating C++ Code");
	G2S::onLog(INFO, "Generate and compile project");

	std::ofstream out(mProjectDirectory + "/generated-src/Game_generated.cpp");

	// set some global settings
	out.setf(std::ios::fixed, std::ios::floatfield);
	out.precision(6);

	out << "#include <Game.h>" << std::endl;
	out << "#include <glm/ext.hpp>" << std::endl;
	out << "using namespace G2;" << std::endl;
	out << "void Game::_auto_generated_loadCurrentScene()" << std::endl;
	out << "{" << std::endl;

	out << "	if(mCurrentScene == \"" << mCurrentScene->getName() << "\")" << std::endl;
	out << "	{" << std::endl;

	// export scene
	mCurrentScene->generateEntityInitializationCode(out, progress);

	out << "	}" << std::endl;

	out << "}" << std::endl;
	out.close();

	progress->setLabelText("Generating Visual Studio Solution");
	progress->setValue(progress->value());

	progress->setMaximum(progress->maximum() + 1);
	// call premake
	std::string premakeCmd = mProjectDirectory.substr(0, 2) + " && cd " + mProjectDirectory + " && " + "premake5.exe --file=premake.lua vs2013";
	system(premakeCmd.c_str());

	progress->setLabelText("Compiling Visual Studio Solution");
	progress->setValue(progress->value() + 1);

	progress->setMaximum(progress->maximum() + 10);
	std::string compileCmd = "\"" + mProjectDirectory.substr(0, 2) + " && cd " + mProjectDirectory + " && \"C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\Common7\\IDE\\devenv.exe\" \"GEAR Studio Sample.sln\" /Build \"Debug|x64\" /Out gear.out /Log gear.log\"";
	int status = system(compileCmd.c_str());
	progress->setValue(progress->value() + 10);

	G2S::onLog(INFO, "Compilation has exited with code " + std::to_string(status));
}