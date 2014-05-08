#include "PassBlockParser.h"
#include "FileResource.h"
#include "ShaderBlockParser.h"
#include "Logger.h"
#include "ShaderBlockParser.h"

using namespace G2;

PassBlockParser::PassBlockParser(Effect::Builder* builder, FileResource* file) 
	: mBuilder(builder),
	mFile(file),
	mShaderBlockParser(new ShaderBlockParser(builder, file))
{
}

void
PassBlockParser::parse() 
{
	if(mFile == nullptr || mBuilder == nullptr)
	{
		logger << "[PassBlockParser] -> Error 1001: given filehandle or Effect::Builder is 0";
		return;
	}
	mBuilder->passes.push_back(Pass::Builder());
	logger << "[PassBlockParser] -> start parsing Pass block\n";
	mShaderBlockParser->parse(&mBuilder->passes.back().metaData);
	logger << "[PassBlockParser] -> done parsing Pass block\n";


}