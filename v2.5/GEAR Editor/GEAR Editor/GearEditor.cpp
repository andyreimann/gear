#include "GearEditor.h"

GEAREditor::GEAREditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// This function passes ownership of the QTextEdit to the MainWindow and tells the MainWindow that the QTextEdit will occupy the MainWindow's central area.
	// http://qt-project.org/doc/qt-4.8/mainwindows-dockwidgets.html
	setCentralWidget(ui.renderSurface);
}

GEAREditor::~GEAREditor()
{

}
