// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "UberShaderBlockParser.h"
#include "LocationBindingsBlockParser.h"
#include "PropertiesBlockParser.h"
#include "ShaderBlockParser.h"
#include "Logger.h"
#include "FileResource.h"

#include <sstream>

using namespace G2;

UberShaderBlockParser::UberShaderBlockParser(Effect::Builder* builder, FileResource* file) 
	: mBuilder(builder),
	mFile(file)
{ }

void
UberShaderBlockParser::parse() 
{
	if(mFile == nullptr || mBuilder == nullptr)
	{
		logger << "[UberShaderBlockParser] -> Error 1001: given filehandle or Effect::Builder is 0\n";
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
				LocationBindingsBlockParser blockParser(mBuilder, mFile);
				blockParser.parse();
			}
			else if(blockName == "Properties")
			{
				PropertiesBlockParser blockParser(mBuilder, mFile);
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
				mBuilder->setShadingLanguage(shadingLanguage);
				ShaderBlockParser blockParser(mBuilder, mFile);
				blockParser.parse();

				// transport shader parts to UberShader
				mBuilder->addVertexShaderParts(blockParser.getVertexShaderParts());
				mBuilder->addFragmentShaderParts(blockParser.getFragmentShaderParts());

				std::vector<PassBlockParser> passes = blockParser.getPassesBlockParser().getParsedPasses();
				for(auto it = passes.begin(); it != passes.end(); ++it)
				{
					// Gather all informations for this pass
					// IMPORTANT: LocationBindings and Properties are shared within all passes!
					std::shared_ptr<ShaderBlockParser> passBlockParser = it->getShaderBlockParser();
					Setting passName = passBlockParser->getSetting("Name");
					Setting renderTarget = passBlockParser->getSetting("RenderTarget");
					//Pass pass(..., mUberShader->getProperties(), passBlockParser->getSettingsBlockParser().getSettings())
					// TODO create pass object
				}
				logger << "[UberShaderBlockParser] -> Shader block provides " << blockParser.getPassesBlockParser().getParsedPasses().size() << " passes\n";
			}
		}
	}
}