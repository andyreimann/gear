#pragma once
#include "BaseSystem.h"

namespace G2 
{
	class LightComponent;
	/// This class holds all informations for the Lights to use at runtime.
	/// It takes care of recalculating the position/orientation of the Light objects.
	/// It is done in the 'postUpdate' phase.
	/// @created:	2014/04/29
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class LightSystem : public BaseSystem<LightSystem,LightComponent> 
	{
		friend class RenderSystem;
		public:
			void runPhase(std::string const& name, FrameInfo const& frameInfo);
	};
};