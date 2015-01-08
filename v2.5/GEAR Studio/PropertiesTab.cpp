#include "PropertiesTab.h"
#include "GEARStudioEvents.h"


PropertiesTab::PropertiesTab(std::string const& technicalName, std::string const& tabName)
	: mTechnicalName(technicalName),
	mTabName(tabName),
	mEntity(nullptr),
	mProject(nullptr)
{
	GEARStudioEvents::onManagedEntitySelected.hook(this, &PropertiesTab::_onManagedEntitySelected);
	GEARStudioEvents::onDeserializeManagedEntity.hook(this, &PropertiesTab::_onDeserializeManagedEntity);
	GEARStudioEvents::onProjectOpened.hook(this, &PropertiesTab::_onProjectOpened);
}

PropertiesTab::~PropertiesTab()
{
	GEARStudioEvents::onManagedEntitySelected.unHookAll(this);
	GEARStudioEvents::onDeserializeManagedEntity.unHookAll(this);
	GEARStudioEvents::onProjectOpened.unHookAll(this);
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

	if (entity != nullptr)
	{
		std::stringstream log;
		log << "[PROP:" << mTechnicalName << "] Changed Entity-Context to " << entity->getName() << "[" << entity->getId() << "]";
		GEARStudioEvents::onLog(INFO, log.str());
	}

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