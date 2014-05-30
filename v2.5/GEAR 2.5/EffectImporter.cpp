#include "EffectImporter.h"
#include "Logger.h"
#include "UberShaderParser.h"

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
	std::shared_ptr<Effect::Builder> builder = UberShaderParser::parse(fileName);

	return std::make_pair(fileName, builder);
}