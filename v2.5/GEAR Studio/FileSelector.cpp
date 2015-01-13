#include "FileSelector.h"
#include "Project.h"
#include "GEARStudioEvents.h"

#include <QtWidgets/QFileDialog>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>

static const std::string PATH = "path";

FileSelector::FileSelector(std::string const& defaultPath, Json::Value const& source, std::string const& projectDirectory, std::string const& fileTypes, QWidget *parent)
	: QWidget(parent),
	mTarget(source),
	mDefaultPath(defaultPath),
	mProjectDirectory(projectDirectory)
{
	ui.setupUi(this);
	
	connect(ui.fileSelect, SIGNAL(clicked()), this, SLOT(selectFile()));
	G2S::onProjectOpened.hook(this, &FileSelector::_onProjectOpened);

	_initializeUiFromData();
}

FileSelector::~FileSelector()
{
	G2S::onProjectOpened.unHookAll(this);
}

void FileSelector::selectFile()
{
	std::string dialogDir = mProjectDirectory + mDefaultPath;
	QString effectPath = QFileDialog::getOpenFileName(this, "Select file", dialogDir.c_str(), mFileTypes.c_str());
	if (!effectPath.isNull())
	{
		// 
		std::string fullPath = effectPath.toStdString();

		if (!boost::algorithm::starts_with(fullPath, mProjectDirectory))
		{
			std::stringstream log;
			log << "Selected file '" << fullPath << "' is not contained in the project directory!";
			G2S::onLog(SEVERE, log.str());
		}
		else
		{
			// strip project directory
			boost::replace_first(fullPath, mProjectDirectory, "");
			mTarget[PATH] = fullPath;

			// update ui
			ui.filePath->setText(fullPath.c_str());

			// invoke event
			onFileSelected(this);
		}
	}
}

void FileSelector::setData(Json::Value const& source)
{
	mTarget = source;
	// initialize UI
	_initializeUiFromData();
}

void FileSelector::_initializeUiFromData()
{
	if (mTarget.isMember(PATH))
	{
		ui.filePath->setText(mTarget[PATH].asCString());
	}
	else
	{
		ui.filePath->setText("");

	}
}

void
FileSelector::_onProjectOpened(Project* project)
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