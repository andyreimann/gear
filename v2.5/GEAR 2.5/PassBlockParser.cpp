#include "PassBlockParser.h"
#include "FileResource.h"
#include "UberShader.h"
#include "ShaderBlockParser.h"
#include "Logger.h"

using namespace G2;

PassBlockParser::PassBlockParser(UberShader* uberShader, FileResource* file) 
	: mUberShader(uberShader),
	mFile(file)
{
}

void
PassBlockParser::parse() 
{
	if(mFile == nullptr || mUberShader == nullptr)
	{
		logger << "[PassBlockParser] -> Error 1001: given filehandle or UberShader is 0";
		return;
	}
	logger << "[PassBlockParser] -> start parsing Pass block\n";
	ShaderBlockParser shaderBlockParser(mUberShader, mFile);
	shaderBlockParser.parse();
	logger << "[PassBlockParser] -> done parsing Pass block\n";


}