#pragma once
#include "PhysicsSystem.h"
#include "CollisionShapeDescriptor.h"
#include "RigidBodyDescriptor.h"

#include <G2Core/BaseComponent.h>

#include <btBulletCollisionCommon.h>
#include <glm/ext.hpp>


#define GEAR_GHOST_COLLISIONS

class btCollisionShape;
struct btDefaultMotionState;
class btRigidBody;
class btPairCachingGhostObject;

namespace G2 
{
	namespace Physics
	{
		/// This class defines...
		/// @created:	2014/05/07
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class PhysicsComponent : public BaseComponent<PhysicsSystem>, btCollisionWorld::ContactResultCallback
		{
			friend class PhysicsSystem;
			public:
				/// This constructs a new PhysicsComponent 
				/// with a collision shape of a Sphere
				//PhysicsComponent(float radius);
				/// This constructs a new PhysicsComponent 
				/// with the given collision shape descriptor
				/// @param collisionShapeDescriptor The descriptor for the collision shape
				/// @param rigidBodyDescriptor The descriptor for the rigid body
				/// @param position The initial position.
				/// @param orientation The initial orientation.
				PhysicsComponent(CollisionShapeDescriptor const& collisionShapeDescriptor, RigidBodyDescriptor const& rigidBodyDescriptor, glm::vec3 const& position = glm::vec3(), glm::quat const& orientation = glm::quat());
				/// This constructs a new PhysicsComponent 
				/// with a collision shape of a Plane
				//PhysicsComponent(glm::vec3 const& normal, float constant);
				/// Move constructor to move a RenderComponent.
				PhysicsComponent(PhysicsComponent && rhs);
				/// Move assignment operator to move a RenderComponent.
				PhysicsComponent& operator=(PhysicsComponent && rhs);
				/// Returns if the PhysicsComponent is a movable one or not.
				bool isMovable() const;


				PhysicsComponent& trackExactCollisions(bool mode);
				PhysicsComponent& trackAABBCollisions(bool mode);
				
				/// This function will return the TrackExactCollisions. 
				/// @return The TrackExactCollisions.
				bool getTrackExactCollisions() const { return mTrackExactCollisions; }



				~PhysicsComponent();
			protected:
			private:

				void _cleanupAABBTracking();

				void _updateGhostObjectCollisions(btDynamicsWorld* world);

				/// Collision callback for bullet
				virtual	btScalar addSingleResult(btManifoldPoint& cp,btCollisionObjectWrapper const* colObj0Wrap,int partId0,int index0, btCollisionObjectWrapper const* colObj1Wrap,int partId1,int index1);
				
				bool					mTrackExactCollisions;
				bool					mTrackAABBCollisions;
				btCollisionShape*		mCollisionShape;	// The collision shape -> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Shapes
				btDefaultMotionState*	mMotionState;		// The motion state -> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/MotionStates
				btRigidBody*			mRigidBody;

				btPairCachingGhostObject* mGhostObject;		// Ghost object to retrieve collisions if needed
		};
	};
};