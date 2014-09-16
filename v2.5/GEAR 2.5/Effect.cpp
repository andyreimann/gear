#include "Effect.h"
#include "Shader.h"
#include "Logger.h"
#include "Shader.h"

#include <G2Core/GfxDevice.h>

using namespace G2;

std::shared_ptr<Shader>
Effect::getShader(Material const* material, VertexArrayObject const* vao) const
{
	if(material == nullptr || vao == nullptr)
	{
		return mShaderPermutations.front();
	}
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
			if(!conditions[k].check(*material,*vao))
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
			// check if less condition checks failed
			bestFitFailCount = failCount;
			bestFit = mShaderPermutations[i];
		}
	}
	return std::shared_ptr<Shader>();
}

bool
Effect::hasCompiledShaders() const 
{
	return mShaderPermutations.size() > 0;
}

std::shared_ptr<Effect>
Effect::Builder::buildResource() 
{
	// create new Effect
	std::shared_ptr<Effect> effect = std::shared_ptr<Effect>(new Effect());

	effect->mShaderPermutations = shaderPermutations;
	//effect->mPasses.resize(passes.size()); // no default ctor
	for(int i = 0; i < passes.size(); ++i)
	{
		Pass::Builder const& passBuilder = passes[i];
		if(passBuilder.renderTargetSampler != Sampler::SAMPLER_INVALID &&
		   passBuilder.renderTargetType != RenderTargetType::RT_INVALID)
		{
			effect->mPasses.push_back(std::move(Pass(passBuilder.shaderPermutations, passBuilder.settings, passBuilder.renderTargetSampler, passBuilder.renderTargetType)));
		}
	}
	return effect;
}

Effect::Builder::Builder()
	: shadingLanguage("") 
{
}

void
Effect::Builder::setShadingLanguage(std::string const& shadingLanguage) 
{
	if(!G2_gfxDevice()->supportsShadingLanguage(shadingLanguage))
	{
		logger << "[Effect::Builder] -> Error: Shading Language '" << shadingLanguage << "' not supported by current GfxDevice!" << endl;
		return;
	}
	logger << "[Effect::Builder] -> Use Shading Language " << shadingLanguage << endl;
	this->shadingLanguage = shadingLanguage;
}

void
Effect::Builder::addVertexShaderParts(std::vector<std::shared_ptr<AbstractShaderPart>> const& parts) 
{
	for(auto it = parts.begin(); it != parts.end(); ++it)
	{
		vertexShaderParts.push_back(*it);
	}
}

void
Effect::Builder::addGeometryShaderParts(std::vector<std::shared_ptr<AbstractShaderPart>> const& parts) 
{
	for(auto it = parts.begin(); it != parts.end(); ++it)
	{
		geometryShaderParts.push_back(*it);
	}
}

void
Effect::Builder::addFragmentShaderParts(std::vector<std::shared_ptr<AbstractShaderPart>> const& parts) 
{
	for(auto it = parts.begin(); it != parts.end(); ++it)
	{
		fragmentShaderParts.push_back(*it);
	}
}

void _compile(
	std::string const& shadingLanguage,
	std::vector<LocationBinding> const& locationBindings,
	std::vector<Property> const& properties,
	std::vector<std::shared_ptr<AbstractShaderPart>> const& vertexShaderParts,
	std::vector<std::shared_ptr<AbstractShaderPart>> const& geometryShaderParts,
	std::vector<std::shared_ptr<AbstractShaderPart>> const& fragmentShaderParts,
	ShaderMetaData const& shaderMetaData,
	std::vector<std::shared_ptr<Shader>>& target
	)
{
	if(!G2_gfxDevice()->supportsShadingLanguage(shadingLanguage))
	{
		return;
	}

	// build shader headers
	std::string vertexShaderHeader = "", geometryShaderHeader = "", fragmentShaderHeader = "";

	
	if(shadingLanguage == "GLSL") 
	{
		vertexShaderHeader = fragmentShaderHeader = "#version 330\n\n";
		if(geometryShaderParts.size() > 0)
		{
			geometryShaderHeader = "#version 330\n\n";
		}
	}

	// create code for location bindings in vertex shader
	for (int i = 0; i < locationBindings.size() ; ++i) 
	{
		vertexShaderHeader.append(locationBindings[i].getShaderCode());
		vertexShaderHeader.append("\n");
	}
	vertexShaderHeader.append("\n");
	
	// create code for properties/uniforms in vertex shader
	for (int i = 0; i < properties.size() ; ++i) 
	{
		vertexShaderHeader.append(properties[i].getShaderCode());
		vertexShaderHeader.append("\n");
		if(geometryShaderParts.size() > 0)
		{
			geometryShaderHeader.append(properties[i].getShaderCode());
			geometryShaderHeader.append("\n");
		}
		fragmentShaderHeader.append(properties[i].getShaderCode());
		fragmentShaderHeader.append("\n");
	}
	vertexShaderHeader.append("\n");
	if(geometryShaderParts.size() > 0)
	{
		geometryShaderHeader.append("\n");
	}
	fragmentShaderHeader.append("\n");

	// calculate the number of permutations to create
	int numPermutations = 1;
	for (int i = 0; i < vertexShaderParts.size() ; ++i) 
	{
		numPermutations *= vertexShaderParts[i]->getNumPermutations();
	}
	for (int i = 0; i < geometryShaderParts.size() ; ++i) 
	{
		numPermutations *= geometryShaderParts[i]->getNumPermutations();
	}
	for (int i = 0; i < fragmentShaderParts.size() ; ++i) 
	{
		numPermutations *= fragmentShaderParts[i]->getNumPermutations();
	}
	logger << "[Effect::Builder] -> Build and compile " << numPermutations << " Shader" << endl;
	
	// create permutations
	int numVSParts = (int)vertexShaderParts.size();
	int numGSParts = (int)geometryShaderParts.size();
	int numFSParts = (int)fragmentShaderParts.size();
	std::vector<int> permutationIndexAtPosition(numVSParts+numGSParts+numFSParts);
	bool increasedThisRound = false;

	for (int i = 0; i < numPermutations ; ++i) 
	{
		std::vector<MacroCondition> shaderConditions;
		increasedThisRound = false;
		
		std::string vertexShaderCode = vertexShaderHeader, geometryShaderCode = geometryShaderHeader, fragmentShaderCode = fragmentShaderHeader;

		for (int k = 0; k < numVSParts; ++k) 
		{
			int numPermutationsOfPart = vertexShaderParts[k]->getNumPermutations();
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
			vertexShaderCode.append(vertexShaderParts[k]->getPermutedShaderPart(permutationIndexAtPosition[k]));
			vertexShaderCode.append("\n");
			// collect all conditions
			std::vector<MacroCondition> partConditions = vertexShaderParts[k]->getMacroConditions(permutationIndexAtPosition[k]);
			shaderConditions.insert( shaderConditions.end(), partConditions.begin(), partConditions.end() );
		}
		for (int k = 0; k < numGSParts; ++k) 
		{
			int numPermutationsOfPart = geometryShaderParts[k]->getNumPermutations();
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
			geometryShaderCode.append(geometryShaderParts[k]->getPermutedShaderPart(permutationIndexAtPosition[numVSParts+k]));
			geometryShaderCode.append("\n");
			// collect all conditions
			std::vector<MacroCondition> partConditions = geometryShaderParts[k]->getMacroConditions(permutationIndexAtPosition[numVSParts+k]);
			shaderConditions.insert( shaderConditions.end(), partConditions.begin(), partConditions.end() );
		}
		for (int k = 0; k < numFSParts; ++k) 
		{
			int numPermutationsOfPart = fragmentShaderParts[k]->getNumPermutations();
			if(!increasedThisRound && i > 0 && numPermutationsOfPart > 1) 
			{
				// try to increase permutation index of part
				// which provides multiple permutations
				if(permutationIndexAtPosition[numVSParts+numGSParts+k] < numPermutationsOfPart-1)
				{
					++permutationIndexAtPosition[numVSParts+numGSParts+k];
					increasedThisRound = true;
				}
				else 
				{
					// reset permutation index
					permutationIndexAtPosition[numVSParts+numGSParts+k] = 0; 
				}
			}
			fragmentShaderCode.append(fragmentShaderParts[k]->getPermutedShaderPart(permutationIndexAtPosition[numVSParts+numGSParts+k]));
			fragmentShaderCode.append("\n");
			// collect all conditions
			std::vector<MacroCondition> partConditions = fragmentShaderParts[k]->getMacroConditions(permutationIndexAtPosition[numVSParts+numGSParts+k]);
			shaderConditions.insert( shaderConditions.end(), partConditions.begin(), partConditions.end() );
		}
		
		//logger << "[Effect::Builder] -> VertexShaderCode:\n" << vertexShaderCode;
		//if(geometryShaderParts.size() > 0)
		//{
		//	logger << "[Effect::Builder] -> GeometryShaderCode:\n" << geometryShaderCode;
		//}
		//logger << "[Effect::Builder] -> FragmentShaderCode:\n" << fragmentShaderCode;
		
		auto shader = std::shared_ptr<Shader>(new Shader());
		if(Effect::Builder::compileAndApplyMetaData(shadingLanguage,vertexShaderCode, geometryShaderCode, fragmentShaderCode, shaderMetaData, shader))
		{
			// attach a list of conditions
			// to the shader, which are used for the decision making process
			shader->setConditions(shaderConditions);
			target.push_back(shader);
		}
	}
}

bool
Effect::Builder::compileAndApplyMetaData(std::string const& shadingLanguage, std::string const& vertexShaderCode, std::string const& geometryShaderCode, std::string const& fragmentShaderCode, ShaderMetaData const& shaderMetaData, std::shared_ptr<Shader> const& shader) 
{
	bool compiled = shader->compile(shadingLanguage, vertexShaderCode,geometryShaderCode,fragmentShaderCode);
	if(compiled)
	{
		// preset samplers and uniforms
		shader->initWithMetaData(shaderMetaData);
	}
	return compiled;
}

Effect::Builder&
Effect::Builder::buildAndCompile() 
{
	// compile main shader
	_compile(
		shadingLanguage, 
		locationBindings, 
		properties, 
		vertexShaderParts, 
		geometryShaderParts,
		fragmentShaderParts, 
		metaData, 
		shaderPermutations
	);
	
	// compile all passes
	for (int i = 0; i < passes.size() ; ++i) 
	{
		logger << "[Effect::Builder] Build Pass " << (i+1) << endl;
		Pass::Builder& passBuilder = passes[i];
		// compile one single pass

		if(passBuilder.vertexShaderParts.size() == 0 || passBuilder.fragmentShaderParts.size() == 0)
		{
			continue; // valid -> nothing to compile here
		}

		_compile(
			shadingLanguage, 
			passBuilder.locationBindings, 
			passBuilder.properties, 
			passBuilder.vertexShaderParts, 
			passBuilder.geometryShaderParts, 
			passBuilder.fragmentShaderParts, 
			passBuilder.metaData, 
			passBuilder.shaderPermutations
		);
	}
	return *this;
}