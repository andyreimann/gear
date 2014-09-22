#include "ShadowDescriptor.h"
#include <algorithm>

using namespace G2;


G2::ShadowDescriptor::ShadowDescriptor(AdvancedShadowTechnique::Name technique) 
	: mTechnique(technique),
	mNumCascades(0),
	mCustomEffect("") 
{
}

ShadowDescriptor
ShadowDescriptor::noShadows() 
{
	return ShadowDescriptor(AdvancedShadowTechnique::NO_SHADOWS);
}

ShadowDescriptor
ShadowDescriptor::cascadedShadowMaps(unsigned int numCascades, std::string const& customEffect) 
{
	ShadowDescriptor descriptor = ShadowDescriptor(AdvancedShadowTechnique::CASCADED_SHADOW_MAPS);
	descriptor.mNumCascades = std::max<unsigned int>(1,std::min<unsigned int>(4,numCascades));
	descriptor.mCustomEffect = customEffect;
	descriptor.mSplitWeight = 0.95f;
	descriptor.mSplitDistFactor = 1.05f;
	return std::move(descriptor);
}