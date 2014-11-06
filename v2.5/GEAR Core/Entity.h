// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "NonCopyable.h"
#include "BaseComponent.h"
#include "BaseSystem.h"
#include <memory>
#include <vector>

namespace G2 
{
	/** This class defines a basic entity from the Entity-Component-System.
	 * The newly created Entity will have a unique ID assigned.
	 * The ID is only guaranteed to be unique, but not necessarily to be continuous.
	 * @note Entities are NonCopyable but movable. Make sure to correctly implement 
	 * the move ctor and move assignment operator in your derived class.
	 * @created:	2014/01/29
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class Entity : public NonCopyable 
	{
		public:
			/** Constructs a new Entity.
			 */
			COREDLL_API Entity();
			/** Move ctor.
			 */
			COREDLL_API Entity(Entity && rhs);
			/** Move assignment operator.
			 */
			COREDLL_API Entity& operator=(Entity && rhs);
			/** This function will return the unique ID of the Entity. 
			* @return The unique ID of the Entity.
			*/
			COREDLL_API unsigned int getId() const;

			/* This function will return a pointer to the requested Component or nullptr
			 * if no such Component is attached.
			 * @return A pointer to the requested Component or nullptr if no such Component is attached.
			 * @complexity constant
			 */
			template<class T>
			T* getComponent() 
			{
				return T::getComponentByEntityId<T>(mId);
			}
			/* This function will return a pointer to the requested Component or nullptr
			 * if no such Component is attached.
			 * @return A pointer to the requested Component or nullptr if no such Component is attached.
			 * @complexity constant
			 */
			template<class T>
			T const* getComponent() const 
			{
				return T::getComponentByEntityId<T>(mId);
			}
			/* This function will instantiate a Component (only if not already attached) and return a pointer to the requested Component.
			 * @note One specific component can only be attached once!
			 * @return A pointer to the requested Component.
			 * @complexity constant
			 */
			template<class T,typename ... ARGS>
			T* addComponent(ARGS ... args) 
			{
				T* componentPtr = getComponent<T>();
				if(nullptr != componentPtr) 
				{
					return componentPtr;
				}
				T* component = T::create<T,ARGS ... >(mId,args ...);
				return component;
			}
			/* This function will check the existence of a requested Component.
			 * @return True if the requested Component is attached, false if not.
			 * @complexity constant
			 */
			template<class T>
			bool hasComponent() 
			{
				return nullptr != getComponent<T>();
			}
			/** This function will remove the given Component from the Entity.
			 * @complexity constant
			 */
			template<class T>
			void removeComponent()
			{
				T::remove<T>(mId);
			}
			/** Deletes the Entity.
			 * @note Deleting an Entity will automatically delete all attached Components of any System.
			 */
			COREDLL_API ~Entity();

			COREDLL_API static unsigned int UNINITIALIZED_ENTITY_ID;

		private:

			Entity& operator=(Entity const& rhs);

			unsigned int	mId; // The unique ID of the entity
			
			static unsigned int LAST_ENTITY_ID;
			
	};
};