#include "PhysicsComponent.h"
#include "PhysicsSystem.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <G2Core/ECSManager.h>

using namespace G2::Physics;

// TUT: http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Hello_World

PhysicsComponent::PhysicsComponent(float radius)
{
	mIsSphere = true;
	mCollisionShape = new btSphereShape(radius);
	mMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));
	
	btScalar mass = 1;
		btVector3 fallInertia(0,0,0);
	mCollisionShape->calculateLocalInertia(mass,fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,mMotionState,mCollisionShape,fallInertia);
	mRigidBody = new btRigidBody(fallRigidBodyCI);

	ECSManager::getShared().getSystem<PhysicsSystem,PhysicsComponent>()->mDynamicsWorld->addRigidBody(mRigidBody);
}

PhysicsComponent::PhysicsComponent(glm::vec3 const& normal, float constant)
{
	mIsSphere = false;
	mCollisionShape = new btStaticPlaneShape(btVector3(normal.x,normal.y,normal.z),constant);
	mMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
	
	// Mass of 0 is immovable static object!
	btRigidBody::btRigidBodyConstructionInfo
				groundRigidBodyCI(0,mMotionState,mCollisionShape,btVector3(0,0,0));
	mRigidBody = new btRigidBody(groundRigidBodyCI);

	ECSManager::getShared().getSystem<PhysicsSystem,PhysicsComponent>()->mDynamicsWorld->addRigidBody(mRigidBody);
}

PhysicsComponent::PhysicsComponent(PhysicsComponent && rhs) 
	: mCollisionShape(nullptr),
	mMotionState(nullptr),
	mRigidBody(nullptr)
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

PhysicsComponent&
PhysicsComponent::operator=(PhysicsComponent && rhs) 
{
	mCollisionShape = rhs.mCollisionShape;
	mMotionState = rhs.mMotionState;
	mRigidBody = rhs.mRigidBody;
	mIsSphere = rhs.mIsSphere;
	
	rhs.mCollisionShape = nullptr;
	rhs.mMotionState = nullptr;
	rhs.mRigidBody = nullptr;
	
	return static_cast<PhysicsComponent&>(BaseComponent::operator=(std::move(rhs)));
}


PhysicsComponent::~PhysicsComponent() 
{
	if(mRigidBody != nullptr)
	{
		ECSManager::getShared().getSystem<PhysicsSystem,PhysicsComponent>()->mDynamicsWorld->removeRigidBody(mRigidBody);
		delete mRigidBody;
	}
	delete mCollisionShape;
	delete mMotionState;
}