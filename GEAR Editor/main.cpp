#include <G2/GEAR.h>
#include "GearEditor.h"
#include <QtWidgets/QApplication>

// hide console
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	GEAREditor w;
	w.show();
	
	G2_startSideThread();
	
	int ret = a.exec();

	G2_stopSideThread();
	G2_shutdown();
	return ret;
}
