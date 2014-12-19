#include "GEARStudioEvents.h"

G2::Event<std::string const&>					GEARStudioEvents::onProjectCreated;
G2::Event<Project*>								GEARStudioEvents::onProjectOpened;
G2::Event<Scene*>								GEARStudioEvents::onSceneUnloaded;
G2::Event<Scene*>								GEARStudioEvents::onSceneLoaded;
G2::Event<Scene*, ManagedEntity*>				GEARStudioEvents::onManagedEntityCreated;
G2::Event<Scene*, ManagedEntity*>				GEARStudioEvents::onManagedEntityModified;
G2::Event<Scene*, ManagedEntity*>				GEARStudioEvents::onManagedEntityRemoved;
G2::Event<ManagedEntity*>						GEARStudioEvents::onManagedEntitySelected;
G2::Event<ManagedEntity*, Json::Value const&>	GEARStudioEvents::onDeserializeManagedEntity;

