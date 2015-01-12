#pragma once
#include "ui_FileSelector.h"

#include <G2Core/Event.h>

#include <QtWidgets/QWidget>
#include <json/json.h>
#include <map>
#include <glm/glm.hpp>

class Project;
/** This class implements a file selector module.
* @created	2015/01/10
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class FileSelector : public QWidget
{
	Q_OBJECT
	public:
		/** Creates a new FileSelector with the given information.
		 * @param defaultPath The default path to open when file is about to be selected.
		 * @param source The source data to initialize the TextureSelector with.
		 * @param projectDirectory The directory of the currently opened project
		 * @param fileTypes The allowed filetypes as expected by the QFileDialog class
		 */
		FileSelector(std::string const& defaultPath, Json::Value const& source, std::string const& projectDirectory, std::string const& fileTypes, QWidget *parent = 0);
		~FileSelector();
		/** Returns the data of this FileSelector in it's most recent state.
		* @return The data of this FileSelector in it's most recent state.
		*/
		Json::Value const& getData() const { return mTarget; }
		/** Updates the data of this FileSelector.
		* @param source The new data to use for initialization.
		*/
		void setData(Json::Value const& source);

		G2::Event<FileSelector*>	onFileSelected;
		Ui::FileSelector			ui;					// The Qt UI class for the project creation dialog

	private slots:
		void selectFile();

	private:
		/** This is a callback function which will be invoked whenever a new Project was opened.
		 * @param project A pointer to the Project which was opened.
		 */
		void _onProjectOpened(Project* project);

		void _initializeUiFromData();

		Json::Value			mTarget;			// The json object to write the information to.
		std::string			mDefaultPath;		// The default color of this ColorSelector
		std::string			mProjectDirectory;	// The directory of the currently opened project
		std::string			mFileTypes;			// The allowed file types
};
