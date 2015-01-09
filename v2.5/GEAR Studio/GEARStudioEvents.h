#pragma once
#include "Project.h"
#include "LoggingTab.h"

#include <G2/Intersection.h>
#include <G2Core/Event.h>
#include <string>

/** This class holds global events, which can be triggered from every position.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class GEARStudioEvents
{
public:
	/** Trigger: 
	 * New project created by User.
	 * std::string const& The directory of the created Project
	 */
	static G2::Event<std::string const&>				onProjectCreated;
	/** Trigger:
	* New project is opened.
	* Project* The opened Project
	*/
	static G2::Event<Project*>							onProjectOpened;
	/** Trigger:
	* A Scene will be unloaded either due to the stop of the application or due to a new scene to load.
	* Scene* A pointer to the Scene which will be unloaded after the callback.
	*/
	static G2::Event<Scene*>							onSceneUnloaded;
	/** Trigger:
	* A new Scene was loaded.
	* Scene* A pointer to the Scene which was loaded before the callback.
	*/
	static G2::Event<Scene*>							onSceneLoaded;
	/** Trigger:
	* A new ManagedEntity was created within the currently loaded Scene.
	* Scene* A pointer to the Scene which is currently loaded.
	* ManagedEntity* A pointer to the ManagedEntity which was created.
	*/
	static G2::Event<Scene*, ManagedEntity*>			onManagedEntityCreated;
	/** Trigger:
	* A ManagedEntity was modified within the currently loaded Scene.
	* Scene* A pointer to the Scene which is currently loaded.
	* ManagedEntity* A pointer to the ManagedEntity which was modified.
	*/
	static G2::Event<Scene*, ManagedEntity*>			onManagedEntityModified;
	/** Trigger:
	* A ManagedEntity will be removed from the currently loaded Scene after the callback.
	* Scene* A pointer to the Scene which is currently loaded.
	* ManagedEntity* A pointer to the ManagedEntity which will be removed.
	*/
	static G2::Event<Scene*, ManagedEntity*>			onManagedEntityRemoved;
	/** Trigger:
	* A ManagedEntity was somehow selected.
	* ManagedEntity* A pointer to the ManagedEntity which was created.
	*/
	static G2::Event<ManagedEntity*>					onManagedEntitySelected;
	/** Trigger:
	* A new ManagedEntity instance is abount to be instantiated from a JSON description 
	* ManagedEntity* A pointer to the ManagedEntity which will be the instantiated one.
	* Json::Value const& a reference to the entities entire JSON document containing it's name, properties and so on.
	*/
	static G2::Event<ManagedEntity*, Json::Value const&> onDeserializeManagedEntity;
	/** Trigger:
	* A log message should be displayed in the editor.
	* LogLevel The log level to use.
	* std::string const& The message to log.
	*/
	static G2::Event<LogLevel, std::string const&> onLog;
	/** Trigger:
	* A click on an editor handle occurred.
	* unsigned int The entity id of the handle which was selected.
	* G2::Intersection const& The G2::Intersection at the selection time.
	*/
	static G2::Event<unsigned int, G2::Intersection const&> onEditorHandleSelected;
	/** Trigger:
	* The translation handle was moved.
	*/
	static G2::Event<> onTranslationHandleMoved;
	/** Trigger:
	* The translation handle was released.
	*/
	static G2::Event<> onTranslationHandleReleased;
	/** Trigger:
	* The generation of the C++ code for loading a ManagedEntity in the exported game
	* is about to be generated.
	* ManagedEntity* The ManagedEntity object pointer to export.
	* std::string const& The name of the G2::Entity& variable in the already generated code.
	* std::ofstream& The filestream to write the code to.
	* @note Make sure to generate as less code as possible to reduce compilation time!
	*/
	static G2::Event<ManagedEntity const*, std::string const&, std::ofstream&> onGenerateCppCodeForManagedEntity;
};
