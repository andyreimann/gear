#pragma once
#include <string>

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
		
		std::string key;
		std::string value;
	};
};