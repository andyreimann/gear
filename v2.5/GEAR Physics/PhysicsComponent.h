#pragma once
#include <G2Core/BaseComponent.h>
#include "PhysicsSystem.h"

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
				PhysicsComponent();

			protected:
			private:

		};
	};
};