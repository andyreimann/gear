#pragma once
#include "Defines.h"

#include <G2Core/BaseSystem.h>


namespace G2 
{
	namespace Physics 
	{
		class PhysicsComponent;
		/// This class defines...
		/// @created:	2014/05/07
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class PhysicsSystem : public BaseSystem<PhysicsSystem,PhysicsComponent> 
		{
			public:
				PHYSICSDLL_API void runPhase(std::string const& name, G2::FrameInfo const& frameInfo);

		};
	};
};