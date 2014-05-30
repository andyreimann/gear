#pragma once
#include <G2Core/NonCopyable.h>

#include <glm/glm.hpp>
#include <utility>

namespace G2 
{
	namespace Physics
	{
		/// This struct 
		/// @created:	2014/05/30
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class Collision : NonCopyable
		{
			public:
				/// This constructs a new Collision.
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
				
				int entityIdA;
				int entityIdB;
				glm::vec3 position;
				glm::vec3 normal;
				float normalSpeed;
		};
	};
};