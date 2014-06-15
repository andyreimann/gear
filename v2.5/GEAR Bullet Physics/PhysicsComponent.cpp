#include "PhysicsComponent.h"
#include "PhysicsSystem.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <G2/TransformComponent.h>
#include <G2/Logger.h>
#include <G2Core/ECSManager.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


#include <G2/RenderComponent.h>

using namespace G2::Physics;

// TUT: http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Hello_World

G2::Physics::PhysicsComponent::PhysicsComponent(CollisionShapeDescriptor const& collisionShapeDescriptor, RigidBodyDescriptor const& rigidBodyDescriptor, glm::vec3 const& position, glm::quat const& orientation) 
	: mCollisionShape(nullptr),
	mRigidBody(nullptr),
	mMotionState(new btDefaultMotionState(
		btTransform(
			btQuaternion(
				orientation.x,
				orientation.y,
				orientation.z,
				orientation.w
			),
			btVector3(
				position.x,
				position.y,
				position.z
			)
		))
	),
	mTrackExactCollisions(false),
	mTrackAABBCollisions(false),
	mGhostObject(nullptr)
{
	float mass = rigidBodyDescriptor.mass;
	switch(collisionShapeDescriptor.getShape())
	{
		case CollisionShape::SPHERE:
			mCollisionShape = new btSphereShape(collisionShapeDescriptor.getRadius());
			break;
		case CollisionShape::BOX:
			mCollisionShape = new btBoxShape(
				btVector3(
					collisionShapeDescriptor.getNormal().x,
					collisionShapeDescriptor.getNormal().y,
					collisionShapeDescriptor.getNormal().z
				)); // normal used to store half extends
			break;
		case CollisionShape::CYLINDER:
			mCollisionShape = new btCylinderShape(
				btVector3(
					collisionShapeDescriptor.getNormal().x,
					collisionShapeDescriptor.getNormal().y,
					collisionShapeDescriptor.getNormal().z
				)); // normal used to store half extends
			break;
		case CollisionShape::CAPSULE:
			mCollisionShape = new btCapsuleShape(collisionShapeDescriptor.getRadius(), collisionShapeDescriptor.getConstant());
			break;
		case CollisionShape::CONE:
			mCollisionShape = new btConeShape(collisionShapeDescriptor.getRadius(), collisionShapeDescriptor.getConstant());
			break;
		case CollisionShape::MULTISPHERE:
			assert(0); // not implemented
			break;
		case CollisionShape::CONVEX_HULL:
			{
				btScalar const* pts = (btScalar const*)collisionShapeDescriptor.getVertexStream().get();
				mCollisionShape = new btConvexHullShape(pts, collisionShapeDescriptor.getVertexStream().getNumWrittenVertices(), sizeof(glm::vec3));
			}
			break;
		case CollisionShape::TRIANGLE_MESH:
			{
				btScalar const* pts = (btScalar const*)collisionShapeDescriptor.getVertexStream().get();
				btTriangleMesh* triMesh = new btTriangleMesh;
				for(unsigned int i = 0; i < collisionShapeDescriptor.getVertexStream().getNumWrittenVertices(); )
				{
					glm::vec3 const& p1 = ((glm::vec3 const*)pts)[i++];
					glm::vec3 const& p2 = ((glm::vec3 const*)pts)[i++];
					glm::vec3 const& p3 = ((glm::vec3 const*)pts)[i++];
					G2::logger << "Added triangle " << i << ": " << p1 << ":" << p2 << ":" << p3 << "\n";
					triMesh->addTriangle(
						btVector3(p1.x,p1.y,p1.z),
						btVector3(p3.x,p3.y,p3.z),
						btVector3(p2.x,p2.y,p2.z),
						false
					);
				}
				mCollisionShape = new btBvhTriangleMeshShape(triMesh, false);
				mass = 0.f;
			}
			break;
		case CollisionShape::HEIGHTFIELD_TERRAIN:
			assert(0); // not implemented
			break;
		case CollisionShape::STATIC_PLANE:
			mCollisionShape = new btStaticPlaneShape(btVector3(collisionShapeDescriptor.getNormal().x,collisionShapeDescriptor.getNormal().y,collisionShapeDescriptor.getNormal().z),collisionShapeDescriptor.getConstant());
			break;
		default:
			break;
	}
	// calculate inertia and generate rigid body
	btVector3 inertia(0,0,0);
	if(collisionShapeDescriptor.getShape() != CollisionShape::TRIANGLE_MESH)
	{
		// moving not supported by bullet
		mCollisionShape->calculateLocalInertia(mass,inertia);
	}

	btRigidBody::btRigidBodyConstructionInfo rigidBodyConstructionInfo(mass,mMotionState,mCollisionShape,inertia);
	rigidBodyConstructionInfo.m_linearDamping = rigidBodyDescriptor.linearDamping;
	rigidBodyConstructionInfo.m_angularDamping = rigidBodyDescriptor.angularDamping;
	rigidBodyConstructionInfo.m_restitution = rigidBodyDescriptor.friction;
	rigidBodyConstructionInfo.m_friction = rigidBodyDescriptor.friction;
	rigidBodyConstructionInfo.m_rollingFriction = rigidBodyDescriptor.rollingFriction;
	rigidBodyConstructionInfo.m_linearSleepingThreshold = rigidBodyDescriptor.linearSleepingThreshold;
	rigidBodyConstructionInfo.m_angularSleepingThreshold = rigidBodyDescriptor.angularSleepingThreshold;
	rigidBodyConstructionInfo.m_additionalDamping = rigidBodyDescriptor.additionalDamping;
	rigidBodyConstructionInfo.m_additionalDampingFactor = rigidBodyDescriptor.additionalDampingFactor;
	rigidBodyConstructionInfo.m_additionalLinearDampingThresholdSqr = rigidBodyDescriptor.additionalLinearDampingThresholdSqr;
	rigidBodyConstructionInfo.m_additionalAngularDampingThresholdSqr = rigidBodyDescriptor.additionalAngularDampingThresholdSqr;
	rigidBodyConstructionInfo.m_additionalAngularDampingFactor = rigidBodyDescriptor.additionalAngularDampingFactor;
	mRigidBody = new btRigidBody(rigidBodyConstructionInfo);
	mRigidBody->setLinearVelocity(btVector3(rigidBodyDescriptor.linearVelocity.x,rigidBodyDescriptor.linearVelocity.y,rigidBodyDescriptor.linearVelocity.z));
	mRigidBody->setAngularVelocity(btVector3(rigidBodyDescriptor.angularVelocity.x,rigidBodyDescriptor.angularVelocity.y,rigidBodyDescriptor.angularVelocity.z));

	mRigidBody->setWorldTransform(btTransform(
				btQuaternion(
					orientation.x,
					orientation.y,
					orientation.z,
					orientation.w
				),
				btVector3(
					position.x,
					position.y,
					position.z
				)
			)
		);

	// add to physics world
	ECSManager::getShared().getSystem<PhysicsSystem,PhysicsComponent>()->mDynamicsWorld->addRigidBody(mRigidBody);
#ifdef GEAR_GHOST_COLLISIONS
	trackAABBCollisions(true);
#endif
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
	mTrackExactCollisions = rhs.mTrackExactCollisions;
	mTrackAABBCollisions = rhs.mTrackAABBCollisions;
	mGhostObject = rhs.mGhostObject;
	
	rhs.mCollisionShape = nullptr;
	rhs.mMotionState = nullptr;
	rhs.mRigidBody = nullptr;
	rhs.mTrackExactCollisions = false;
	rhs.mTrackAABBCollisions = false;
	rhs.mGhostObject = nullptr;
	
	return static_cast<PhysicsComponent&>(BaseComponent::operator=(std::move(rhs)));
}

PhysicsComponent::~PhysicsComponent() 
{
	
	auto* physicsSystem = ECSManager::getShared().getSystem<PhysicsSystem,PhysicsComponent>();
	if(mRigidBody != nullptr)
	{
		if(mRigidBody->getUserPointer() != nullptr)
		{
			delete mRigidBody->getUserPointer();
		}
		if(physicsSystem != nullptr && physicsSystem->mDynamicsWorld != nullptr)
		{
			physicsSystem->mDynamicsWorld->removeCollisionObject(mRigidBody);
		}
		delete mRigidBody;
	}
	if(mCollisionShape != nullptr)
	{
		delete mCollisionShape;
	}
	
	_cleanupAABBTracking();

	delete mMotionState;
}

PhysicsComponent&
PhysicsComponent::trackExactCollisions(bool mode) 
{
	mTrackExactCollisions = mode;
	return *this;
}

PhysicsComponent&
PhysicsComponent::trackAABBCollisions(bool mode) 
{
	mTrackAABBCollisions = mode;
	if(!mTrackAABBCollisions && mGhostObject != nullptr)
	{
		// remove tracking
		_cleanupAABBTracking();
	}
	else
	{
		// track
		mGhostObject = new btPairCachingGhostObject();
		mGhostObject->setCollisionShape(mCollisionShape);
		mGhostObject->setWorldTransform(mRigidBody->getWorldTransform());
		mGhostObject->setCollisionFlags( btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_NO_CONTACT_RESPONSE );
		ECSManager::getShared().getSystem<PhysicsSystem,PhysicsComponent>()->mDynamicsWorld->addCollisionObject(mGhostObject);
		mTrackExactCollisions = false;
	}
	return *this;
}

void
PhysicsComponent::_cleanupAABBTracking() 
{
	if(mGhostObject != nullptr)
	{
		// remove tracking
		if(mGhostObject->getUserPointer() != nullptr)
		{
			delete mGhostObject->getUserPointer();
		}
		auto* physicsSystem = ECSManager::getShared().getSystem<PhysicsSystem,PhysicsComponent>();
		if(physicsSystem != nullptr && physicsSystem->mDynamicsWorld != nullptr)
		{
			physicsSystem->mDynamicsWorld->removeCollisionObject(mGhostObject);
		}
		delete mGhostObject;
		mGhostObject = nullptr;
	}
}

void
PhysicsComponent::_updateGhostObjectCollisions(btDynamicsWorld* world) 
{
	if(mGhostObject == nullptr)
	{
		return;
	}
	auto* renderSystem = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>();
	btManifoldArray   manifoldArray;
	btBroadphasePairArray& pairArray = mGhostObject->getOverlappingPairCache()->getOverlappingPairArray();
	int numPairs = pairArray.size();

	for (int i=0;i<numPairs;++i)
	{
		manifoldArray.clear();

		const btBroadphasePair& pair = pairArray[i];

		//unless we manually perform collision detection on this pair, the contacts are in the dynamics world paircache:
		btBroadphasePair* collisionPair = world->getPairCache()->findPair(pair.m_pProxy0,pair.m_pProxy1);
		if (!collisionPair)
			continue;

		if (collisionPair->m_algorithm)
			collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);


		for (int j=0;j<manifoldArray.size();++j)
		{
			btPersistentManifold* manifold = manifoldArray[j];

			if(manifold->getBody0()->getUserPointer() == nullptr || manifold->getBody1()->getUserPointer() == nullptr)
			{
				continue; // cannot map to Entities
			}

			int entityA = *((int const*)manifold->getBody0()->getUserPointer()); 
			int entityB = *((int const*)manifold->getBody1()->getUserPointer()); 

			if(entityA == entityB)
			{
				continue;
			}

			btScalar directionSign = manifold->getBody0() == mGhostObject ? btScalar(-1.0) : btScalar(1.0);
			for (int p=0;p<manifold->getNumContacts();p++)
			{
				const btManifoldPoint&pt = manifold->getContactPoint(p);
				if (pt.getDistance()<0.f)
				{
					const btVector3& ptA = pt.getPositionWorldOnA();
					const btVector3& ptB = pt.getPositionWorldOnB();
					const btVector3& normalOnB = pt.m_normalWorldOnB;
					/// work here
					//G2::logger << "[PhysicsComponent#" << getEntityId() << "] Got ghost collision for Entity ID pair [" << entityA << ":" << entityB << "]" << G2::endl;
					
					//float r = (rand() % 100) / 100.f;

					//renderSystem->get(entityA)->material.setAmbient(glm::vec4(0.f,r,1-r,1.f));
					//renderSystem->get(entityB)->material.setAmbient(glm::vec4(0.f,r,1-r,1.f));
				}
			}
		}
	}
	//mGhostObject->setWorldTransform(mRigidBody->getWorldTransform());
}

bool
PhysicsComponent::isMovable() const 
{
	return !mCollisionShape->isNonMoving();
}

btScalar
PhysicsComponent::addSingleResult(btManifoldPoint& cp,btCollisionObjectWrapper const* colObj0Wrap,int partId0,int index0, btCollisionObjectWrapper const* colObj1Wrap,int partId1,int index1) 
{
	auto* physicsSystem = ECSManager::getShared().getSystem<PhysicsSystem,PhysicsComponent>();
	if(physicsSystem != nullptr && physicsSystem->mDynamicsWorld != nullptr)
	{
		btVector3 ptA = cp.getPositionWorldOnA();
		btVector3 ptB = cp.getPositionWorldOnB();
	
		int numManifolds = physicsSystem->mDynamicsWorld->getDispatcher()->getNumManifolds();
		for(int i = 0; i < numManifolds; ++i)
		{
			btPersistentManifold* contactManifold =  physicsSystem->mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject const* obA = static_cast<btCollisionObject const*>(contactManifold->getBody0());
			btCollisionObject const* obB = static_cast<btCollisionObject const*>(contactManifold->getBody1());

			if(obA->getUserPointer() == nullptr || obB->getUserPointer() == nullptr)
			{
				continue; // cannot map to Entities
			}
			int entityA = *((int const*)obA->getUserPointer()); 
			int entityB = *((int const*)obB->getUserPointer()); 
			if(entityA != getEntityId() && entityB != getEntityId())
			{
				continue; // not for me
			}
			int numContacts = contactManifold->getNumContacts();
			for (int j = 0; j < numContacts; ++j)
			{
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				if (pt.getDistance()<0.f)
				{
					const btVector3& ptA = pt.getPositionWorldOnA();
					const btVector3& ptB = pt.getPositionWorldOnB();
					const btVector3& normalOnB = pt.m_normalWorldOnB;

					G2::logger << "[PhysicsComponent#" << getEntityId() << "] Got collision for Entity ID pair [" << entityA << ":" << entityB << "]" << G2::endl;


				}
			}
		}
	}

	return 0;
}