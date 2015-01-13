#include "GEAREditor.h"
#include "ComponentListItemModel.h"
#include "ProjectCreation.h"
#include "GEARStudioEvents.h"


#include "MeshPropertiesTab.h"
#include "TransformationPropertiesTab.h"
#include "MaterialPropertiesTab.h"
#include "LightPropertiesTab.h"

#include <QtWidgets/QFileDialog>
#include <boost/filesystem.hpp>
#include <sstream>

static const std::string PROJECT_HISTORY = "hist";
static const std::string PROJECT_HISTORY_NAME = "n";

GEAREditor::GEAREditor(QWidget *parent)
	: QMainWindow(parent),
	mStudioSettings("settings.json"),
	mEntity(nullptr)
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

	G2S::onProjectCreated.hook(this, &GEAREditor::_openProjectFromDirectory);
	G2S::onSceneLoaded.hook(this, &GEAREditor::_onSceneLoaded);
	G2S::onManagedEntitySelected.hook(this, &GEAREditor::_onManagedEntitySelected);

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

	mEditorPanel = std::unique_ptr<EditorPanel>(new EditorPanel(this, this));
	ui.editorPanelRoot->layout()->addWidget(mEditorPanel.get());


	Json::Value const& projectHistory = mStudioSettings.getSettings()[PROJECT_HISTORY];

	for (int i = projectHistory.size()-1; i >= 0; --i)  // Iterates over the sequence elements.
	{
		std::string path = projectHistory[i][PROJECT_HISTORY_NAME].asString();
		if (boost::filesystem::exists(path))
		{
			QAction* menuAction = ui.menuOpenRecent->addAction(path.c_str());
			connect(menuAction, SIGNAL(triggered()), this, SLOT(openRecentProject()));
		}
	}
}

GEAREditor::~GEAREditor()
{
	G2S::onProjectCreated.unHookAll(this);
	G2S::onSceneLoaded.unHookAll(this);
	G2S::onManagedEntitySelected.unHookAll(this);
}

void GEAREditor::connectToGEARCore()
{
	// lost default scene
	((GLContext*)ui.renderSurface)->createEditorCamera();

	// init editor geometry
	mEditorGeometryManager = std::shared_ptr<EditorGeometryManager>(new EditorGeometryManager());
	((GLContext*)ui.renderSurface)->setEditorGeometryManager(mEditorGeometryManager);
	// translation handle is activated by default
	G2S::activateHandle(G2S::HandleMode::NO_HANDLE);
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

		Json::Value& projectHistory = mStudioSettings.getSettings()[PROJECT_HISTORY];
		bool found = false;
		for (unsigned int i = 0; i < projectHistory.size(); ++i)  // Iterates over the sequence elements.
		{
			if (projectHistory[i][PROJECT_HISTORY_NAME].asString() == projectDirectory)
			{
				found = true;
				// TODO update TS
			}
		}
		if (!found)
		{
			// create a new history entry
			Json::Value historyEntry;
			historyEntry[PROJECT_HISTORY_NAME] = projectDirectory;
			Json::Value const& projectHistory = mStudioSettings.getSettings()[PROJECT_HISTORY].append(historyEntry);
			mStudioSettings.save();
		}
	}
	else
	{
		return;
	}
	// load the Project
	mProject = std::shared_ptr<Project>(new Project(projectDirectory));
	// fire event that a new project is opened
	G2S::onProjectOpened(mProject.get());

	ui.createEntity->setEnabled(true);


	QProgressDialog progress("Importing Scene...", "Cancel", 0, 10, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.show();
	progress.raise();
	progress.activateWindow();

	QApplication::processEvents();

	mProject->loadLastScene(&progress);
}

void GEAREditor::openProject()
{
	QString directory = QFileDialog::getExistingDirectory(this,
		tr("Select Project Directory"), QDir::currentPath());
	_openProjectFromDirectory(directory.toStdString());
}

void GEAREditor::openRecentProject()
{
	_openProjectFromDirectory(((QAction*)sender())->text().toStdString());
}

ManagedEntity* GEAREditor::createManagedEntity()
{
	ManagedEntity* instance = nullptr;
	if (mProject.get() != nullptr && mProject->getCurrentScene().get() != nullptr)
	{
		std::stringstream name;
		unsigned int i = 1;
		do
		{
			name.str("");
			name << "Entity " << i++;
			instance = mProject->getCurrentScene()->createNewEntity(name.str());
		} while (instance == nullptr);
	}
	return instance;
}

void GEAREditor::exportAndStartProject()
{
	QProgressDialog progress("Task in progress...", "Cancel", 0, 10, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.show();
	progress.raise();
	progress.activateWindow();

	QApplication::processEvents();
	//progress.setMinimum(0);
	//progress.setMaximum(0);
	//progress.setValue(0);
	if (mProject.get() != nullptr)
	{
		mProject->exportProject(&progress);
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
	mEntity = entity;
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