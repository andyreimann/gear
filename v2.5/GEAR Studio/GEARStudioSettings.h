#pragma once
#include "JsonSerializer.h"
#include "JsonDeserializer.h"

#include <string>

/** This class holds all information about the settings of the GEAR Studio.
* @created	2015/01/10
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class GEARStudioSettings : public JsonDeserializer, JsonSerializer
{
public:
	/** This will create a new GEARStudioSettings instance from a given settings file.
	* @param projectDirectory The project directory to load.
	*/
	GEARStudioSettings(std::string const& settingsFile);

	Json::Value& getSettings() { return mResource; }
	/** Saves the settings in it's current state into it's Json file.
	*/
	void save();
};
