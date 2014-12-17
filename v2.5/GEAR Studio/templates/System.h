#pragma once
#include <G2Core/BaseSystem.h>

namespace ##NS##
{
	class ##COMPONENT##;

	class ##SYSTEM## : public G2::BaseSystem<##SYSTEM##,##COMPONENT##> 
	{
		public:

			void runPhase(std::string const& name, G2::FrameInfo const& frameInfo);
	};
};