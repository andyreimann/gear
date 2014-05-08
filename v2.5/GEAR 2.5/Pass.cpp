#include "Pass.h"
#include "AbstractShaderPart.h"
#include "Shader.h"

using namespace G2;

Pass::Pass(std::vector<std::shared_ptr<Shader>> const& shaderPermutations, std::unordered_map<std::string,Setting> const& settings, Sampler::Name renderTargetSampler) 
	: mSettings(settings),
	mShaderPermutations(shaderPermutations),
	mRenderTarget(renderTargetSampler,
				  (unsigned)Pass::get("Width", mSettings, "512").toInt(),
				  (unsigned)Pass::get("Height", mSettings, "512").toInt())
{
}

Pass::Pass(Pass && rhs)
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

Pass& Pass::operator=(Pass && rhs)
{
	// 1. Stage: delete maybe allocated resources on target type
	// nothing here
	// 2. Stage: transfer data from src to target
	mSettings = std::move(rhs.mSettings);
	mShaderPermutations = std::move(rhs.mShaderPermutations);
	mRenderTarget = std::move(rhs.mRenderTarget);
	// 3. Stage: modify src to a well defined state

	return *this;
}

std::shared_ptr<Shader>
Pass::getShader(Material const& material, VertexArrayObject const& vao) const
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
			// check if less condition checks failed
			bestFitFailCount = failCount;
			bestFit = mShaderPermutations[i];
		}
	}
	return std::shared_ptr<Shader>();
}

void
Pass::Builder::addVertexShaderParts(std::vector<std::shared_ptr<AbstractShaderPart>> const& parts) 
{
	for(auto it = parts.begin(); it != parts.end(); ++it)
	{
		vertexShaderParts.push_back(*it);
	}
}

void
Pass::Builder::addFragmentShaderParts(std::vector<std::shared_ptr<AbstractShaderPart>> const& parts) 
{
	for(auto it = parts.begin(); it != parts.end(); ++it)
	{
		fragmentShaderParts.push_back(*it);
	}
}

Setting const&
Pass::get(std::string const& name, std::unordered_map<std::string,Setting>& settings, std::string const& defaultValue /*= ""*/) 
{
	auto it = settings.find(name);
	if(it != settings.end())
	{
		return it->second;
	}
	settings[name] = Setting(name, defaultValue);
	return settings[name];
}