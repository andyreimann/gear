#pragma once
#include "Effect.h"

#include <string>
#include <unordered_map>

namespace G2 
{
	class FileResource;

	struct Setting
	{
		Setting(std::string const& key = "", std::string const& value = "")
		 : key(key), value(value) {}
		
		int toInt() { return std::stoi(value); }
		long toLong() { return std::stol(value); }
		double toDouble() { return std::stod(value); }
		float toFloat() { return std::stof(value); }
		
		std::string key;
		std::string value;
	};

	/// This class takes care of loading the content of a generic Settings block
	/// @created:	2014/03/10
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class SettingsBlockParser 
	{
		public:
			/// This constructs a new SettingsBlockParser.
			SettingsBlockParser(Effect::Builder* builder, FileResource* file);

			void parse();
			/// This function will return the Setting for a given name.
			/// @param name The name of the Setting object to get the Settings for.
			/// @return The Setting object registered under the given name or a default settings object
			/// if no Setting object is registered.
			Setting getSetting(std::string const& name) const;
			/// This function will return all settings.
			/// @return All parsed settings
			std::unordered_map<std::string,Setting> const& getSettings() const { return mSettings; }
		private:
			Effect::Builder*								mBuilder;
			FileResource*									mFile;
			mutable std::unordered_map<std::string,Setting> mSettings;
	};
};