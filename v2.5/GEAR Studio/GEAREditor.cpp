#include "GEAREditor.h"
#include "ComponentListItemModel.h"

GEAREditor::GEAREditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// This function passes ownership of the QTextEdit to the MainWindow and tells the MainWindow that the QTextEdit will occupy the MainWindow's central area.
	// http://qt-project.org/doc/qt-4.8/mainwindows-dockwidgets.html
	setCentralWidget(ui.renderSurface);

	auto* model = new ComponentListItemModel();
	ui.componentListView->setModel(model);
}

GEAREditor::~GEAREditor()
{
}

void
GEAREditor::connectToGEARCore()
{
	// connect to GEAR Core ECS
	((ComponentListItemModel*)ui.componentListView->model())->startListening();
	// lost default scene
	((GLContext*)ui.renderSurface)->loadDefaultScene();
}