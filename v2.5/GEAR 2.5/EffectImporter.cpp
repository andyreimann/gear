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

	logger << "[EffectImporter] Import Effect file " << fileName << endl;
	
	// Step 1: create builder and fill
	std::shared_ptr<Effect::Builder> builder = std::shared_ptr<Effect::Builder>(new Effect::Builder);

	builder->uberShader = UberShaderParser::parse(fileName);

	// (Step 2: cache builder)
	mCache[fileName] = builder;
	// Step 3: build and return resource
	return builder->build();
}