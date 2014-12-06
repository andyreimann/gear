#include "PassesBlockParser.h"
#include "FileResource.h"
#include "Logger.h"

using namespace G2;

PassesBlockParser::PassesBlockParser(Effect::Builder* builder, FileResource* file) 
	: mBuilder(builder),
	mFile(file)
{
}

void
PassesBlockParser::parse() 
{
	if(mFile == nullptr || mBuilder == nullptr)
	{
		
		logger << "[PassesBlockParser] -> Error 1001: given filehandle or Effect::Builder is 0";
		return;
	}

	if(mFile->isOpen()) 
	{
		while(!mFile->eof()) 
		{
			std::string line = mFile->getLine();

			std::stringstream lineStr;
			lineStr << line;

			std::string prefix;
			lineStr >> prefix;
			if(prefix.substr(0,2) == "//")
			{
				continue;
			}
			if(prefix == "Pass") 
			{
				mPassBlockParser.push_back(PassBlockParser(mBuilder,mFile));
				mPassBlockParser.back().parse();
			}
			else if(prefix == "}") 
			{
				break;
			}
		}
	}
}

std::vector<PassBlockParser> const&
PassesBlockParser::getParsedPasses() const
{
	return mPassBlockParser;
}