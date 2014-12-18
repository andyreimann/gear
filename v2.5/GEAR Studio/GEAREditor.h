#ifndef GEAREDITOR_H
#define GEAREDITOR_H

#include <QtWidgets/QMainWindow>
#include "ui_geareditor.h"
#include "Project.h"

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
		Ui::GEAREditorClass ui;
		std::unique_ptr<ProjectCreation> mNewProjectDialog;

		G2::PropertiesFile mStudioProperties;				// The properties of the entire GEAR Studio


		void _openProjectFromDirectory(std::string const& projectDirectory);

		std::shared_ptr<Project> mProject;

		private slots:
		void newProject();
		void openProject();


};

#endif // GEAREDITOR_H
