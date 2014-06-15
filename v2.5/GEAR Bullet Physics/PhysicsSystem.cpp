#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include <G2/CameraComponent.h>

#include <G2/Logger.h>
#include <G2/TransformComponent.h>
#include <G2/RenderComponent.h>
#include <G2Core/BaseComponent.h>

#include <glm/ext.hpp>
#include <thread>

///btBulletDynamicsCommon.h is the main Bullet include file, contains most common include files.
#include <btBulletDynamicsCommon.h>
// If you introduce different types of collision object later (e.g. meshes using btGImpactMeshShape) then you may need to register a collision algorithm to get collisions recognised:
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
//#include <BulletMultiThreaded/btParallelConstraintSolver.h>

using namespace G2::Physics;

#ifdef GEAR_GLOBAL_COLLISIONS
void localTickCallback(btDynamicsWorld *world, btScalar timeStep) {
	PhysicsSystem* physicsSystem = static_cast<PhysicsSystem*>(world->getWorldUserInfo());
	physicsSystem->tick(timeStep);
}
#endif

PhysicsSystem::PhysicsSystem() :
	mMaxSimulationSubsteps(5),
	mGhostPairCallback(nullptr)
{
	mCollisionConfiguration = new btDefaultCollisionConfiguration();

	mBroadphase = new btDbvtBroadphase();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

	btGImpactCollisionAlgorithm::registerAlgorithm(mDispatcher);
	
	mSolver = new btSequentialImpulseConstraintSolver;
	//mSolver = new btParallelConstraintSolver;

	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher,mBroadphase,mSolver,mCollisionConfiguration);

	mDynamicsWorld->setGravity(btVector3(0.f,-9.81f,0.f));

	
#ifdef GEAR_GLOBAL_COLLISIONS
	mDynamicsWorld->setInternalTickCallback(localTickCallback, static_cast<void*>(this), false);
#endif
	
#ifdef GEAR_GHOST_COLLISIONS
	// register ghost pair callback to track all ghost objects collisions
	mGhostPairCallback = new btGhostPairCallback();
	mDynamicsWorld->getPairCache()->setInternalGhostPairCallback(mGhostPairCallback);
#endif
	
	//mDynamicsWorld->setDebugDrawer(&mDebugDrawer);
}
#ifdef GEAR_GLOBAL_COLLISIONS
#include <stdlib.h>     /* srand, rand */
void
PhysicsSystem::tick(btScalar timeStep) 
{
	auto* renderSystem = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>();

	int numManifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold =  mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject const* obA = static_cast<btCollisionObject const*>(contactManifold->getBody0());
		btCollisionObject const* obB = static_cast<btCollisionObject const*>(contactManifold->getBody1());
		
		if(obA->getUserPointer() == nullptr || obB->getUserPointer() == nullptr)
		{
			continue; // cannot map
		}

		int entityA = *((int const*)obA->getUserPointer()); 
		int entityB = *((int const*)obB->getUserPointer()); 

		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;
				
				//float r = (rand() % 100) / 100.f;

				//renderSystem->get(entityA)->material.setAmbient(glm::vec4(1-r,r,0.f,1.f));
				//renderSystem->get(entityB)->material.setAmbient(glm::vec4(1-r,r,0.f,1.f));
			}
		}
	}
}
#endif

void
PhysicsSystem::runPhase(std::string const& name, G2::FrameInfo const& frameInfo)
{
	if(name == "render") 
	{
		//glUseProgram(0);
		//
		//auto* cameraSystem = ECSManager::getShared().getSystem<CameraSystem,CameraComponent>();
		//auto* transformSystem = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>();

		//CameraComponent* camera = cameraSystem->getRenderCamera();
		//assert(camera != nullptr);
		//
		//glm::mat4 cameraSpaceMatrix;
		//auto* cameraTransformation = transformSystem->get(camera->getEntityId());
		//
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//glMultMatrixf(glm::value_ptr(camera->getProjectionMatrix()));

		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		//glMultMatrixf(glm::value_ptr(cameraTransformation->getWorldSpaceMatrix()));

		//mDynamicsWorld->debugDrawWorld();

		//
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();

	}
#ifdef GEAR_SINGLE_THREADED
	else if(name == "preUpdate") 
#else
	else if(name == "updateSideThread") 
#endif
	{
		// TODO-COLLISION-IMPROVEMENTS: http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Things

		// http://www.bulletphysics.org/mediawiki-1.5.8/index.php?title=Stepping_The_World
		float fixedTimeStep = 1.f / 120.f;
		/*
		while(frameInfo.timeSinceLastFrame > (mMaxSimulationSubsteps * fixedTimeStep))
		{
			++mMaxSimulationSubsteps;
			G2::logger << "WARNING#"<< mMaxSimulationSubsteps << "\n";
		}*/
		//mDynamicsWorld->getDispatchInfo().m_debugDraw = &mDebugDrawer;
		mDynamicsWorld->stepSimulation((float)frameInfo.timeSinceLastFrame,mMaxSimulationSubsteps, fixedTimeStep);
		mDynamicsWorld->performDiscreteCollisionDetection();
		auto* transformSystem = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>();

		for(int i = 0; i < components.size() ; ++i) 
		{
			PhysicsComponent& comp = components[i];
			if(comp.mRigidBody->getUserPointer() == nullptr)
			{
				int* userData = new int;
				*userData = comp.getEntityId();
				comp.mRigidBody->setUserPointer(userData);
			}
			if(comp.mGhostObject != nullptr && comp.mGhostObject->getUserPointer() == nullptr)
			{
				int* userData = new int;
				*userData = comp.getEntityId();
				comp.mGhostObject->setUserPointer(userData);
			}

			//if(comp.isMovable())
			//{
				btTransform trans;
				comp.mRigidBody->getMotionState()->getWorldTransform(trans);

				transformSystem->lock();
				auto* transformComponent = transformSystem->get(comp.getEntityId());
				if(transformComponent == nullptr)
				{
					transformSystem->unlock();
					transformComponent = BaseComponent<TransformSystem>::create<TransformComponent>(comp.getEntityId());
					transformSystem->lock();
				}
				if(comp.isMovable())
				{ 
					transformComponent->setPosition(
						glm::vec3(
							trans.getOrigin().getX(),
							trans.getOrigin().getY(),
							trans.getOrigin().getZ()
						)
					);
					transformComponent->setRotation(
						glm::quat(
							-trans.getRotation().getZ(),
							trans.getRotation().getY(),
							-trans.getRotation().getX(),
							trans.getRotation().getW()
						)
					);
				}
				transformSystem->unlock();

				//if(comp.getTrackExactCollisions())
				//{
				//	mDynamicsWorld->contactTest(comp.mRigidBody,comp);
				//}
				
				// update ghost tracking if wanted
#ifdef GEAR_GHOST_COLLISIONS
				comp._updateGhostObjectCollisions(mDynamicsWorld);
				comp.mGhostObject->setWorldTransform(comp.mRigidBody->getWorldTransform());
#endif
			//}
		}

		
	}
}

bool
PhysicsSystem::runsOnMainThread() 
{
#ifdef GEAR_SINGLE_THREADED
	return true;
#else
	return false;
#endif
}

bool
PhysicsSystem::runsOnSideThread() 
{
#ifdef GEAR_SINGLE_THREADED
	return false;
#else
	return true;
#endif
}


PhysicsSystem::~PhysicsSystem() 
{
	delete mDynamicsWorld;
	delete mSolver;
	delete mDispatcher;
	delete mCollisionConfiguration;
	delete mBroadphase;
	mDynamicsWorld = nullptr;
	delete mGhostPairCallback;
}