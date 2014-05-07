// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "PropertiesBlockParser.h"
#include "Property.h"
#include "StringTools.h"
#include "Logger.h"
#include "FileResource.h"

#include <sstream>
#include <algorithm>


using namespace G2;

PropertiesBlockParser::PropertiesBlockParser(Effect::Builder* builder, FileResource* file) 
	: mBuilder(builder),
	mFile(file)
{
}

void
PropertiesBlockParser::parse() 
{

	if(mFile == nullptr || mBuilder == nullptr)
	{		
		logger << "[PropertiesBlockParser] -> Error 1001: given filehandle or Effect::Builder is 0\n";
		return;
	}

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
				return;
			}

			std::stringstream lineStr;
			lineStr << line;

			std::string name, niceName, datatype, defaultValue;
			lineStr >> name;
			
			// parse nice name 
			size_t startPos = line.find_first_of("\"");
			size_t endPos = line.find_last_of("\"");
			if(startPos != std::string::npos && endPos != std::string::npos) 
			{
				niceName = line.substr(startPos+1, endPos-startPos-1);
			}
			// parse datatype
			startPos = line.find_first_of(",");
			endPos = line.find_first_of(")");
			if(startPos != std::string::npos && endPos != std::string::npos) 
			{
				datatype = Tools::String::trim(line.substr(startPos+1, endPos-startPos-1));
			}
			// parse default value
			startPos = line.find_first_of("{");
			endPos = line.find_last_of("}");
			if(startPos != std::string::npos && endPos != std::string::npos) 
			{
				defaultValue = Tools::String::trim(line.substr(startPos+1, endPos-startPos-1));
			}
			
			logger << "[PropertiesBlockParser] -> Add Property[name=" << name << ";nname=" << niceName << ";dt=" << datatype << ";default=" << defaultValue << "]\n";

			checkAndCreateMetaData(name, niceName, datatype, defaultValue);
		}
	}
}

void
PropertiesBlockParser::checkAndCreateMetaData(std::string const& name, 
											  std::string const& niceName, 
											  std::string const& dataType, 
											  std::string const& defaultValue)
{
	std::string samplerPrefix = "sampler";
	if(!dataType.compare(0,samplerPrefix.size(), samplerPrefix)) 
	{
		// check and create meta data for sampler Property
		Sampler::Name sampler = Sampler::getSampler(defaultValue);
		if(sampler != Sampler::SAMPLER_INVALID)
		{
			G2::ShaderMetaData& metaData = mBuilder->metaData;
			metaData.samplers[sampler].name = name;
			metaData.samplers[sampler].samplerSlot = sampler;
			// omit default value for samplers, because they are not compliant!
			mBuilder->properties.push_back(Property(name, niceName, dataType, ""));
			return;
		}
		logger << "[PropertiesBlockParser] -> Warning: Sampler-Property '" << name << "' has no valid sampler value given! Valid values are: DIFFUSE,NORMAL,ALPHA,SHADOW,SPECULAR,SAMPLER_6-SAMPLER_16.";
		return;
	}
	// just add a new property
	mBuilder->properties.push_back(Property(name, niceName, dataType, defaultValue));
}