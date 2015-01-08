#include "LoggingTab.h"
#include "GEARStudioEvents.h"

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
	GEARStudioEvents::onLog.hook(this, &LoggingTab::_onLog);
}

LoggingTab::~LoggingTab()
{
	GEARStudioEvents::onLog.unHookAll(this);

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