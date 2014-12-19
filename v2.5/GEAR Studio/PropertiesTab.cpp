#include "PropertiesTab.h"
#include "GEARStudioEvents.h"


PropertiesTab::PropertiesTab(std::string const& technicalName)
	: mTechnicalName(technicalName),
	mEntity(nullptr)
{
	GEARStudioEvents::onManagedEntitySelected.hook(this, &PropertiesTab::_onManagedEntitySelected);
	GEARStudioEvents::onDeserializeManagedEntity.hook(this, &PropertiesTab::_onDeserializeManagedEntity);
}

PropertiesTab::~PropertiesTab()
{
	GEARStudioEvents::onManagedEntitySelected.unHookAll(this);
	GEARStudioEvents::onDeserializeManagedEntity.unHookAll(this);
}

void PropertiesTab::_onManagedEntitySelected(ManagedEntity* entity)
{
	mEntity = entity;
	// callback to initialize PropertiesTab for the entity
	_initWithEntity(mEntity);
}

void PropertiesTab::_onDeserializeManagedEntity(ManagedEntity* entity, Json::Value const& desc)
{
	_instantiateFromDescription(entity);
}