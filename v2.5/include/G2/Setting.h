#pragma once
#include <string>
#include <unordered_map>
#include <algorithm>

namespace G2 
{
	/// This class defines...
	/// @created:	2014/05/07
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	struct Setting
	{
		Setting(std::string const& key = "", std::string const& value = "")
		 : key(key), value(value) {}
		
		int toInt() const { return std::stoi(value); }
		long toLong() const { return std::stol(value); }
		double toDouble() const { return std::stod(value); }
		float toFloat() const { return std::stof(value); }
		bool toBool() const { 
			std::string tmp = value;
			std::transform(tmp.begin(), tmp.end(),tmp.begin(), ::toupper);
			if(tmp == "ON" ||
			   tmp == "TRUE" ||
			   tmp == "ENABLED" ||
			   tmp == "1")
			{
				return true;
			}
			return false;
		}
		
		std::string key;
		std::string value;

		static Setting const& get(std::string const& name, std::unordered_map<std::string,Setting>& settings, std::string const& defaultValue = "");

	};
};