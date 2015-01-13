#include "PropertiesTab.h"
#include "GEARStudioEvents.h"


PropertiesTab::PropertiesTab(std::string const& technicalName, std::string const& tabName)
	: mTechnicalName(technicalName),
	mTabName(tabName),
	mEntity(nullptr),
	mProject(nullptr)
{
	G2S::onManagedEntitySelected.hook(this, &PropertiesTab::_onManagedEntitySelected);
	G2S::onDeserializeManagedEntity.hook(this, &PropertiesTab::_onDeserializeManagedEntity);
	G2S::onProjectOpened.hook(this, &PropertiesTab::_onProjectOpened);
}

PropertiesTab::~PropertiesTab()
{
	G2S::onManagedEntitySelected.unHookAll(this);
	G2S::onDeserializeManagedEntity.unHookAll(this);
	G2S::onProjectOpened.unHookAll(this);
}

bool PropertiesTab::attachToSelectedEntity()
{
	if (mEntity != nullptr && !mEntity->hasProperties(mTechnicalName))
	{
		// create the property by just calling it's getter
		mEntity->getProperties(mTechnicalName);
		// reinitialize the tab
		_initUiWithEntity(mEntity);
		return true;
	}
	return false;
}

void PropertiesTab::_onManagedEntitySelected(ManagedEntity* entity)
{
	mEntity = entity;

	// callback to initialize PropertiesTab for the entity
	_initUiWithEntity(mEntity);
}

void PropertiesTab::_onDeserializeManagedEntity(ManagedEntity* entity, Json::Value const& desc)
{
	_instantiateFromDescription(entity);
}

void PropertiesTab::_onProjectOpened(Project* project)
{
	mProject = project;
	mProjectDirectory = project->getProjectDirectory();
}