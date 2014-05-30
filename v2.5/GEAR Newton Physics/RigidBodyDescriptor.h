#pragma once

#include <glm/glm.hpp>

namespace G2 
{
	namespace Physics 
	{
		typedef int PhysicsMaterial;
		/// This class defines all properties for a rigid body.
		/// @created:	2014/05/29
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		struct RigidBodyDescriptor 
		{
			RigidBodyDescriptor() :
				mass(0.f),
				material(-1)
			{
			}
		
			RigidBodyDescriptor& setMass(float v) { mass = v; return *this; }
			RigidBodyDescriptor& setLinearVelocity(glm::vec3 const& v) { linearVelocity = v; return *this; }
			RigidBodyDescriptor& setAngularVelocity(glm::vec3 const& v) { angularVelocity = v; return *this; }
			RigidBodyDescriptor& setMaterial(PhysicsMaterial v) { material = v; return *this; }
			
			float mass;					// The mass of the rigid body
			PhysicsMaterial material;	// The Material of the rigid body
			glm::vec3 linearVelocity;	// The linear velocity of the rigid body
			glm::vec3 angularVelocity;	// The angular velocity of the rigid body
		};
	};
};
