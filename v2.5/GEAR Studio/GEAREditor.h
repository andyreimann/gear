#ifndef GEAREDITOR_H
#define GEAREDITOR_H

#include <QtWidgets/QMainWindow>
#include "ui_geareditor.h"
#include "Project.h"
#include "EditorGeometryManager.h"
#include "PropertiesTab.h"
#include "LoggingTab.h"

#include <G2/PropertiesFile.h>

#include <memory>
#include <vector>

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

	private slots:
		void newProject();		// Menu callback for creating a new project
		void openProject();		// Menu callback for opening an existing project
		void createManagedEntity();
		void addPropertyByIndex(int index);
		void exportAndStartProject();
		void openLastProject();

	private:
		void _openProjectFromDirectory(std::string const& projectDirectory);
		void _onSceneLoaded(Scene* scene);
		void _onManagedEntitySelected(ManagedEntity* entity);
		

		Ui::GEAREditorClass ui;												// The Qt UI object for the main editor.
		std::unique_ptr<ProjectCreation>			mNewProjectDialog;		// A pointer to the dialog for creating a new Project.
		G2::PropertiesFile							mStudioProperties;		// The properties of the entire GEAR Studio read from the settings.conf file
		std::shared_ptr<Project>					mProject;				// The currently loaded Project

		std::shared_ptr<EditorGeometryManager>		mEditorGeometryManager;	// The manager class for all the different geometry belonging to the editor
		std::vector<std::shared_ptr<PropertiesTab>> mPropertyTabs;			// All the PropertyTab instances operating on ManagedEntity objects.

		std::unique_ptr<LoggingTab>					mLoggingTab;

};

#endif // GEAREDITOR_H
