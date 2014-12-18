#ifndef GEAREDITOR_H
#define GEAREDITOR_H

#include <QtWidgets/QMainWindow>
#include "ui_geareditor.h"
#include "Project.h"

#include "MeshPropertiesTab.h"

#include <G2/PropertiesFile.h>

#include <memory>

class ProjectCreation;

/** This is the main class of the GEAR Editor Studio.
* @created	2014/12/09
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class GEAREditor : public QMainWindow
{
	Q_OBJECT

	public:
		GEAREditor(QWidget *parent = 0);
		/** Connects to the GEAR Core engine.
		 * This will start loading the last loaded project.
		 */
		void connectToGEARCore();

		~GEAREditor();

	private:
		void _openProjectFromDirectory(std::string const& projectDirectory);
		void _onSceneLoaded(Scene* scene);


		Ui::GEAREditorClass ui;								// The Qt UI object for the main editor.
		std::unique_ptr<ProjectCreation> mNewProjectDialog;	// A pointer to the dialog for creating a new Project.
		G2::PropertiesFile mStudioProperties;				// The properties of the entire GEAR Studio read from the settings.conf file
		std::shared_ptr<Project> mProject;					// The currently loaded Project

		MeshPropertiesTab* mMeshTab;

	private slots:
		void newProject();		// Menu callback for creating a new project
		void openProject();		// Menu callback for opening an existing project
		void createManagedEntity();

};

#endif // GEAREDITOR_H
