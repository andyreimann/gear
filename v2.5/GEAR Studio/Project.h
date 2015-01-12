#pragma once
#include "JsonSerializer.h"
#include "JsonDeserializer.h"
#include "Scene.h"

#include <memory>

class QProgressDialog;
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
		/** This function will trigger a full export of the project, which will generate all source files
		 * and prepare them for compilation.
		 */
		void exportProject(QProgressDialog* progress);
		
		void loadScene(std::string const& sceneFile);

		std::shared_ptr<Scene>& getCurrentScene() { return mCurrentScene; }
		/** Returns the projects directory.
		 * @return The projects directory.
		 */
		std::string const& getProjectDirectory() const { return mProjectDirectory; }

		~Project();
	private:
		std::string mProjectDirectory;

		std::shared_ptr<Scene> mCurrentScene;

};
