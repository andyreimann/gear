#pragma once
#include "RigidBodyDescriptor.h"

namespace G2 
{
	namespace Physics 
	{
		/// This class defines an interaction of 2 different materials.
		/// Whenever they collide with each other, the material settings 
		/// from this class will be taken into account.
		/// @created:	2014/05/29
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		struct PhysicsMaterialInteraction 
		{
				/// This constructs a new PhysicsMaterialInteraction.
				PhysicsMaterialInteraction()
					: materialA(-1),
					materialB(-1),
					staticFriction(0.5f),
					kineticFriction(0.3f),
					restitution(0.f),
					softness(0.5f)
				{}
				/// This constructs a new PhysicsMaterialInteraction.
				PhysicsMaterialInteraction(PhysicsMaterial materialA, PhysicsMaterial materialB, float friction, float restitution)
					: materialA(materialA),
					materialB(materialB),
					staticFriction(friction),
					kineticFriction(friction),
					restitution(restitution),
					softness(0.5f)
				{}

				PhysicsMaterial materialA;	
				PhysicsMaterial materialB;
				float staticFriction;		// The static friction of the material pair. Default: 0.5
				float kineticFriction;		// The kinetic friction of the material pair. Default: 0.3
				float restitution;			// The restitution of the material pair. Default: 0.0
				float softness;				// The softness of the material pair. Default: 0.5
		};
	};
};