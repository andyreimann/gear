#include "PropertiesTab.h"
#include "GEARStudioEvents.h"

#include "G2/Logger.h"


PropertiesTab::PropertiesTab(std::string const& technicalName)
	: mTechnicalName(technicalName),
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

void
PropertiesTab::attachToSelectedEntity()
{
	if (mEntity != nullptr && !mEntity->hasProperties(mTechnicalName))
	{
		// create the property by just calling it's getter
		mEntity->getProperties(mTechnicalName);
		// reinitialize the tab
		_initWithEntity(mEntity);
	}
}

void PropertiesTab::_onManagedEntitySelected(ManagedEntity* entity)
{
	mEntity = entity;

	if (entity != nullptr)
	{
		G2::logger << "[PROP:"<<mTechnicalName<<"] Changed Entity-Context to " << entity->getName() << G2::endl;
	}

	// callback to initialize PropertiesTab for the entity
	_initWithEntity(mEntity);
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