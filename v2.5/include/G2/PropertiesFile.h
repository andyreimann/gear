// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"
#include "Setting.h"

#include <string>
#include <fstream>

namespace G2 
{
	/** A Properties file is a simple file format having key value storages.
	 * @created:	2014/12/17
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class PropertiesFile 
	{
	
		public:
			/** This constructs a new PropertiesFile.
			 */
			PropertiesFile(std::string const& name);

			/** Returns a value of a setting coming from a Properties file.
			* @param name The name of the Setting to retrieve.
			* @param defaultValue The default value to return in case the setting is not present.
			*/
			G2::Setting const& getSetting(std::string const& name, std::string const& defaultValue = "");

		private:
			// private members

			std::unordered_map<std::string, Setting> mSettings;
	};
};