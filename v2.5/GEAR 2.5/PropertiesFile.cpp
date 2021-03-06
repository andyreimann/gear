// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "PropertiesFile.h"
#include "Logger.h"
#include "FileResource.h"
#include "StringTools.h"
#include <fstream>

using namespace G2;

PropertiesFile::PropertiesFile(std::string const& name)
{
	FileResource file(name);

	static const std::string delimiter = "=";
	if (file.isOpen())
	{
		while (!file.eof())
		{

			auto line = file.getLine();
			size_t pos = line.find(delimiter);

			if (pos != std::string::npos && pos > 0)
			{
				auto key = G2::Tools::String::trim(line.substr(0, pos));
				auto start = pos + delimiter.length();
				auto value = G2::Tools::String::trim(line.substr(start, line.length() - start));
				mSettings[key] = Setting(key, value);
			}
		}
	}
}

G2::Setting const&
G2::PropertiesFile::getSetting(std::string const& name, std::string const& defaultValue /*= ""*/)
{
	return Setting::get(name, mSettings, defaultValue);
}

void G2::PropertiesFile::setSetting(std::string const& name, std::string const& value)
{
	mSettings[name] = Setting(name, value);
}

void G2::PropertiesFile::serialize(std::string const& file) const
{
	std::ofstream out(file);

	for (auto it = mSettings.begin(); it != mSettings.end(); ++it)
	{
		out << it->first << "=" << it->second.value << std::endl;
	}

	out.close();
}
