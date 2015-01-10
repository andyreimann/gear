#include "GEAREditor.h"
#include "ComponentListItemModel.h"
#include "ProjectCreation.h"
#include "GEARStudioEvents.h"


#include "MeshPropertiesTab.h"
#include "TransformationPropertiesTab.h"
#include "MaterialPropertiesTab.h"
#include "LightPropertiesTab.h"

#include <QtWidgets/QFileDialog>
#include <sstream>

GEAREditor::GEAREditor(QWidget *parent)
	: QMainWindow(parent),
	mStudioSettings("settings.json")
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
	connect(ui.addPropertyComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(addPropertyByIndex(int)));
	connect(ui.playButton, SIGNAL(clicked()), this, SLOT(exportAndStartProject()));

	GEARStudioEvents::onProjectCreated.hook(this, &GEAREditor::_openProjectFromDirectory);
	GEARStudioEvents::onSceneLoaded.hook(this, &GEAREditor::_onSceneLoaded);
	GEARStudioEvents::onManagedEntitySelected.hook(this, &GEAREditor::_onManagedEntitySelected);

	ui.propertiesRoot->layout()->setAlignment(Qt::AlignTop);

	// setup all the tabs and hide them
	auto* transformationTab = new TransformationPropertiesTab(ui.propertiesRoot);
	mPropertyTabs.push_back(std::shared_ptr<PropertiesTab>(transformationTab));
	ui.propertiesRoot->layout()->addWidget(transformationTab);
	transformationTab->hide();

	auto* meshTab = new MeshPropertiesTab(ui.propertiesRoot);
	mPropertyTabs.push_back(std::shared_ptr<PropertiesTab>(meshTab));
	ui.propertiesRoot->layout()->addWidget(meshTab);
	meshTab->hide();

	auto* materialTab = new MaterialPropertiesTab(ui.propertiesRoot);
	mPropertyTabs.push_back(std::shared_ptr<PropertiesTab>(materialTab));
	ui.propertiesRoot->layout()->addWidget(materialTab);
	materialTab->hide();

	auto* lightTab = new LightPropertiesTab(ui.propertiesRoot);
	mPropertyTabs.push_back(std::shared_ptr<PropertiesTab>(lightTab));
	ui.propertiesRoot->layout()->addWidget(lightTab);
	lightTab->hide();


	mLoggingTab = std::unique_ptr<LoggingTab>(new LoggingTab(this));
	ui.loggingRoot->layout()->addWidget(mLoggingTab.get());


	auto& lastProject = mStudioSettings.getSettings().get("lastproject", "").asString();
	if (lastProject != "")
	{
		QAction* menuAction = ui.menuOpenRecent->addAction(lastProject.c_str());
		connect(menuAction, SIGNAL(triggered()), this, SLOT(openLastProject()));
	}
}

GEAREditor::~GEAREditor()
{
	GEARStudioEvents::onProjectCreated.unHookAll(this);
	GEARStudioEvents::onSceneLoaded.unHookAll(this);
	GEARStudioEvents::onManagedEntitySelected.unHookAll(this);
}

void GEAREditor::connectToGEARCore()
{
	// lost default scene
	((GLContext*)ui.renderSurface)->createEditorCamera();

	// init editor geometry
	mEditorGeometryManager = std::shared_ptr<EditorGeometryManager>(new EditorGeometryManager());
	((GLContext*)ui.renderSurface)->setEditorGeometryManager(mEditorGeometryManager);
}

void GEAREditor::newProject()
{
	std::cout << "Creating new Project" << std::endl;
	mNewProjectDialog = std::unique_ptr<ProjectCreation>(new ProjectCreation(this));

	mNewProjectDialog->mStudioSettings = &mStudioSettings.getSettings();
	mNewProjectDialog->show();
}

void GEAREditor::_openProjectFromDirectory(std::string const& projectDirectory)
{
	if (projectDirectory != "")
	{
		mStudioSettings.getSettings().get("lastproject", "").asString();
		mStudioSettings.save();
	}
	// load the Project
	mProject = std::shared_ptr<Project>(new Project(projectDirectory));
	// fire event that a new project is opened
	GEARStudioEvents::onProjectOpened(mProject.get());

	ui.createEntity->setEnabled(true);
	mProject->loadLastScene();
}

void GEAREditor::openProject()
{
	QString directory = QFileDialog::getExistingDirectory(this,
		tr("Select Project Directory"), QDir::currentPath());
	_openProjectFromDirectory(directory.toStdString());
}

void GEAREditor::openLastProject()
{
	auto& lastProject = mStudioSettings.getSettings().get("lastproject", "").asString();
	_openProjectFromDirectory(lastProject);
}

void GEAREditor::createManagedEntity()
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

void GEAREditor::exportAndStartProject()
{
	if (mProject.get() != nullptr)
	{
		mProject->exportProject();
	}
}

void GEAREditor::_onSceneLoaded(Scene* scene)
{
	ui.sceneNameLabel->setText(scene->getName().c_str());
}

void GEAREditor::_onManagedEntitySelected(ManagedEntity* entity)
{
	if (entity == nullptr)
	{
		ui.addPropertyComboBox->setEnabled(false);
	}
	else
	{
		ui.addPropertyComboBox->setEnabled(true);
	}
}

void GEAREditor::addPropertyByIndex(int index)
{
	std::cout << "Selected index " << index << std::endl;

	std::string item = ui.addPropertyComboBox->itemText(index).toStdString();

	for (auto it = mPropertyTabs.begin(); it != mPropertyTabs.end(); ++it)
	{
		if (item == (*it)->getTabName())
		{
			(*it)->attachToSelectedEntity();
		}
	}
}