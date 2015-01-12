#ifndef GEAREDITOR_H
#define GEAREDITOR_H

#include <QtWidgets/QMainWindow>
#include "ui_geareditor.h"
#include "Project.h"
#include "EditorGeometryManager.h"
#include "PropertiesTab.h"
#include "LoggingTab.h"
#include "GEARStudioSettings.h"
#include "EditorPanel.h"

#include <G2/PropertiesFile.h>

#include <memory>
#include <vector>

class ProjectCreation;

/** This is the main class of the GEAR Studio.
* @created	2014/12/09
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class GEAREditor : public QMainWindow
{
	Q_OBJECT

	public:
		GEAREditor(QWidget *parent = 0);
		/** Connects to the GEAR Core engine.
		 * @warning You should only start using GEAR functionalities after this function was called!
		 */
		void connectToGEARCore();

		~GEAREditor();
	public slots:
		/** Starts a dialog for a project creation.
		 */
		void newProject();

		/** Starts a dialog for a project open operation, which can be canceled.
		*/
		void openProject();
		/** Initiates a blocking export of the currently loaded Project 
		 * and starts the project after successful compilation.
		 */
		void exportAndStartProject();

	private slots:
		/** Creates a new ManagedEntity instance in the currently loaded project.
		 * @return The created ManagedEntity instance or a nullptr, in case no new ManagedEntity could be created.
		*/
		ManagedEntity* createManagedEntity();
		void addPropertyByIndex(int index);
		/** DEBUG callback for opening the last loaded project.
		 * @note This function is only a dummy function and will be replaced 
		 * by a real history functionality as soon as the project history is saved in the GEAR Studio settings.
		 */
		G2_DEPRECATED void openLastProject();

	private:
		void _openProjectFromDirectory(std::string const& projectDirectory);
		void _onSceneLoaded(Scene* scene);
		void _onManagedEntitySelected(ManagedEntity* entity);
		

		Ui::GEAREditorClass ui;												// The Qt UI object for the main editor.
		std::unique_ptr<ProjectCreation>			mNewProjectDialog;		// A pointer to the dialog for creating a new Project.
		std::shared_ptr<Project>					mProject;				// The currently loaded Project

		std::shared_ptr<EditorGeometryManager>		mEditorGeometryManager;	// The manager class for all the different geometry belonging to the editor
		std::vector<std::shared_ptr<PropertiesTab>> mPropertyTabs;			// All the PropertyTab instances operating on ManagedEntity objects.

		std::unique_ptr<LoggingTab>					mLoggingTab;			// The instance of the logging tab
		std::unique_ptr<EditorPanel>				mEditorPanel;			// The instance of the editor panel
		GEARStudioSettings							mStudioSettings;		// The settings of the GEAR Studio

		ManagedEntity*								mEntity;				// The currently selected ManagedEntity instance.
};

#endif // GEAREDITOR_H
