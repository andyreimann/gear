#pragma once
#include "Defines.h"
#include "Project.h"
#include "LoggingTab.h"

#include <G2/Intersection.h>
#include <G2Core/Event.h>
#include <string>

namespace G2S
{
	/** Trigger: 
	 * New project created by User.
	 * std::string const& The directory of the created Project
	 */
	extern G2::Event<std::string const&>				onProjectCreated;
	/** Trigger:
	* New project is opened.
	* Project* The opened Project
	*/
	extern G2::Event<Project*>							onProjectOpened;
	/** Trigger:
	* A Scene will be unloaded either due to the stop of the application or due to a new scene to load.
	* Scene* A pointer to the Scene which will be unloaded after the callback.
	*/
	extern G2::Event<Scene*>							onSceneUnloaded;
	/** Trigger:
	* A new Scene was loaded.
	* Scene* A pointer to the Scene which was loaded before the callback.
	*/
	extern G2::Event<Scene*>							onSceneLoaded;
	/** Trigger:
	* A new ManagedEntity was created within the currently loaded Scene.
	* Scene* A pointer to the Scene which is currently loaded.
	* ManagedEntity* A pointer to the ManagedEntity which was created.
	*/
	extern G2::Event<Scene*, ManagedEntity*>			onManagedEntityCreated;
	/** Trigger:
	* A ManagedEntity was modified within the currently loaded Scene.
	* Scene* A pointer to the Scene which is currently loaded.
	* ManagedEntity* A pointer to the ManagedEntity which was modified.
	*/
	extern G2::Event<Scene*, ManagedEntity*>			onManagedEntityModified;
	/** Trigger:
	* A ManagedEntity will be removed from the currently loaded Scene after the callback.
	* Scene* A pointer to the Scene which is currently loaded.
	* ManagedEntity* A pointer to the ManagedEntity which will be removed.
	*/
	extern G2::Event<Scene*, ManagedEntity*>			onManagedEntityRemoved;
	/** Trigger:
	* A ManagedEntity was somehow selected.
	* ManagedEntity* A pointer to the ManagedEntity which was created.
	*/
	extern G2::Event<ManagedEntity*>					onManagedEntitySelected;
	/** Trigger:
	* A new ManagedEntity instance is abount to be instantiated from a JSON description 
	* ManagedEntity* A pointer to the ManagedEntity which will be the instantiated one.
	* Json::Value const& a reference to the entities entire JSON document containing it's name, properties and so on.
	*/
	extern G2::Event<ManagedEntity*, Json::Value const&> onDeserializeManagedEntity;
	/** Trigger:
	* A log message should be displayed in the editor.
	* LogLevel The log level to use.
	* std::string const& The message to log.
	*/
	extern G2::Event<LogLevel, std::string const&> onLog;
	/** Trigger:
	* A click on an editor handle occurred.
	* unsigned int The entity id of the handle which was selected.
	* G2::Intersection const& The G2::Intersection at the selection time.
	*/
	extern G2::Event<unsigned int, G2::Intersection const&> onEditorHandleSelected;
	/** Trigger:
	* The translation handle was moved.
	*/
	extern G2::Event<> onTranslationHandleMoved;
	/** Trigger:
	* The translation handle was released.
	*/
	extern G2::Event<> onTranslationHandleReleased;
	/** Trigger:
	* The scale handle was moved/changed.
	*/
	extern G2::Event<> onScaleHandleMoved;
	/** Trigger:
	* The scale handle was released.
	*/
	extern G2::Event<> onScaleHandleReleased;
	/** Trigger:
	* The rotation handle was moved/changed.
	*/
	extern G2::Event<> onRotationHandleMoved;
	/** Trigger:
	* The rotation handle was released.
	*/
	extern G2::Event<> onRotationHandleReleased;
	/** Trigger:
	* The generation of the C++ code for loading a ManagedEntity in the exported game
	* is about to be generated.
	* ManagedEntity* The ManagedEntity object pointer to export.
	* std::string const& The name of the G2::Entity& variable in the already generated code.
	* std::ofstream& The filestream to write the code to.
	* @note Make sure to generate as less code as possible to reduce compilation time!
	*/
	extern G2::Event<ManagedEntity const*, std::string const&, std::ostream&> onGenerateCppCodeForManagedEntity;
	/** Trigger:
	* The active 3D handle should be changed (scale, rotation, translation, ...).
	*/
	extern G2::Event<G2S::HandleMode::Name> activateHandle;
};
