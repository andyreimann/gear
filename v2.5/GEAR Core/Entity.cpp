// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Entity.h"

using namespace G2;

unsigned int Entity::UNINITIALIZED_ENTITY_ID = 0;
unsigned int Entity::LAST_ENTITY_ID = 1;

Entity::Entity() 
	: mId(++LAST_ENTITY_ID) 
{
	// register entity?
	// or no: entities can hold components
	//		these components will be connected with the ID! :)
}

Entity::Entity(Entity && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

Entity& Entity::operator=(Entity && rhs)
{
	// 1. Stage: delete maybe allocated resources on target type
	if(mId != UNINITIALIZED_ENTITY_ID)
	{
		ECSManager::getShared().deleteComponentsForEntity(mId);
	}
	// 2. Stage: transfer data from src to target
	mId = rhs.mId;
	// 3. Stage: modify src to a well defined state
	rhs.mId = UNINITIALIZED_ENTITY_ID;

	return *this;
}

unsigned int
G2::Entity::getId() const 
{
	return mId;
}


G2::Entity::~Entity() 
{
	ECSManager::getShared().deleteComponentsForEntity(mId);
}