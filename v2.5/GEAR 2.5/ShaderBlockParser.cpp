// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "ShaderBlockParser.h"
#include "SimpleShaderPart.h"
#include "MatricesDefaultInclude.h"
#include "MaterialDefaultInclude.h"
#include "LightDefaultInclude.h"
#include "IfElseMacro.h"
#include "Logger.h"
#include "FileResource.h"
#include "ShaderMetaData.h"

#include <sstream>
#include <algorithm>


using namespace G2;

ShaderBlockParser::ShaderBlockParser(Effect::Builder* builder, FileResource* file) 
	: mBuilder(builder),
	mFile(file),
	mSettingsBlockParser(builder,file),
	mPassesBlockParser(builder,file),
	mLocationBindingBlockParser(file),
	mPropertiesBlockParser(file)
{
}

void
ShaderBlockParser::parse(ShaderMetaData* shaderMetaData) 
{
	mCurrentState = WAITING_FOR_SHADER_CODE;

	if(mFile == nullptr || mBuilder == nullptr)
	{
		
		logger << "[ShaderBlockParser] -> Error 1001: given filehandle or Effect::Builder is 0\n";
		return;
	}
	logger << "[ShaderBlockParser] -> start parsing Shader block\n";
	int curvedBracketsOpened = 0;

	if(mFile->isOpen()) 
	{
		while(!mFile->eof()) 
		{
			std::string line = mFile->getLine();
			
			curvedBracketsOpened += (int)std::count(line.begin(), line.end(), '{');
			curvedBracketsOpened -= (int)std::count(line.begin(), line.end(), '}');

			if(curvedBracketsOpened <= -1) 
			{
				// block content parsed
				flushShaderPart(std::shared_ptr<AbstractShaderPart>());
				logger << "[ShaderBlockParser] -> done parsing Shader block\n";
				return;
			}

			std::stringstream lineStr;
			lineStr << line; 

			switch(mCurrentState) 
			{
				case READING_SETTINGS_BLOCK:
				case READING_PROPERTIES_BLOCK:
				case READING_LOCATIONBINDINGS_BLOCK:
				case READING_PASSES_BLOCK:
				case WAITING_FOR_SHADER_CODE:
				{
					std::string macro;
					lineStr >> macro;
					if(macro == "Settings") 
					{
						mCurrentState = READING_SETTINGS_BLOCK;
						--curvedBracketsOpened;
						mSettingsBlockParser.parse();
					}
					else if(macro == "Passes") 
					{
						mCurrentState = READING_PASSES_BLOCK;
						--curvedBracketsOpened;
						mPassesBlockParser.parse();
					}
					else if(macro == "LocationBindings") 
					{
						// only valid for pass block
						mCurrentState = READING_LOCATIONBINDINGS_BLOCK;
						--curvedBracketsOpened;
						mLocationBindingBlockParser.parse();
					}
					else if(macro == "Properties") 
					{
						// only valid for pass block
						mCurrentState = READING_PROPERTIES_BLOCK;
						--curvedBracketsOpened;
						mPropertiesBlockParser.parse(shaderMetaData);
					}
					else if(macro == "#VERTEXPROGRAM") 
					{
						mCurrentState = READING_VERTEX_SHADER;
					}
					else if(macro == "#FRAGMENTPROGRAM") 
					{
						mCurrentState = READING_FRAGMENT_SHADER;
					}
					if( mCurrentState != READING_PASSES_BLOCK && 
						mCurrentState != READING_SETTINGS_BLOCK && 
						mCurrentState != READING_PROPERTIES_BLOCK && 
						mCurrentState != READING_LOCATIONBINDINGS_BLOCK)
					{
						flushShaderPart(std::shared_ptr<AbstractShaderPart>(new SimpleShaderPart));
					}
					break;
				}
				case READING_VERTEX_SHADER:
				case READING_FRAGMENT_SHADER:
				{
					parseLine(line, lineStr);
					break;
				}
			}
		}
	}
}

void
ShaderBlockParser::parseLine(std::string const& line, std::stringstream& lineStr) 
{
	// read first word to check for macro
	std::string macro;
	std::string macroPrefix = "#";
	lineStr >> macro;
	if(!macro.compare(0,macroPrefix.size(), macroPrefix)) 
	{
		// macro detected
		// ELSE MACRO IS NO CHANGE OF MACRO, SO WE DONT FLUSH HERE
		if(macro != "#ELSE") 
		{
			flushShaderPart(std::shared_ptr<AbstractShaderPart>(new SimpleShaderPart()));
		}
		if(macro == "#INCLUDE") 
		{
			parseIncludeMacro(line);
		}
		else if(macro == "#FOR") 
		{
		}
		else if(macro == "#ENDFOR") 
		{
		}
		else if(macro == "#IF") 
		{
			std::string resource, comparator, genericValue;
			lineStr >> resource >> comparator >> genericValue;

			flushShaderPart(std::shared_ptr<AbstractShaderPart>(new IfElseMacro(resource, comparator, genericValue)));
		}
		else if(macro == "#ELSE")
		{
			mCurrentShaderPart->append(macro); // this switches the macro -> now it appends to the else part
		}
		else if(macro == "#ENDIF") 
		{
			flushShaderPart(std::shared_ptr<AbstractShaderPart>(new SimpleShaderPart()));
		}
		else if(macro == "#FRAGMENTPROGRAM") 
		{
			flushShaderPart(std::shared_ptr<AbstractShaderPart>(new SimpleShaderPart));
			mCurrentState = READING_FRAGMENT_SHADER;
		}
		else if(macro == "#VERTEXPROGRAM") 
		{
			flushShaderPart(std::shared_ptr<AbstractShaderPart>(new SimpleShaderPart));
			mCurrentState = READING_VERTEX_SHADER;
		}
	}
	else
	{
		mCurrentShaderPart->append(line);
	}
}

void
ShaderBlockParser::flushShaderPart(std::shared_ptr<AbstractShaderPart> newPart) 
{
	if(mCurrentShaderPart.get() != nullptr && mCurrentShaderPart->getNumPermutations() > 0) 
	{
		if(mCurrentState == READING_VERTEX_SHADER) 
		{
			mVertexShaderParts.push_back(mCurrentShaderPart);
		}
		else if(mCurrentState == READING_FRAGMENT_SHADER) 
		{
			mFragmentShaderParts.push_back(mCurrentShaderPart);
		}
	}
	mCurrentShaderPart = newPart;
}

void
ShaderBlockParser::parseIncludeMacro(std::string const& line) 
{
	// include file/resource into shader
	std::string name;
	size_t startPos = line.find_first_of("\"");
	size_t endPos = line.find_last_of("\"");
	if(startPos != std::string::npos && endPos != std::string::npos) 
	{
		name = line.substr(startPos+1, endPos-startPos-1);
	}
	mCurrentShaderPart = getInclude(name);
	flushShaderPart(std::shared_ptr<AbstractShaderPart>(new SimpleShaderPart()));
}

std::shared_ptr<AbstractShaderPart>
ShaderBlockParser::getInclude(std::string const& resource) const 
{
	std::string name = resource;
	// check if resource is a default resource of the engine
	std::string defaultResourcePrexif = "G2.";
	if(!name.compare(0,defaultResourcePrexif.size(), defaultResourcePrexif)) 
	{
		// append shading language shortcut to include name
		if(mBuilder->shadingLanguage== ShadingLanguage::GLSL)
		{
			name.append(".GLSL");
		}
		if(mBuilder->shadingLanguage == ShadingLanguage::CG)
		{
			name.append(".CG");
		}
	}
	auto it = gDefaultIncludes.find(name);
	if(it != gDefaultIncludes.end()) {
		return it->second;
	}
	// TODO: try to include from file
	return std::shared_ptr<AbstractShaderPart>();
}

std::unordered_map<std::string,std::shared_ptr<AbstractShaderPart>> 
ShaderBlockParser::initDefaultIncludes() 
{
	std::unordered_map<std::string,std::shared_ptr<AbstractShaderPart>> defaultIncludes;
	
	defaultIncludes.insert(
		std::make_pair("G2.matrices.GLSL", std::shared_ptr<AbstractShaderPart>(new MatricesDefaultInclude(ShadingLanguage::GLSL))));
	defaultIncludes.insert(
		std::make_pair("G2.material.GLSL", std::shared_ptr<AbstractShaderPart>(new MaterialDefaultInclude(ShadingLanguage::GLSL))));
	defaultIncludes.insert(
		std::make_pair("G2.light.GLSL", std::shared_ptr<AbstractShaderPart>(new LightDefaultInclude(ShadingLanguage::GLSL))));

	defaultIncludes.insert(
		std::make_pair("G2.matrices.CG", std::shared_ptr<AbstractShaderPart>(new MatricesDefaultInclude(ShadingLanguage::CG))));
	defaultIncludes.insert(
		std::make_pair("G2.material.CG", std::shared_ptr<AbstractShaderPart>(new MaterialDefaultInclude(ShadingLanguage::CG))));
	defaultIncludes.insert(
		std::make_pair("G2.light.CG", std::shared_ptr<AbstractShaderPart>(new LightDefaultInclude(ShadingLanguage::CG))));
	return defaultIncludes;
}

std::unordered_map<std::string,std::shared_ptr<AbstractShaderPart>> ShaderBlockParser::gDefaultIncludes = ShaderBlockParser::initDefaultIncludes();