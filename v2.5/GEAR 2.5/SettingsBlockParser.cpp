#include "SettingsBlockParser.h"
#include "Logger.h"
#include "FileResource.h"

using namespace G2;

SettingsBlockParser::SettingsBlockParser(Effect::Builder* builder, FileResource* file) 
	: mBuilder(builder),
	mFile(file)
{
}

void
SettingsBlockParser::parse() 
{

	if(mFile == nullptr || mBuilder == nullptr)
	{
		
		logger << "[SettingsBlockParser] -> Error 1001: given filehandle or Effect::Builder is 0\n";
		return;
	}
	logger << "[SettingsBlockParser] -> start parsing Settings block\n";
	int curvedBracketsOpened = 0;

	if(mFile->isOpen()) 
	{
		while(!mFile->eof()) 
		{
			std::string line = mFile->getLine();
			
			curvedBracketsOpened += (int)std::count(line.begin(), line.end(), '{');
			curvedBracketsOpened -= (int)std::count(line.begin(), line.end(), '}');

			if(curvedBracketsOpened <= -1) 
			{
				// block content parsed
				logger << "[SettingsBlockParser] -> done parsing Settings block\n";
				return;
			}

			std::stringstream lineStr;
			lineStr << line;

			std::string settingName, settingValue, singleSettingValue;	
			lineStr >> settingName;

			do 
			{
				singleSettingValue = "";
				lineStr >> singleSettingValue;
				if(singleSettingValue == "")
				{
					break;
				}
				if(settingValue != "")
				{
					settingValue += " ";
				}
				settingValue += singleSettingValue;
			} while (true);

			

			if(settingName.substr(0,2) == "//")
			{
				continue;
			}

			// parse settingValue
			if(settingValue.substr(0,1) == "\"")
			{
				settingValue = settingValue.substr(1,settingValue.length()-2);
			}
			logger << "[SettingsBlockParser] -> Add Setting[name=" << settingName << ";value=" << settingValue << "]\n";
			mSettings[settingName] = Setting(settingName, settingValue);
		}
	}
	logger << "[SettingsBlockParser] -> done parsing Settings block\n";
}

G2::Setting
SettingsBlockParser::getSetting(std::string const& name) const
{
	return mSettings[name];
}