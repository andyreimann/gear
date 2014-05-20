#include "PhysicsSystem.h"
#include "PhysicsComponent.h"

#include <G2/Logger.h>
#include <G2/TransformComponent.h>

#include <thread>

///btBulletDynamicsCommon.h is the main Bullet include file, contains most common include files.
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
// If you introduce different types of collision object later (eg meshes using btGImpactMeshShape) then you may need to register a collision algorithm to get collisions recognised:
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

using namespace G2::Physics;


PhysicsSystem::PhysicsSystem() 
{
	mCollisionConfiguration = new btDefaultCollisionConfiguration();

	mBroadphase = new btDbvtBroadphase();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

	btGImpactCollisionAlgorithm::registerAlgorithm(mDispatcher);

	mSolver = new btSequentialImpulseConstraintSolver;

	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher,mBroadphase,mSolver,mCollisionConfiguration);

	mDynamicsWorld->setGravity(btVector3(0,-10,0));

	/*
	TESTgroundShape = new btStaticPlaneShape(btVector3(0,1,0),1);

	TESTfallShape = new btSphereShape(1);

	TESTgroundMotionState =
				new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));

	btRigidBody::btRigidBodyConstructionInfo
				groundRigidBodyCI(0,TESTgroundMotionState,TESTgroundShape,btVector3(0,0,0));
	TESTgroundRigidBody = new btRigidBody(groundRigidBodyCI);
	mDynamicsWorld->addRigidBody(TESTgroundRigidBody);

	TESTfallMotionState =
				new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));

	btScalar mass = 1;
		btVector3 fallInertia(0,0,0);
		TESTfallShape->calculateLocalInertia(mass,fallInertia);

	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,TESTfallMotionState,TESTfallShape,fallInertia);
	TESTfallRigidBody = new btRigidBody(fallRigidBodyCI);
	mDynamicsWorld->addRigidBody(TESTfallRigidBody);
	*/
}

void
PhysicsSystem::runPhase(std::string const& name, G2::FrameInfo const& frameInfo)
{
	if(name == "update") 
	{
		mDynamicsWorld->stepSimulation(frameInfo.timeSinceLastFrame,10);
		auto* transformSystem = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>();
		transformSystem->lock();

		for (int i = 0; i < components.size() ; ++i) 
		{
			PhysicsComponent& comp = components[i];

			if(comp.mIsSphere)
			{
				btTransform trans;
				comp.mRigidBody->getMotionState()->getWorldTransform(trans);
				G2::logger << "[PhysicsSystem] sphere height: " << trans.getOrigin().getY() << G2::endl;
				auto* transformComponent = transformSystem->get(comp.getEntityId());
				if(transformComponent != nullptr)
				{
					transformComponent->setPosition(
						glm::vec3(
							trans.getOrigin().getX(),
							trans.getOrigin().getY(),
							trans.getOrigin().getZ()
						)
					);
				}
			}
			//G2::logger << "Process PhysicsComponent " << comp.getEntityId() << " in thread " << std::this_thread::get_id() << G2::endl;
		}

		transformSystem->unlock();
	}
}

bool
PhysicsSystem::runsOnMainThread() 
{
	return false; // make it running on the side thread
}


PhysicsSystem::~PhysicsSystem() 
{
	delete mDynamicsWorld;
	delete mSolver;
	delete mDispatcher;
	delete mCollisionConfiguration;
	delete mBroadphase;
}