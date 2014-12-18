#include "GEAREditor.h"
#include "ComponentListItemModel.h"
#include "ProjectCreation.h"
#include "GEARStudioEvents.h"
#include <QtWidgets/QFileDialog>
#include <sstream>

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
	connect(ui.actionOpen_2, SIGNAL(triggered()), this, SLOT(openProject()));
	connect(ui.createEntity, SIGNAL(clicked()), this, SLOT(createManagedEntity()));

	GEARStudioEvents::onProjectCreated.hook(this, &GEAREditor::_openProjectFromDirectory);
	GEARStudioEvents::onSceneLoaded.hook(this, &GEAREditor::_onSceneLoaded);

	ui.propertiesRoot->layout()->setAlignment(Qt::AlignTop);

	// setup all the tabs and hide them
	mMeshTab = new MeshPropertiesTab(ui.propertiesRoot);
	ui.propertiesRoot->layout()->addWidget(mMeshTab);
	mMeshTab->hide();
}

GEAREditor::~GEAREditor()
{
	GEARStudioEvents::onProjectCreated.unHookAll(this);
	GEARStudioEvents::onSceneLoaded.unHookAll(this);
}

void
GEAREditor::connectToGEARCore()
{
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

void
GEAREditor::_openProjectFromDirectory(std::string const& projectDirectory)
{
	// load the Project
	mProject = std::shared_ptr<Project>(new Project(projectDirectory));
	mProject->loadLastScene();
	ui.createEntity->setEnabled(true);
}

void
GEAREditor::openProject()
{

	QString directory = QFileDialog::getExistingDirectory(this,
		tr("Select Project Directory"), QDir::currentPath());
	_openProjectFromDirectory(directory.toStdString());
}

void
GEAREditor::createManagedEntity()
{
	if (mProject.get() != nullptr && mProject->getCurrentScene().get() != nullptr)
	{
		std::stringstream name;
		unsigned int i = 1;
		do
		{
			name.str("");
			name << "Entity " << i++;
		} while (mProject->getCurrentScene()->createNewEntity(name.str()) == nullptr);
	}
}

void
GEAREditor::_onSceneLoaded(Scene* scene)
{
	ui.sceneNameLabel->setText(scene->getName().c_str());
}