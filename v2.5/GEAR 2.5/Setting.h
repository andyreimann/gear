#pragma once
#include <string>
#include <sstream>
#include <unordered_map>
#include <algorithm>

#include <glm/glm.hpp>

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
		bool toBool() const 
		{ 
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
		glm::vec2 toVec2() const 
		{ 
			std::stringstream tmp;
			tmp << value;
			glm::vec2 v;
			tmp >> v.x >> v.y;
			return v;
		}
		glm::vec3 toVec3() const 
		{ 
			std::stringstream tmp;
			tmp << value;
			glm::vec3 v;
			tmp >> v.x >> v.y >> v.z;
			return v;
		}
		glm::vec4 toVec4() const 
		{ 
			std::stringstream tmp;
			tmp << value;
			glm::vec4 v;
			tmp >> v.x >> v.y >> v.z >> v.w;
			return v;
		}
		
		std::string key;
		std::string value;

		static Setting const& get(std::string const& name, std::unordered_map<std::string,Setting>& settings, std::string const& defaultValue = "");

	};
};