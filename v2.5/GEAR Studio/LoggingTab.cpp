#include "LoggingTab.h"
#include "GEARStudioEvents.h"
#include <QtWidgets/QFileDialog>
#include <fstream>

const std::string currentDateTime() 
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%X", &tstruct);

	return buf;
}

LoggingTab::LoggingTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.clearLogsButton, SIGNAL(clicked()), this, SLOT(clearAll()));
	connect(ui.saveLogsButton, SIGNAL(clicked()), this, SLOT(saveLogs()));

	G2S::onLog.hook(this, &LoggingTab::_onLog);
	G2S::onProjectOpened.hook(this, &LoggingTab::_onProjectOpened);
}

LoggingTab::~LoggingTab()
{
	G2S::onLog.unHookAll(this);
	G2S::onProjectOpened.unHookAll(this);

}

void LoggingTab::_onLog(LogLevel, std::string const& msg)
{
	int row = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(row);

	ui.tableWidget->setItem(row, 0, new QTableWidgetItem("INFO"));
	ui.tableWidget->setItem(row, 1, new QTableWidgetItem(currentDateTime().c_str()));
	ui.tableWidget->setItem(row, 2, new QTableWidgetItem(msg.c_str()));
	ui.tableWidget->scrollToBottom();
}

void LoggingTab::clearAll()
{
	while (ui.tableWidget->rowCount() > 0)
	{
		ui.tableWidget->removeRow(0);
	}
}

void
LoggingTab::saveLogs()
{
	if (ui.tableWidget->rowCount() == 0)
	{
		return;
	}
	QString logFilePath = QFileDialog::getSaveFileName(this, "Select file to save to", mProjectDirectory.c_str(), "Log-Files (*.log *.txt)");

	std::ofstream out(logFilePath.toStdString());
	for (int i = 0; i < ui.tableWidget->rowCount(); ++i)
	{
		out << 
			ui.tableWidget->item(i, 1)->text().toStdString() << " - " <<
			ui.tableWidget->item(i, 0)->text().toStdString() << " - " <<
			ui.tableWidget->item(i, 2)->text().toStdString() << std::endl;
	}
	out.close();
}

void
LoggingTab::_onProjectOpened(Project* project)
{
	if (project != nullptr)
	{
		mProjectDirectory = project->getProjectDirectory();
	}
	else
	{
		mProjectDirectory = "";
	}
}