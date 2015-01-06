#include "Project.h"
#include "GEARStudioEvents.h"

#include <G2/StringTools.h>

Project::Project(std::string const& projectDirectory) :
	JsonDeserializer(projectDirectory + "/project.json"),
	JsonSerializer(projectDirectory + "/project.json"),
	mProjectDirectory(projectDirectory)
{
}

void
Project::loadLastScene()
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
			loadScene(mProjectDirectory + lastSceneName);
		}
	}
}

void
Project::loadScene(std::string const& sceneFile)
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
			GEARStudioEvents::onSceneUnloaded(mCurrentScene.get());
		}
		mCurrentScene = scene;
		mCurrentScene->load();
		if (mCurrentScene.get() != nullptr)
		{
			GEARStudioEvents::onSceneLoaded(mCurrentScene.get());
		}
	}
}


Project::~Project()
{
	// unload existing scene and replace with new scene
	if (mCurrentScene.get() != nullptr)
	{
		GEARStudioEvents::onSceneUnloaded(mCurrentScene.get());
	}
}