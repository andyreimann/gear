#pragma once
#include "ui_LoggingTab.h"
#include "PropertiesTab.h"

#include <G2/TextureImporter.h>
#include <G2/Sampler.h>

#include <QtWidgets/QWidget>
#include <memory>
#include <vector>

enum LogLevel
{
	DEBUG,
	INFO,
	WARNING,
	SEVERE,
};
class Project;
/** This class implements the logging tab functionality of the editor.
* @created	2015/01/08
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class LoggingTab : public QWidget
{
	Q_OBJECT
	public:
		LoggingTab(QWidget *parent = 0);
		~LoggingTab();

		public slots:
		/** Clears all log entries.
		*/
		void clearAll();
		/** Opens a file save dialog to save the current logs.
		*/
		void saveLogs();
	private:
		/** This is a callback function which will be invoked whenever a new Project was opened.
		* @param project A pointer to the Project which was opened.
		*/
		void _onProjectOpened(Project* project);
		/** Called whenever a new log message should be logged.
		 * @param lvl The log level to log with.
		 * @param msg The message to log
		 */
		void _onLog(LogLevel lvl, std::string const& msg);
		
		Ui::LoggingTab	ui;
		std::string mProjectDirectory;		// The directory of the currently loaded project
};
