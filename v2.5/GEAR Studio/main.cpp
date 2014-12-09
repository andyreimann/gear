#include <G2/GEAR.h>
#include "GEAREditor.h"
#include <QtWidgets/QApplication>

// hide console
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main(int argc, char *argv[])
{

	G2_loadGfxDevice("GEARGL.dll");
	int ret = 0;
	{
		QApplication a(argc, argv);

		GEAREditor w;
		w.show();

		w.connectToGEARCore();

		ret = a.exec();
	}
	G2_shutdown();

	return ret;
}
