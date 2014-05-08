// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "UberShaderBlockParser.h"
#include "PropertiesBlockParser.h"
#include "ShaderBlockParser.h"
#include "Logger.h"
#include "FileResource.h"

#include <sstream>

using namespace G2;

UberShaderBlockParser::UberShaderBlockParser(Effect::Builder* builder, FileResource* file) 
	: mBuilder(builder),
	mFile(file),
	mLocationBindingBlockParser(file),
	mPropertiesBlockParser(file)
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
				mLocationBindingBlockParser.parse();
			}
			else if(blockName == "Properties")
			{
				mPropertiesBlockParser.parse(&mBuilder->metaData);
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
				blockParser.parse(&mBuilder->metaData);

				// transport shader parts to builder
				mBuilder->addVertexShaderParts(blockParser.getVertexShaderParts());
				mBuilder->addFragmentShaderParts(blockParser.getFragmentShaderParts());

				// transport location bindings to builder
				mBuilder->locationBindings = mLocationBindingBlockParser.getLocationBindings();
				// transport location bindings to builder
				mBuilder->properties = mPropertiesBlockParser.getProperties();

				std::vector<PassBlockParser> passes = blockParser.getPassesBlockParser().getParsedPasses();
				for (int i = 0; i < passes.size() ; ++i) 
				{
					PassBlockParser& passBlockParser = passes[i];
					// Gather all informations for this pass
					// IMPORTANT: LocationBindings and Properties are shared within all passes!
					std::shared_ptr<ShaderBlockParser> shaderBlockParser = passBlockParser.getShaderBlockParser();
					
					Setting passName = shaderBlockParser->getSetting("Name");
					Setting renderTarget = shaderBlockParser->getSetting("RenderTarget");

					Pass::Builder& pass = mBuilder->passes[i];
					pass.renderTargetSampler = Sampler::SAMPLER_INVALID;

					for(auto it = mBuilder->metaData.samplers.begin(); it != mBuilder->metaData.samplers.end(); ++it)
					{
						// resolve the render target to a sampler to bind the final result texture to.
						logger << "[UberShaderBlockParser] Check Property '"+it->name+"' against rendertarget name of '"+renderTarget.value+"'" << endl;
						if(it->name == renderTarget.value)
						{
							pass.renderTargetSampler = it->samplerSlot;
						}
					}

					if(pass.renderTargetSampler == Sampler::SAMPLER_INVALID)
					{
						logger << "[UberShaderBlockParser] Did not find Property with name '"+renderTarget.value+"' in Properties of main shader!" << endl;
					}

					// gather all parsed data from the block parsers
					pass.settings = shaderBlockParser->getSettingsBlockParser().getSettings();
					pass.locationBindings = shaderBlockParser->getLocationBindingBlockParser().getLocationBindings();
					pass.properties = shaderBlockParser->getPropertiesBlockParser().getProperties();
					pass.addVertexShaderParts(shaderBlockParser->getVertexShaderParts());
					pass.addFragmentShaderParts(shaderBlockParser->getFragmentShaderParts());
				}
				logger << "[UberShaderBlockParser] -> Shader block provides " << blockParser.getPassesBlockParser().getParsedPasses().size() << " passes\n";
			}
		}
	}
}