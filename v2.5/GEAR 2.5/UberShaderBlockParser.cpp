// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "UberShaderBlockParser.h"
#include "LocationBindingsBlockParser.h"
#include "PropertiesBlockParser.h"
#include "ShaderBlockParser.h"
#include "Logger.h"
#include "UberShader.h"
#include "FileResource.h"

#include <sstream>

using namespace G2;

UberShaderBlockParser::UberShaderBlockParser(UberShader* uberShader, FileResource* file) 
	: mUberShader(uberShader),
	mFile(file)
{ }

void
UberShaderBlockParser::parse() 
{
	if(mFile == nullptr || mUberShader == nullptr)
	{
		logger << "[UberShaderBlockParser] -> Error 1001: given filehandle or UberShader is 0\n";
		return;
	}
	
	int curvedBracketsOpened = 0;

	if(mFile->isOpen()) 
	{
		while(!mFile->eof()) 
		{
			std::string line = mFile->getLine();
			std::stringstream lineStr;
			lineStr << line;
			std::string blockName;
			lineStr >> blockName;

			if(blockName == "}") 
			{
				// reached end of block
				return;
			}

			logger << "[UberShaderBlockParser] -> parse '" << blockName << "' block\n";
			//qDebug(log.str().c_str());

			if(blockName == "LocationBindings") 
			{
				LocationBindingsBlockParser blockParser(mUberShader, mFile);
				blockParser.parse();
			}
			else if(blockName == "Properties")
			{
				PropertiesBlockParser blockParser(mUberShader, mFile);
				blockParser.parse();
			}
			else if(blockName == "Shader") 
			{
				std::string shadingLanguage;
				// parse shading language to use
				size_t startPos = line.find_first_of("\"");
				size_t endPos = line.find_last_of("\"");
				if(startPos != std::string::npos && endPos != std::string::npos) 
				{
					shadingLanguage = line.substr(startPos+1, endPos-startPos-1);
				}
				mUberShader->setShadingLanguage(shadingLanguage);
				ShaderBlockParser blockParser(mUberShader, mFile);
				blockParser.parse();

				// transport shader parts to UberShader
				auto& vertexShaderParts = blockParser.getVertexShaderParts();
				for(auto it = vertexShaderParts.begin(); it != vertexShaderParts.end(); ++it)
				{
					mUberShader->addVertexShaderPart(*it);
				}
				// transport shader parts to UberShader
				auto& fragmentShaderParts = blockParser.getFragmentShaderParts();
				for(auto it = fragmentShaderParts.begin(); it != fragmentShaderParts.end(); ++it)
				{
					mUberShader->addFragmentShaderPart(*it);
				}
				logger << "[UberShaderBlockParser] -> Shader block provides " << blockParser.getPassesBlockParser().getParsedPasses().size() << " passes\n";
			}
		}
	}
}