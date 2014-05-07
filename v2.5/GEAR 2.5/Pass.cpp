#include "Pass.h"

using namespace G2;

Pass::Pass( std::vector<std::shared_ptr<Shader>> shaderPermutations, 
			std::vector<Property> const& globalProperties,
			std::unordered_map<std::string,Setting> settings) 
	: mShaderPermutations(shaderPermutations),
	mProperties(globalProperties),
	mSettings(settings)
{
}
