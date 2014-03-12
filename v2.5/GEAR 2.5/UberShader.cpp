// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "UberShader.h"
#include "Shader.h"
#include "GlslShader.h"
#include "CgShader.h"
#include "Logger.h"

#include <sstream>
#include <limits>

using namespace G2;

UberShader::UberShader() 
	: mShadingLanguage(ShadingLanguage::UNKNOWN) {
}

bool
UberShader::hasCompiledShaders() const 
{
	return mShaderPermutations.size() > 0;
}

void
UberShader::add(LocationBinding const& locationBinding) 
{
	mLocationBindings.push_back(locationBinding);
}

void
UberShader::add(Property const& property) 
{
	mProperties.push_back(property);
}

void
UberShader::addVertexShaderPart(std::shared_ptr<AbstractShaderPart> const& part) 
{
	mVertexShaderParts.push_back(part);
}

void
UberShader::addFragmentShaderPart(std::shared_ptr<AbstractShaderPart> const& part) 
{
	mFragmentShaderParts.push_back(part);
}

UberShader&
UberShader::buildAndCompile() 
{

	// build shader headers
	std::string vertexShaderHeader = "", fragmentShaderHeader = "";

	
	if(mShadingLanguage == ShadingLanguage::GLSL) 
	{
		vertexShaderHeader = fragmentShaderHeader = "#version 330\n\n";
	}

	// create code for location bindings in vertex shader
	for (int i = 0; i < mLocationBindings.size() ; ++i) 
	{
		vertexShaderHeader.append(mLocationBindings[i].getShaderCode());
		vertexShaderHeader.append("\n");
	}
	vertexShaderHeader.append("\n");
	
	// create code for properties/uniforms in vertex shader
	for (int i = 0; i < mProperties.size() ; ++i) 
	{
		vertexShaderHeader.append(mProperties[i].getShaderCode());
		vertexShaderHeader.append("\n");
		fragmentShaderHeader.append(mProperties[i].getShaderCode());
		fragmentShaderHeader.append("\n");
	}
	vertexShaderHeader.append("\n");
	fragmentShaderHeader.append("\n");

	// calculate the number of permutations to create
	int numPermutations = 1;
	for (int i = 0; i < mVertexShaderParts.size() ; ++i) 
	{
		numPermutations *= mVertexShaderParts[i]->getNumPermutations();
	}
	for (int i = 0; i < mFragmentShaderParts.size() ; ++i) 
	{
		numPermutations *= mFragmentShaderParts[i]->getNumPermutations();
	}
	logger << "[UberShader] -> Build and compile " << numPermutations << " Shader" << endl;
	
	// create permutations
	int numVSParts = (int)mVertexShaderParts.size();
	int numFSParts = (int)mFragmentShaderParts.size();
	std::vector<int> permutationIndexAtPosition(numVSParts+numFSParts);
	bool increasedThisRound = false;

	for (int i = 0; i < numPermutations ; ++i) 
	{
		std::vector<MacroCondition> shaderConditions;
		increasedThisRound = false;
		
		std::string vertexShaderCode = vertexShaderHeader, fragmentShaderCode = fragmentShaderHeader;

		for (int k = 0; k < numVSParts; ++k) 
		{
			int numPermutationsOfPart = mVertexShaderParts[k]->getNumPermutations();
			if(!increasedThisRound && i > 0 && numPermutationsOfPart > 1) 
			{
				// try to increase permutation index of part
				// which provides multiple permutations
				if(permutationIndexAtPosition[k] < numPermutationsOfPart-1)
				{
					++permutationIndexAtPosition[k];
					increasedThisRound = true;
				}
				else 
				{
					// reset permutation index
					permutationIndexAtPosition[k] = 0; 
				}
			}
			vertexShaderCode.append(mVertexShaderParts[k]->getPermutedShaderPart(permutationIndexAtPosition[k]));
			vertexShaderCode.append("\n");
			// collect all conditions
			std::vector<MacroCondition> partConditions = mVertexShaderParts[k]->getMacroConditions(permutationIndexAtPosition[k]);
			shaderConditions.insert( shaderConditions.end(), partConditions.begin(), partConditions.end() );
		}
		for (int k = 0; k < numFSParts; ++k) 
		{
			int numPermutationsOfPart = mFragmentShaderParts[k]->getNumPermutations();
			if(!increasedThisRound && i > 0 && numPermutationsOfPart > 1) 
			{
				// try to increase permutation index of part
				// which provides multiple permutations
				if(permutationIndexAtPosition[numVSParts+k] < numPermutationsOfPart-1)
				{
					++permutationIndexAtPosition[numVSParts+k];
					increasedThisRound = true;
				}
				else 
				{
					// reset permutation index
					permutationIndexAtPosition[numVSParts+k] = 0; 
				}
			}
			fragmentShaderCode.append(mFragmentShaderParts[k]->getPermutedShaderPart(permutationIndexAtPosition[numVSParts+k]));
			fragmentShaderCode.append("\n");
			// collect all conditions
			std::vector<MacroCondition> partConditions = mFragmentShaderParts[k]->getMacroConditions(permutationIndexAtPosition[numVSParts+k]);
			shaderConditions.insert( shaderConditions.end(), partConditions.begin(), partConditions.end() );
		}
		
		logger << "[UberShader] -> VertexShaderCode:\n" << vertexShaderCode;
		logger << "[UberShader] -> FragmentShaderCode:\n" << fragmentShaderCode;
		
		if(mShadingLanguage == ShadingLanguage::GLSL) {
		
			auto shader = std::shared_ptr<Shader>(new GlslShader());
			if(shader->compile(vertexShaderCode,fragmentShaderCode))
			{
				// pre set samplers and uniforms
				shader->initWithMetaData(mMetaData);

				// attach a list of conditions
				// to the shader, which are used for the decision making process
				shader->setConditions(shaderConditions);
				mShaderPermutations.push_back(shader);
			}
		}
		else if(mShadingLanguage == ShadingLanguage::CG) {
		
			auto shader = std::shared_ptr<Shader>(new CgShader());
			if(shader->compile(vertexShaderCode,fragmentShaderCode))
			{
				// pre set samplers and uniforms
				shader->initWithMetaData(mMetaData);

				// attach a list of conditions
				// to the shader, which are used for the decision making process
				shader->setConditions(shaderConditions);
				mShaderPermutations.push_back(shader);
			}
		}
	}
	logger << "[UberShader] -> Compiled " << mShaderPermutations.size() << " Shader" << endl;
	return *this;
}

void
UberShader::setShadingLanguage(std::string const& shadingLanguage) 
{
	if(shadingLanguage == "GLSL") 
	{
		mShadingLanguage = ShadingLanguage::GLSL;
		std::cout << "[UberShader] -> Use Shading Language GLSL" << std::endl;
		//qDebug(log.str().c_str());
	}
	else if(shadingLanguage == "CG")
	{
		mShadingLanguage = ShadingLanguage::CG;
		std::cout << "[UberShader] -> Use Shading Language CG" << std::endl;
		//qDebug(log.str().c_str());
	}
	else {
		std::cout << "[UberShader] -> Error: Unknown Shading Language '" << shadingLanguage << "' given in Shader block!" << std::endl;
		//qDebug(log.str().c_str());
	}
}

std::shared_ptr<Shader>
UberShader::getShader(Material const& material, VertexArrayObject const& vao) 
{
	#undef max
	int bestFitFailCount = std::numeric_limits<int>::max();
	std::shared_ptr<Shader> bestFit = std::shared_ptr<Shader>();
	int num = (int)mShaderPermutations.size();
	for (int i = 0; i < num ; ++i) 
	{
		auto const& conditions = mShaderPermutations[i]->getConditions();
		int numConditions = (int)conditions.size();
		int failCount = 0; // number of checks failed for this Shader
		for (int k = 0; k < numConditions ; ++k) 
		{
			if(!conditions[k].check(material,vao))
			{
				++failCount;
			}
		}
		if(failCount == 0)
		{
			// best fit - all conditions met
			return mShaderPermutations[i];
		}
		else if(failCount < bestFitFailCount)
		{
			// check if more conditions have
			bestFitFailCount = failCount;
			bestFit = mShaderPermutations[i];
		}
	}
	return std::shared_ptr<Shader>();
}