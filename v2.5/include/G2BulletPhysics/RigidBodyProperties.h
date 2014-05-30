#pragma once

#include <glm/glm.hpp>

namespace G2 
{
	/// This class defines...
	/// @created:	2014/05/25
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	struct RigidBodyProperties 
	{
		RigidBodyProperties() :
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
			additionalAngularDampingFactor(0.f)
		{

		}
		float mass;
		float linearDamping;
		float angularDamping;
		///best simulation results when friction is non-zero
		float friction;
		///the m_rollingFriction prevents rounded shapes, such as spheres, cylinders and capsules from rolling forever.
		float rollingFriction;
		///best simulation results using zero restitution.
		float restitution;
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
