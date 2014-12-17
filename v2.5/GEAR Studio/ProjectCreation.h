#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ProjectCreation.h"

#include <G2/PropertiesFile.h>

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
		Ui::ProjectCreation ui;
		G2::PropertiesFile* mStudioProperties;

	private slots:
		void createProject();
		void selectProjectFolder();
		void generatePremakeFile(std::string const& projectName, std::string const& destinationFile);
		void generateProjectFile(std::string projectName, std::string const& destinationFile, std::string const& defaultSceneFile);
		void generateSceneFile(std::string sceneName, std::string const& destinationFile);
};
