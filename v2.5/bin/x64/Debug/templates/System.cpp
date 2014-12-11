#include "##COMPONENT##.h"

using namespace ##NS##;

void
NameSystem::runPhase(std::string const& name, G2::FrameInfo const& frameInfo) 
{
	if (name == G2Core::ECSPhase::UPDATE)
	{
		// for every existing component of that type
		for(auto i = 0; i < components.size(); ++i) 
		{
			##COMPONENT##& comp = components[i];
			// do something 
		}
	}
}