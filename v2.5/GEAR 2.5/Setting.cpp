#include "Setting.h"
#include "StringTools.h"

using namespace G2;

Setting const&
Setting::get(std::string const& name, std::unordered_map<std::string,Setting>& settings, std::string const& defaultValue /*= ""*/) 
{
	auto it = settings.find(name);
	if(it != settings.end())
	{
		return it->second;
	}
	settings[name] = Setting(name, defaultValue);
	return settings[name];
}

glm::vec4
G2::Setting::toVec4() const
{
	std::stringstream tmp;
	tmp << value;
	glm::vec4 v;
	tmp >> v.x >> v.y >> v.z >> v.w;
	return v;
}

glm::vec3
G2::Setting::toVec3() const
{
	std::stringstream tmp;
	tmp << value;
	glm::vec3 v;
	tmp >> v.x >> v.y >> v.z;
	return v;
}

glm::vec2
G2::Setting::toVec2() const
{
	std::stringstream tmp;
	tmp << value;
	glm::vec2 v;
	tmp >> v.x >> v.y;
	return v;
}

bool
G2::Setting::toBool() const
{
	std::string tmp = value;
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
	if (tmp == "ON" ||
		tmp == "TRUE" ||
		tmp == "ENABLED" ||
		tmp == "1")
	{
		return true;
	}
	return false;
}

std::list<std::string>
G2::Setting::toList(std::string const& delimiter) const
{
	size_t start = value.find_first_of("{");
	size_t end = value.find_first_of("}");

	if (start == std::string::npos)
	{
		start = 0;
	}
	else
	{
		++start;
	}
	if (end == std::string::npos)
	{
		end = value.length();
	}
	else
	{
		--end;
	}
	std::list<std::string> list;
	// strip existing "{}" 
	std::string listStr = value.substr(start, end);
	size_t pos;
	while((pos = listStr.find(delimiter)) != std::string::npos) 
	{
		list.push_back(G2::Tools::String::trim(listStr.substr(0, pos)));
		listStr.erase(0, pos + delimiter.length());
	}
	std::string last = G2::Tools::String::trim(listStr);
	if (last.length() > 0)
	{
		list.push_back(last);
	}
	return list;
}