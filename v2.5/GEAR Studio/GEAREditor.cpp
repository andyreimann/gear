#include "GEAREditor.h"
#include "ComponentListItemModel.h"
#include "ProjectCreation.h"

GEAREditor::GEAREditor(QWidget *parent)
	: QMainWindow(parent),
	mStudioProperties("settings.conf")
{
	ui.setupUi(this);

	// This function passes ownership of the QTextEdit to the MainWindow and tells the MainWindow that the QTextEdit will occupy the MainWindow's central area.
	// http://qt-project.org/doc/qt-4.8/mainwindows-dockwidgets.html
	setCentralWidget(ui.renderSurface);

	auto* model = new ComponentListItemModel();
	ui.componentListView->setModel(model);
	connect(ui.actionProject, SIGNAL(triggered()), this, SLOT(newProject()));


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

void
GEAREditor::newProject()
{
	std::cout << "Creating new Project" << std::endl;
	mNewProjectDialog = std::unique_ptr<ProjectCreation>(new ProjectCreation(this));
	mNewProjectDialog->mStudioProperties = &mStudioProperties;
	mNewProjectDialog->show();

}