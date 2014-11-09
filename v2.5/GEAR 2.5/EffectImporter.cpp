#include "EffectImporter.h"
#include "Logger.h"
#include "UberShaderParser.h"

#ifdef USE_NEW_EFFECT_IMPORTER
#include "FileResource.h"
#include "StringTools.h"
#endif

using namespace G2;

std::shared_ptr<Effect>
EffectImporter::importResource(std::string const& fileName) 
{
	
	auto it = mCache.find(fileName);
	if(it != mCache.end())
	{
		// cache hit
		return it->second->build();
	}
	// should never occur
	return std::shared_ptr<Effect>();
}

std::pair<std::string,std::shared_ptr<Effect::Builder>> 
EffectImporter::produceResourceBuilder(std::string const& fileName) 
{
	if(isCached(fileName))
	{
		return std::make_pair(fileName,std::shared_ptr<Effect::Builder>());
	}
	logger << "[EffectImporter] Import Effect file " << fileName << endl;

	// create builder and fill

#ifdef USE_NEW_EFFECT_IMPORTER


	FileResource file(fileName);
	if (!file.isOpen()) {

		logger << "[EffectImporter] could not open file '" << fileName << "'" << endl;
		return std::make_pair(fileName, std::shared_ptr<Effect::Builder>());
	}

	// file handle is open

	mCurrentBlockImporter = nullptr;
	mBlockImporterStack.empty();
	int curvedBracketsOpened = 0;

	while (!file.eof())
	{
		std::string line = file.getLine();
		std::stringstream lineStr;
		lineStr << line;


		std::string blockName;
		lineStr >> blockName;
		if (blockName.substr(0, 2) == "//")
		{
			continue;
		}

		// check if this can be a block name
		// special case 1: blockName ends with { -> strip it
		
		std::string suffix = "{";
		if (G2::Tools::String::endsWith(blockName, suffix))
		{
			blockName = blockName.substr(0, blockName.size() - suffix.size());
		}
		// special case 2: blockName contains a " or ' -> strip from that position
		size_t pos = blockName.find_first_of("\"");
		if (pos != std::string::npos)
		{
			blockName = blockName.substr(0, pos);
		}
		pos = blockName.find_first_of("'");
		if (pos != std::string::npos)
		{
			blockName = blockName.substr(0, pos);
		}


		if (mBlockImporterStack.empty())
		{
			mBlockImporterStack.push(&mRootImporter);
			mRootImporter.beginBlock();
		}
			// here we might push another block importer if one is registered

		// but first we try to detect the core block importer
		if (blockName == "LocationBindings")
		{

		}
		else if (blockName == "Properties")
		{

		}
		else if (blockName == "Shader")
		{

		}
		else if (blockName == "Passes")
		{

		}
		else if (blockName == "Pass")
		{

		}
		else if (blockName == "Settings")
		{

		}
		// let the line be consumed by the top block importer on the stack
		mBlockImporterStack.top()->processLine(line);
		
	}

	return std::make_pair(fileName, std::shared_ptr<Effect::Builder>());
#else

	std::shared_ptr<Effect::Builder> builder = UberShaderParser::parse(fileName);
	return std::make_pair(fileName, builder);
#endif
}