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
/** This class implements the ManagedEntity property regarding materials.
* @created	2015/01/06
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class LoggingTab : public QWidget
{
	Q_OBJECT
	public:
		LoggingTab(QWidget *parent = 0);
		~LoggingTab();
	private:

		void _onLog(LogLevel, std::string const& msg);
		
		Ui::LoggingTab	ui;

};
