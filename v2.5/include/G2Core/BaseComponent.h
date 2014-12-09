// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "ECSManager.h"

namespace G2 
{
	/** This class defines the BaseComponent of the Entity-Component-System.
	 * A Component is basically a container holding user defined data.
	 * @created:	2014/01/29
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	template<class SYSTEM>
	class BaseComponent 
	{
			template<class AnySystem,class AnyComponent> friend class BaseSystem;
		public:
			/** Constructor of the BaseComponent.
			 * @note After construction, the BaseComponent does not yet have the entity id set!
			 */
			BaseComponent() 
				: mEntityId(0)
			{
			}
			/** Move constructor of the BaseComponent.
			 * @note The given rhs BaseComponent will be invalid afterwards!
			 */
			BaseComponent(BaseComponent && rhs) 
				: mEntityId(0)
			{
				// eliminates redundant code
				*this = std::move(rhs); // rvalue property is kept with std::move!
			}
			/** Move assignment operator of the BaseComponent.
			 * @note The given rhs BaseComponent will be invalid afterwards!
			 */
			BaseComponent& operator=(BaseComponent && rhs) 
			{
				mEntityId = rhs.mEntityId;
	
				rhs.mEntityId = 0;
	
				return *this;
			}
			/** This function will return the entity id the component is attached to. 
			* @return The entity id.
			*/
			unsigned int getEntityId() const { return mEntityId; }
			/** This function will create a managed instance of the derived component.
			 * @param entityId The unique ID of the entity, the Component will be attached to.
			 * @note This function is only called by the Entity class!
			 */
			template<class COMPONENT>
			static COMPONENT* create(unsigned int entityId) 
			{
				// call the System of the Component to create a new one
				// return type of create should be BaseComponent*
				COMPONENT* component = static_cast<COMPONENT*>(getSystem<COMPONENT>()->create(entityId));
				return component;
			}
			/** This function will create a managed instance of the derived component.
			 * @param entityId The unique ID of the entity, the Component will be attached to.
			 * @note This function is only called by the Entity class!
			 */
			template<class COMPONENT,typename ... ARGS>
			static COMPONENT* create(unsigned int entityId, ARGS ... args) 
			{
				// call the System of the Component to create a new one
				// return type of create should be BaseComponent*
				COMPONENT* component = static_cast<COMPONENT*>(getSystem<COMPONENT>()->create<ARGS ...>(entityId, args ...));
				return component;
			}
			/** This function will remove a managed instance of the derived component.
			 * @param entityId The unique ID of the entity, the corresponding Component should be removed.
			 * @note This function is only called by the Entity class!
			 */
			template<class COMPONENT>
			static void remove(unsigned int entityId) 
			{
				getSystem<COMPONENT>()->remove(entityId);
			}
			/** This function will return the managed instance of the derived component with the given entity id.
			 * @param entityId The unique ID of the entity, to get the corresponding Component for.
			 * @note This function is only called by the Entity class!
			 */
			template<class COMPONENT>
			static COMPONENT* getComponentByEntityId(unsigned int entityId) 
			{
				return getSystem<COMPONENT>()->get(entityId);
			}

			virtual ~BaseComponent() {}

		protected:
			static SYSTEM* system; // The responsible system class of the BaseComponent

			/** This function will return the appropriate system 
			 * for the current component class.
			 * The component itself uses an internal caching to accelerate the 
			 * retrieval of the system, since the ECSManager itself has to perform
			 * at least one dynamic_cast to retrieve the system.
			 * As soon as the system is cached, the retrieval of the system is trivial.
			 */
			template<class COMPONENT>
			static SYSTEM* getSystem() 
			{
				if(system == nullptr) 
				{
					system = ECSManager::getShared().getSystem<SYSTEM,COMPONENT>();
				}
				return system;
			}


			unsigned int mEntityId; // The ID of the entity the Component is attached to
	};

	template<class SYSTEM>
	SYSTEM* BaseComponent<SYSTEM>::system = nullptr;
};