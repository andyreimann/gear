#pragma once
#include <G2/BaseComponent.h>
#include "PhysicsSystem.h"
#include "Defines.h"

namespace G2 
{
	namespace Physics
	{
		/// This class defines...
		/// @created:	2014/05/07
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class PhysicsComponent : public BaseComponent<PhysicsSystem> 
		{
			public:
				/// This constructs a new PhysicsComponent.
				PHYSICSDLL_API PhysicsComponent();

			protected:
			private:

		};
	};
};