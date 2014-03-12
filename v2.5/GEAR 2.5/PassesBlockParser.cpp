#include "PassesBlockParser.h"
#include "FileResource.h"
#include "UberShader.h"
#include "Logger.h"

using namespace G2;

PassesBlockParser::PassesBlockParser(UberShader* uberShader, FileResource* file) 
	: mUberShader(uberShader),
	mFile(file)
{
}

void
PassesBlockParser::parse() 
{
	if(mFile == nullptr || mUberShader == nullptr)
	{
		
		logger << "[PassesBlockParser] -> Error 1001: given filehandle or UberShader is 0";
		return;
	}
	logger << "[PassesBlockParser] -> start parsing Passes block\n";

	if(mFile->isOpen()) 
	{
		while(!mFile->eof()) 
		{
			std::string line = mFile->getLine();

			std::stringstream lineStr;
			lineStr << line;

			std::string prefix;
			lineStr >> prefix;
			if(prefix == "Pass") 
			{
				mPassBlockParser.push_back(PassBlockParser(mUberShader,mFile));
				mPassBlockParser.back().parse();
			}
			else if(prefix == "}") 
			{
				break;
			}
		}
	}
	logger << "[PassesBlockParser] -> done parsing Passes block\n";
}

std::vector<PassBlockParser> const&
PassesBlockParser::getParsedPasses() const
{
	return mPassBlockParser;
}