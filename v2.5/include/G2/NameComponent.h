// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "NameSystem.h"

#include <G2Core/BaseComponent.h>

namespace G2 
{
	/** This class defines a Component only having a name.
	 * This enables you to assign a human readable name to your entities.
	 * @created:	2014/01/30
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class NameComponent : public BaseComponent<NameSystem> 
	{
		public:

			NameComponent() {}

			NameComponent(std::string const& name) : name(name) {}

			std::string name;
	};
};