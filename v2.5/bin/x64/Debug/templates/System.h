#pragma once
#include "Defines.h"
#include <G2Core/BaseSystem.h>

namespace ##NS##
{
	class ##COMPONENT##;

	class ##SYSTEM## : public BaseSystem<##SYSTEM##,##COMPONENT##> 
	{
		public:

			DLL_API void runPhase(std::string const& name, G2::FrameInfo const& frameInfo);
	};
};