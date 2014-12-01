#pragma once
#include <string>
#include <sstream>
#include <unordered_map>
#include <algorithm>

#include <glm/glm.hpp>
#include <list>

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
		bool toBool() const;
		glm::vec2 toVec2() const;
		glm::vec3 toVec3() const;
		glm::vec4 toVec4() const;

		std::list<std::string> toList(std::string const& delimiter) const;
		
		std::string key;
		std::string value;

		static Setting const& get(std::string const& name, std::unordered_map<std::string,Setting>& settings, std::string const& defaultValue = "");

	};
};