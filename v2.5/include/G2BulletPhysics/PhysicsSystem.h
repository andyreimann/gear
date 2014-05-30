#pragma once
#include "PhysicsDebugDrawer.h"

#include <G2Core/BaseSystem.h>
#include <btBulletCollisionCommon.h>


class btDefaultCollisionConfiguration;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btDynamicsWorld;

class btCollisionShape;
struct btDefaultMotionState;
class btRigidBody;
class btGhostPairCallback;

#define GEAR_SINGLE_THREADED
//#define GEAR_GLOBAL_COLLISIONS

namespace G2 
{
	namespace Physics 
	{
		class PhysicsComponent;
		/// This class defines a PhysicsSystem which takes care of calculating physics.
		/// @created:	2014/05/07
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class PhysicsSystem : public BaseSystem<PhysicsSystem,PhysicsComponent>//, btCollisionWorld::ContactResultCallback
		{
			friend class PhysicsComponent;
			public:
				PhysicsSystem();
				void runPhase(std::string const& name, G2::FrameInfo const& frameInfo);
				
				virtual bool runsOnMainThread() override;
				virtual bool runsOnSideThread() override;
#ifdef GEAR_GLOBAL_COLLISIONS
				void tick(btScalar timeStep);
#endif

				~PhysicsSystem();

			private:

				/// Collision callback for bullet
				//virtual	btScalar addSingleResult(btManifoldPoint& cp,btCollisionObjectWrapper const* colObj0Wrap,int partId0,int index0, btCollisionObjectWrapper const* colObj1Wrap,int partId1,int index1);
				

				btDefaultCollisionConfiguration*		mCollisionConfiguration;
				btBroadphaseInterface*					mBroadphase;
				btCollisionDispatcher*					mDispatcher;
				btConstraintSolver*	mSolver;
				btDynamicsWorld*						mDynamicsWorld;
				btGhostPairCallback*					mGhostPairCallback;
				int										mMaxSimulationSubsteps;
				DebugDrawer								mDebugDrawer;
		};
	};
};