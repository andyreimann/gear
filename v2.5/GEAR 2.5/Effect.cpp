#include "Effect.h"
#include "Shader.h"

using namespace G2;

Effect::Effect() 
{
}

Effect::Effect(Effect const& rhs) 
{
	// do copy here
}

Effect&
Effect::operator=(Effect const& rhs) 
{
	// do assignment here
	return *this;
}

Effect::~Effect() 
{
}

std::shared_ptr<Shader>
Effect::getShader(Material const& material, VertexArrayObject const& vao) 
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

	effect->mShaderPermutations = uberShader.getAllShaders();

	return effect;
}