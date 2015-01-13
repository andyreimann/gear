#include "GEARStudioEvents.h"

G2::Event<std::string const&>						GEARStudioEvents::onProjectCreated;
G2::Event<Project*>									GEARStudioEvents::onProjectOpened;
G2::Event<Scene*>									GEARStudioEvents::onSceneUnloaded;
G2::Event<Scene*>									GEARStudioEvents::onSceneLoaded;
G2::Event<Scene*, ManagedEntity*>					GEARStudioEvents::onManagedEntityCreated;
G2::Event<Scene*, ManagedEntity*>					GEARStudioEvents::onManagedEntityModified;
G2::Event<Scene*, ManagedEntity*>					GEARStudioEvents::onManagedEntityRemoved;
G2::Event<ManagedEntity*>							GEARStudioEvents::onManagedEntitySelected;
G2::Event<ManagedEntity*, Json::Value const&>		GEARStudioEvents::onDeserializeManagedEntity;
G2::Event<LogLevel, std::string const&>				GEARStudioEvents::onLog;
G2::Event<unsigned int, G2::Intersection const&>	GEARStudioEvents::onEditorHandleSelected;
G2::Event<>											GEARStudioEvents::onTranslationHandleMoved;
G2::Event<>											GEARStudioEvents::onTranslationHandleReleased;
G2::Event<>											GEARStudioEvents::onScaleHandleMoved;
G2::Event<>											GEARStudioEvents::onScaleHandleReleased;
G2::Event<>											GEARStudioEvents::onRotationHandleMoved;
G2::Event<>											GEARStudioEvents::onRotationHandleReleased;
G2::Event<ManagedEntity const*, std::string const&, std::ostream&> GEARStudioEvents::onGenerateCppCodeForManagedEntity;
G2::Event<G2S::HandleMode::Name>					GEARStudioEvents::activateHandle;

