// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include <G2Core/BaseSystem.h>

namespace G2
{
	class NameComponent;
	/** This class defines...
	 * @created:	2014/01/30
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class NameSystem : public BaseSystem<NameSystem,NameComponent> 
	{
		public:

			void runPhase(std::string const& name, FrameInfo const& frameInfo);
	};
};