#pragma once
#include <G2/GEAR.h>
#include <string>

namespace G2 
{
	// Functions implemented via generated code inside GameRuntime.dll
	typedef bool (*LoadScene)(std::string const& sceneName);

	// Functions which might be available optional 
	// implemented by the user
	typedef bool (*PostLoadScene)(std::string const& sceneName);

	struct GameRuntime
	{
		GameRuntime(std::string const& gameRuntimeLib);
		~GameRuntime();

		// required functions
		LoadScene loadScene;

		// optional functions
		PostLoadScene postLoadScene;
	};
};
