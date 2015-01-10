#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ProjectCreation.h"

#include <G2/PropertiesFile.h>
#include <json/json.h>

/** This is the main class of the GEAR Editor Studio.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class ProjectCreation : public QDialog, public Ui::ProjectCreation
{
	Q_OBJECT
	public:
		friend class GEAREditor;
		ProjectCreation(QWidget *parent = 0);


	private:
		Ui::ProjectCreation ui;					// The Qt UI class for the project creation dialog
		Json::Value* mStudioSettings;			// A pointer to the settings of GEAR Studio

	private slots:
		/** This trigger function is called whenever the user accepts the dialog.
		 */
		void createProject();
		/** This trigger function is called whenever the user clicks on the browse button.
		 * in the dialog to select a project folder.
		 */
		void selectProjectFolder();
	private:
		/** Generates a new premake lua file from the template contained in GEAR Studio.
		 * @param projectName The name of the Project, for which to generate the file.
		 * @param destinationFile The full path with name of the file to store the generated premake file to.
		 */
		void generatePremakeFile(std::string const& projectName, std::string const& destinationFile);
		/** Generates a new default project file.
		* @param projectName The name of the Project, for which to generate the file.
		* @param destinationFile The full path with name of the file to store the generated project file to.
		* @param defaultSceneFile The relative to project directory path of the default scene of the project.
		* @note Every project has at least one scene, which is a default scene if a new project is created.
		*/
		void generateProjectFile(std::string projectName, std::string const& destinationFile, std::string const& defaultSceneFile);
		/** Generates a new default scene file.
		* @param sceneName The name of the Scene, for which to generate the file.
		* @param destinationFile The full path with name of the file to store the generated Scene file to.
		* @note Every project has at least one scene, which is a default scene if a new project is created.
		*/
		void generateSceneFile(std::string sceneName, std::string const& destinationFile);
};
