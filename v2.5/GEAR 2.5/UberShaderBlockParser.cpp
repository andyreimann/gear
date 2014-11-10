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
				mBuilder->addGeometryShaderParts(blockParser.getGeometryShaderParts());
				mBuilder->addFragmentShaderParts(blockParser.getFragmentShaderParts());

				mBuilder->vertexShaderVersion = blockParser.getVertexShaderVersionString();
				mBuilder->geometryShaderVersion = blockParser.getGeometryShaderVersionString();
				mBuilder->fragmentShaderVersion = blockParser.getFragmentShaderVersionString();

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

					Pass::Builder& pass = mBuilder->passes[i];
					pass.settings = shaderBlockParser->getSettingsBlockParser().getSettings();
					pass.renderTargetSampler = Sampler::getSampler(Setting::get("RenderTarget", pass.settings, "DIFFUSE").value);
					pass.renderTargetType = RenderTargetType::getRenderTargetType(Setting::get("RenderType", pass.settings, "TEXTURE_2D").value);

					if(pass.renderTargetSampler == Sampler::SAMPLER_INVALID)
					{
						logger << "[UberShaderBlockParser] Could not determine RenderTarget!" << endl;
					}

					if(pass.renderTargetType == RenderTargetType::RT_INVALID)
					{
						logger << "[UberShaderBlockParser] Could not determine RenderType!" << endl;
					}

					// gather all parsed data from the block parsers
					pass.locationBindings = shaderBlockParser->getLocationBindingBlockParser().getLocationBindings();
					pass.properties = shaderBlockParser->getPropertiesBlockParser().getProperties();
					pass.addVertexShaderParts(shaderBlockParser->getVertexShaderParts());
					pass.addGeometryShaderParts(shaderBlockParser->getGeometryShaderParts());
					pass.addFragmentShaderParts(shaderBlockParser->getFragmentShaderParts());
					pass.vertexShaderVersion = shaderBlockParser->getVertexShaderVersionString();
					pass.geometryShaderVersion = shaderBlockParser->getGeometryShaderVersionString();
					pass.fragmentShaderVersion = shaderBlockParser->getFragmentShaderVersionString();
				}
				logger << "[UberShaderBlockParser] -> Shader block provides " << blockParser.getPassesBlockParser().getParsedPasses().size() << " passes\n";
			}
		}
	}
}