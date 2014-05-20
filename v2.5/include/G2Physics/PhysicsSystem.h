#pragma once

#include <G2Core/BaseSystem.h>


class btDefaultCollisionConfiguration;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

class btCollisionShape;
struct btDefaultMotionState;
class btRigidBody;

namespace G2 
{
	namespace Physics 
	{
		class PhysicsComponent;
		/// This class defines...
		/// @created:	2014/05/07
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class PhysicsSystem : public BaseSystem<PhysicsSystem,PhysicsComponent> 
		{
			friend class PhysicsComponent;
			public:
				PhysicsSystem();
				void runPhase(std::string const& name, G2::FrameInfo const& frameInfo);

				virtual bool runsOnMainThread() override;

				~PhysicsSystem();

			private:
				btDefaultCollisionConfiguration*		mCollisionConfiguration;
				btBroadphaseInterface*					mBroadphase;
				btCollisionDispatcher*					mDispatcher;
				btSequentialImpulseConstraintSolver*	mSolver;
				btDiscreteDynamicsWorld*				mDynamicsWorld;

				// Testing
				btCollisionShape*						TESTgroundShape;
				btCollisionShape*						TESTfallShape;

				btDefaultMotionState*					TESTgroundMotionState;
				btDefaultMotionState*					TESTfallMotionState;
				
				btRigidBody*							TESTfallRigidBody;
				btRigidBody*							TESTgroundRigidBody;
		};
	};
};