#pragma once
#include <G2Core/Entity.h>

class GameObject : public G2::Entity
{
	public:
	
		GameObject() {}

		GameObject(GameObject && rhs) 
		{
			*this = std::move(rhs); // rvalue property is kept with std::move!
		}

		GameObject& operator=(GameObject && rhs) 
		{
			return static_cast<GameObject&>(G2::Entity::operator=(std::move(rhs)));
		}

};

