#include "GEARStudioEvents.h"

G2::Event<std::string const&>						G2S::onProjectCreated;
G2::Event<Project*>									G2S::onProjectOpened;
G2::Event<Scene*>									G2S::onSceneUnloaded;
G2::Event<Scene*>									G2S::onSceneLoaded;
G2::Event<Scene*, ManagedEntity*>					G2S::onManagedEntityCreated;
G2::Event<Scene*, ManagedEntity*>					G2S::onManagedEntityModified;
G2::Event<Scene*, ManagedEntity*>					G2S::onManagedEntityRemoved;
G2::Event<ManagedEntity*>							G2S::onManagedEntitySelected;
G2::Event<ManagedEntity*, Json::Value const&>		G2S::onDeserializeManagedEntity;
G2::Event<LogLevel, std::string const&>				G2S::onLog;
G2::Event<unsigned int, G2::Intersection const&>	G2S::onEditorHandleSelected;
G2::Event<>											G2S::onTranslationHandleMoved;
G2::Event<>											G2S::onTranslationHandleReleased;
G2::Event<>											G2S::onScaleHandleMoved;
G2::Event<>											G2S::onScaleHandleReleased;
G2::Event<>											G2S::onRotationHandleMoved;
G2::Event<>											G2S::onRotationHandleReleased;
G2::Event<ManagedEntity const*, std::string const&, std::ostream&> G2S::onGenerateCppCodeForManagedEntity;
G2::Event<G2S::HandleMode::Name>					G2S::activateHandle;

