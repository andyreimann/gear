#pragma once
#include "PhysicsSystem.h"
#include "CollisionShapeDescriptor.h"
#include "RigidBodyDescriptor.h"

#include <G2Core/BaseComponent.h>

#include <glm/ext.hpp>

struct NewtonCollision;
struct NewtonWorld;
struct NewtonBody;

namespace G2 
{
	class TransformSystem;
	namespace Physics
	{
		struct PhysicsComponentUserData
		{
			int entityId;
			int materialId;
			RigidBodyDescriptor rigidBodyDescriptor;
			PhysicsSystem* physicsSystem;
			TransformSystem* transformSystem;
			glm::vec4 gravity;
		};
		/// This class holds all relevant informations needed to describe a PhysicsComponent, which
		/// can hold several collision shapes together with their rigid bodies.
		/// @created:	2014/05/29
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class PhysicsComponent : public BaseComponent<PhysicsSystem>
		{


			friend class PhysicsSystem;
			public:
				/// This constructs a new PhysicsComponent
				/// @param gravity The gravity force to apply to all contained rigid bodies
				PhysicsComponent(glm::vec4 const& gravity = glm::vec4(0.f,-9.81f,0.f,1.f));
				/// This constructs a new PhysicsComponent 
				/// with the given collision shape descriptor
				/// @param collisionShapeDescriptor The descriptor for the collision shape
				/// @param rigidBodyDescriptor The descriptor for the rigid body
				/// @param position The initial position.
				/// @param orientation The initial orientation.
				/// @return The index of the newly added collision object. 
				/// @note The index can be used to retrieve and change informations 
				/// from the newly created rigid body such as material settings.
				int add(CollisionShapeDescriptor const& collisionShapeDescriptor, RigidBodyDescriptor const& rigidBodyDescriptor, glm::vec3 const& position = glm::vec3(), glm::quat const& orientation = glm::quat());
				/// Adds a listener method to listen for collisions
				/// @note Thread safe
				template<typename TargetT>
				void addCollisionListener(TargetT* t, void (TargetT::*method)(Collision const&))
				{
					std::lock_guard<std::mutex> lock(ECSManager::getShared().getSystem<PhysicsSystem,PhysicsComponent>()->mCollisionMutex);
					ECSManager::getShared().getSystem<PhysicsSystem,PhysicsComponent>()->mCollisionCallbacks[getEntityId()].hook(t, method);
				}
				/// Removes a listener method from listening for collisions
				/// @note Thread safe
				template<typename TargetT>
				void removeCollisionListener(TargetT* t, void (TargetT::*method)(Collision const&))
				{
					std::lock_guard<std::mutex> lock(ECSManager::getShared().getSystem<PhysicsSystem,PhysicsComponent>()->mCollisionMutex);
					ECSManager::getShared().getSystem<PhysicsSystem,PhysicsComponent>()->mCollisionCallbacks[getEntityId()].unHook(t, method);
				}
				/// Returns the rigid body in the given index.
				/// @param index The index of the Rigid body.
				/// @return The rigid body at the given index. 
				NewtonBody* getRigidBody(int index);
				/// Returns the PhysicsMaterial assigned to the rigid body in the given index.
				/// @param index The index of the Rigid body.
				/// @return The PhysicsMaterial assigned to the rigid body at the given index. 
				PhysicsMaterial& getMaterial(int index);
				/// Move constructor to move a RenderComponent.
				PhysicsComponent(PhysicsComponent && rhs);
				/// Move assignment operator to move a RenderComponent.
				PhysicsComponent& operator=(PhysicsComponent && rhs);
				~PhysicsComponent();
			private:
				
				std::vector<NewtonBody*>		mRigidBodies;		// The rigid bodies
				std::vector<NewtonCollision*>	mCollisionShapes;	// The collision shapes of the rigid bodies
				NewtonWorld*					mDynamicsWorld;		// The global physics world
				glm::vec4						mLocalGravity;		// The local gravity shared between all rigid bodies
		};
	};
};