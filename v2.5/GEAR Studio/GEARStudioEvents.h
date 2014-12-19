#pragma once
#include "Scene.h"

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
};
