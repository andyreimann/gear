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
	private:
		/** Called whenever a new log message should be logged.
		 * @param lvl The log level to log with.
		 * @param msg The message to log
		 */
		void _onLog(LogLevel lvl, std::string const& msg);
		
		Ui::LoggingTab	ui;

};
