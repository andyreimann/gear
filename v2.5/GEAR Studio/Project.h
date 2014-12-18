#pragma once
#include "JsonSerializer.h"
#include "JsonDeserializer.h"
#include "Scene.h"

#include <memory>


/** This class holds all information about a Project.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class Project : public JsonDeserializer, JsonSerializer
{
	public:
		/** This will create a new Project instance from a given project file.
		 * @param projectDirectory The project directory to load.
		 */
		Project(std::string const& projectDirectory);

		/** This will load all resources from the last scene, which was opened.
		 */
		void loadLastScene();
		
		void loadScene(std::string const& sceneFile);

		~Project();
	private:
		std::string mProjectDirectory;

		std::shared_ptr<Scene> mCurrentScene;

};
