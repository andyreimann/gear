#include "Setting.h"

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