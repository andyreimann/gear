#pragma once

#include <glm/glm.hpp>

namespace G2 
{
	namespace Physics 
	{
		/// This class defines all properties for a rigid body.
		/// @created:	2014/05/25
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		struct RigidBodyDescriptor 
		{
			RigidBodyDescriptor() :
				mass(0.f),
				linearDamping(0.f),
				angularDamping(0.f),
				friction(0.5f),
				rollingFriction(0.f),
				restitution(0.f),
				linearSleepingThreshold(0.8f),
				angularSleepingThreshold(1.f),
				additionalDamping(false),
				additionalDampingFactor(0.005f),
				additionalLinearDampingThresholdSqr(0.01f),
				additionalAngularDampingThresholdSqr(0.01f),
				additionalAngularDampingFactor(0.01f)
			{
			}
		
			RigidBodyDescriptor& setMass(float v) { mass = v; return *this; }
			RigidBodyDescriptor& setFriction(float v) { friction = v; return *this; }
			RigidBodyDescriptor& setRollingFriction(float v) { rollingFriction = v; return *this; }
			RigidBodyDescriptor& setRestitution(float v) { restitution = v; return *this; }
			RigidBodyDescriptor& setLinearVelocity(glm::vec3 const& v) { linearVelocity = v; return *this; }
			RigidBodyDescriptor& setAngularVelocity(glm::vec3 const& v) { angularVelocity = v; return *this; }
			RigidBodyDescriptor& setLinearDamping(float v) { linearDamping = v; return *this; }
			RigidBodyDescriptor& setAngularDamping(float v) { angularDamping = v; return *this; }

			float mass;
			float linearDamping;
			float angularDamping;
			float restitution;
			///best simulation results when friction is non-zero
			float friction;
			///the m_rollingFriction prevents rounded shapes, such as spheres, cylinders and capsules from rolling forever.
			float rollingFriction;
			float linearSleepingThreshold;
			float angularSleepingThreshold;

			//Additional damping can help avoiding low pass jitter motion, help stability for ragdolls etc.
			//Such damping is undesirable, so once the overall simulation quality of the rigid body dynamics system has improved, this should become obsolete
			bool additionalDamping;
			float additionalDampingFactor;
			float additionalLinearDampingThresholdSqr;
			float additionalAngularDampingThresholdSqr;
			float additionalAngularDampingFactor;
		
			glm::vec3 linearVelocity;
			glm::vec3 angularVelocity;
		};
	};
};
