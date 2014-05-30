#include "PhysicsComponent.h"
#include "PhysicsSystem.h"

#include <G2/TransformComponent.h>

#include <Newton/newton/Newton.h>
#include <Newton/dMath/dVector.h>
#include <Newton/dMath/dQuaternion.h>
#include <Newton/dMath/dMatrix.h>
#include <glm/glm.hpp>

using namespace G2::Physics;

// http://newtondynamics.com/wiki/index.php5?title=Collision_primitives

PhysicsComponent::PhysicsComponent(glm::vec4 const& gravity) 
	: mDynamicsWorld(ECSManager::getShared().getSystem<PhysicsSystem,PhysicsComponent>()->mDynamicsWorld),
	mLocalGravity(gravity)
{ }

PhysicsComponent::PhysicsComponent(PhysicsComponent && rhs) 
	: mDynamicsWorld(nullptr)
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

PhysicsComponent&
PhysicsComponent::operator=(PhysicsComponent && rhs) 
{
	mCollisionShapes = std::move(rhs.mCollisionShapes);
	mDynamicsWorld = rhs.mDynamicsWorld;
	mRigidBodies = std::move(rhs.mRigidBodies);
	mLocalGravity = std::move(rhs.mLocalGravity);
	
	rhs.mCollisionShapes.clear();
	rhs.mDynamicsWorld = nullptr;
	rhs.mRigidBodies.clear();
	
	return static_cast<PhysicsComponent&>(BaseComponent::operator=(std::move(rhs)));
}

PhysicsComponent::~PhysicsComponent() 
{
	for(int i = 0; i < mRigidBodies.size(); ++i)
	{
		PhysicsComponentUserData* entityData = (PhysicsComponentUserData*)NewtonBodyGetUserData(mRigidBodies[i]);
		delete entityData;
	}
	if(mDynamicsWorld != nullptr)
	{
		// not moved
		// clean up collision listener
		ECSManager::getShared().getSystem<PhysicsSystem,PhysicsComponent>()->_onRemovePhysicsComponent(getEntityId());
	}

}

int
PhysicsComponent::add(
	CollisionShapeDescriptor const& collisionShapeDescriptor, 
	RigidBodyDescriptor const& rigidBodyDescriptor, 
	glm::vec3 const& position,
	glm::quat const& orientation) 
{
	float mass = rigidBodyDescriptor.mass;
	
	glm::vec3 pos = position;
	// make and offset Matrix for this collision shape.
	dMatrix collisionShapePositionMatrix(GetIdentityMatrix());
	collisionShapePositionMatrix.m_posit = dVector(pos.x, pos.y, pos.z);
	
	switch(collisionShapeDescriptor.getShape())
	{
		case CollisionShape::SPHERE:
			mCollisionShapes.push_back(NewtonCreateSphere(mDynamicsWorld, collisionShapeDescriptor.getRadius(), 0, nullptr));
			break;
		case CollisionShape::BOX: // normal used to store half extends
			mCollisionShapes.push_back(NewtonCreateBox(mDynamicsWorld, collisionShapeDescriptor.getNormal().x * 2.f, collisionShapeDescriptor.getNormal().y * 2.f, collisionShapeDescriptor.getNormal().z * 2.f, 0, nullptr));
			break;
		case CollisionShape::CYLINDER:
			assert(0); // not implemented

			//mCollisionShape = new btCylinderShape(
			//	btVector3(
			//		collisionShapeDescriptor.getNormal().x,
			//		collisionShapeDescriptor.getNormal().y,
			//		collisionShapeDescriptor.getNormal().z
			//	)); // normal used to store half extends
			break;
		case CollisionShape::CAPSULE:
			//mCollisionShape = new btCapsuleShape(collisionShapeDescriptor.getRadius(), collisionShapeDescriptor.getConstant());
			assert(0); // not implemented
			break;
		case CollisionShape::CONE:
			//mCollisionShape = new btConeShape(collisionShapeDescriptor.getRadius(), collisionShapeDescriptor.getConstant());
			assert(0); // not implemented
			break;
		case CollisionShape::MULTISPHERE:
			assert(0); // not implemented
			break;
		case CollisionShape::CONVEX_HULL:
			{
			assert(0); // not implemented
				//btScalar const* pts = (btScalar const*)collisionShapeDescriptor.getVertexStream().get();
				//mCollisionShape = new btConvexHullShape(pts, collisionShapeDescriptor.getVertexStream().getNumWrittenVertices(), sizeof(glm::vec3));
			}
			break;
		case CollisionShape::TRIANGLE_MESH:
			{
			assert(0); // not implemented
				//btScalar const* pts = (btScalar const*)collisionShapeDescriptor.getVertexStream().get();
				//btTriangleMesh* triMesh = new btTriangleMesh;
				//for(unsigned int i = 0; i < collisionShapeDescriptor.getVertexStream().getNumWrittenVertices(); )
				//{
				//	glm::vec3 const& p1 = ((glm::vec3 const*)pts)[i++];
				//	glm::vec3 const& p2 = ((glm::vec3 const*)pts)[i++];
				//	glm::vec3 const& p3 = ((glm::vec3 const*)pts)[i++];
				//	G2::logger << "Added triangle " << i << ": " << p1 << ":" << p2 << ":" << p3 << "\n";
				//	triMesh->addTriangle(
				//		btVector3(p1.x,p1.y,p1.z),
				//		btVector3(p3.x,p3.y,p3.z),
				//		btVector3(p2.x,p2.y,p2.z),
				//		false
				//	);
				//}
				//mCollisionShape = new btBvhTriangleMeshShape(triMesh, false);
				//mass = 0.f;
			}
			break;
		case CollisionShape::HEIGHTFIELD_TERRAIN:
			assert(0); // not implemented
			break;
		case CollisionShape::STATIC_PLANE:
			assert(0); // not implemented
			//mCollisionShape = new btStaticPlaneShape(btVector3(collisionShapeDescriptor.getNormal().x,collisionShapeDescriptor.getNormal().y,collisionShapeDescriptor.getNormal().z),collisionShapeDescriptor.getConstant());
			break;
		default:
			assert(0);
			break;
	}
	mRigidBodies.push_back(NewtonCreateDynamicBody(mDynamicsWorld, mCollisionShapes.back(), &collisionShapePositionMatrix[0][0]));
	
	NewtonBodySetVelocity(mRigidBodies.back(), glm::value_ptr(rigidBodyDescriptor.linearVelocity));
	

	PhysicsComponentUserData* userData = new PhysicsComponentUserData;
	userData->entityId = getEntityId();
	userData->rigidBodyDescriptor = rigidBodyDescriptor;
	userData->physicsSystem = ECSManager::getShared().getSystem<PhysicsSystem,PhysicsComponent>();
	userData->transformSystem = ECSManager::getShared().getSystem<G2::TransformSystem,G2::TransformComponent>();
	if(rigidBodyDescriptor.material != -1)
	{
		userData->materialId = rigidBodyDescriptor.material;
	}
	else
	{
		userData->materialId = NewtonMaterialGetDefaultGroupID(mDynamicsWorld);
	}
	NewtonBodySetUserData (mRigidBodies.back(), userData);
	
	dVector origin;
	dVector inertia;
	// we need to set the proper center of mass and inertia matrix for this body
	// the inertia matrix calculated by this function does not include the mass.
	// therefore it needs to be multiplied by the mass of the body before it is used.
	NewtonConvexCollisionCalculateInertialMatrix (mCollisionShapes.back(), &inertia[0], &origin[0]);	

	// set the body mass matrix
	NewtonBodySetMassMatrix (mRigidBodies.back(), mass, mass * inertia.m_x, mass * inertia.m_y, mass * inertia.m_z);

	// set the body origin
	NewtonBodySetCentreOfMass (mRigidBodies.back(), &origin[0]);

	// set the function callback to apply the external forces and torque to the body
	// the most common force is Gravity
	NewtonBodySetForceAndTorqueCallback (mRigidBodies.back(), PhysicsSystem::applyForceAndTorqueCallback);

	// set the function callback to set the transformation state of the graphic entity associated with this body 
	// each time the body change position and orientation in the physics world
	NewtonBodySetTransformCallback (mRigidBodies.back(), PhysicsSystem::setTransformCallback);

	NewtonDestroyCollision(mCollisionShapes.back());


	NewtonMaterialSetCollisionCallback (mDynamicsWorld, 0, 0, nullptr, nullptr, PhysicsSystem::genericContactProcess);
	return (int)mRigidBodies.size()-1;
}

G2::Physics::PhysicsMaterial&
PhysicsComponent::getMaterial(int i) 
{
	return ((PhysicsComponentUserData*)NewtonBodyGetUserData(mRigidBodies[i]))->materialId;
}

NewtonBody*
PhysicsComponent::getRigidBody(int index) 
{
	return mRigidBodies[index];
}