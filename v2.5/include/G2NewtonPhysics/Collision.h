#pragma once
#include <G2Core/NonCopyable.h>

#include <glm/glm.hpp>
#include <utility>

namespace G2 
{
	namespace Physics
	{
		/// This struct holds all informations describing a collision event.
		/// @created:	2014/05/30
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class Collision : NonCopyable
		{
			public:
				/// This constructs a new Collision.
				/// @param entityIdA The first entity ID of the colliding pair of entities
				/// @param entityIdB The second entity ID of the colliding pair of entities
				/// @param position The position of the collision.
				/// @param normal The normal vector of the collision.
				/// @param normalSpeed The speed into the direction of the normal vector of the collision.
				Collision(int entityIdA, int entityIdB, glm::vec3 const& position, glm::vec3 const& normal, float normalSpeed)
					: entityIdA(entityIdA),
					entityIdB(entityIdB),
					position(position),
					normal(normal),
					normalSpeed(normalSpeed)
				{}
				/// Move constructor.
				Collision(Collision && rhs)
				{
					// eliminates redundant code
					*this = std::move(rhs); // rvalue property is kept with std::move!
				}
				/// Basic move operator.
				Collision& operator=(Collision && rhs)
				{
					entityIdA = rhs.entityIdA;
					entityIdB = rhs.entityIdB;
					position = std::move(rhs.position);
					normal = std::move(rhs.normal);
					normalSpeed = rhs.normalSpeed;
	
					rhs.entityIdA = -1;
					rhs.entityIdB = -1;
	
					return *this;
				}
				
				int entityIdA;		// The first entity ID of the colliding pair of entities
				int entityIdB;		// The second entity ID of the colliding pair of entities
				glm::vec3 position;	// The position of the collision.
				glm::vec3 normal;	// The normal vector of the collision.
				float normalSpeed;	// The speed into the direction of the normal vector of the collision.
		};
	};
};