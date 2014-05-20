#pragma once
#include "PhysicsSystem.h"

#include <G2Core/BaseComponent.h>

#include <glm/glm.hpp>

class btCollisionShape;
struct btDefaultMotionState;
class btRigidBody;

namespace G2 
{
	namespace Physics
	{
		/// This class defines...
		/// @created:	2014/05/07
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class PhysicsComponent : public BaseComponent<PhysicsSystem> 
		{
			friend class PhysicsSystem;
			public:
				/// This constructs a new PhysicsComponent 
				/// with a collision shape of a Sphere
				PhysicsComponent(float radius);
				/// This constructs a new PhysicsComponent 
				/// with a collision shape of a Plane
				PhysicsComponent(glm::vec3 const& normal, float constant);
				/// Move constructor to move a RenderComponent.
				PhysicsComponent(PhysicsComponent && rhs);
				/// Move assignment operator to move a RenderComponent.
				PhysicsComponent& operator=(PhysicsComponent && rhs);

				~PhysicsComponent();
			protected:
			private:

				bool					mIsSphere;
				btCollisionShape*		mCollisionShape;	// The collision shape -> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Shapes
				btDefaultMotionState*	mMotionState;		// The motion state -> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/MotionStates
				btRigidBody*			mRigidBody;
		};
	};
};