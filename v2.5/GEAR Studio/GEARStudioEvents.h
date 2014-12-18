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
	static G2::Event<std::string const&>				onProjectCreated; // std::string=project directory
	static G2::Event<Scene*>							onSceneUnloaded; 
	static G2::Event<Scene*>							onSceneLoaded;
	static G2::Event<Scene*, ManagedEntity*>			onManagedEntityCreated;
	static G2::Event<Scene*, ManagedEntity*>			onManagedEntityModified;
	static G2::Event<Scene*, ManagedEntity*>			onManagedEntityRemoved;
};
