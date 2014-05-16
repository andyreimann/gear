// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include <G2Core/BaseSystem.h>

namespace G2 
{
	class TransformComponent;
	/** This class is responsible for the local space matrix updates of the loaded TransformComponent.
	 * The update itself takes place in the 'update' phase during rendering.
	 * @created:	2014/01/30
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class TransformSystem : public BaseSystem<TransformSystem,TransformComponent> 
	{
		public:

			void runPhase(std::string const& name, FrameInfo const& frameInfo);
	};
};