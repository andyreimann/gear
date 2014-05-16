// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Event.h"
#include "Defines.h"
#include "FrameInfo.h"

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>

namespace G2 
{
	class BaseSystemWrapper 
	{
		public:
			virtual void deleteComponentsForEntity(unsigned int entityId) = 0;
			virtual ~BaseSystemWrapper() {}
	};
	/** This class defines...
	 * @created	2014/01/29
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	template<class DERIVED_SYSTEM,class COMPONENT>
	class BaseSystem : public BaseSystemWrapper 
	{
		public:
			/** This function is called when a run phase is called on the System.
			 * @param name The name of the phase to run.
			 * 
			 * @note The derived system has to provide a non-virtual function called 
			 * runPhase(std::string const& name), which is called by this function.
			 */
			void run(std::string const& name, FrameInfo const& frameInfo) 
			{
				((DERIVED_SYSTEM*)this)->runPhase(name, frameInfo);
			}
			/** This generic function will create a Component
			 * with the given Entity-ID.
			 * @param entityId The id of the Entity.
			 * @return The created Component.
			 */
			template<typename ... ARGS>
			COMPONENT* create(unsigned int entityId, ARGS ... args)
			{
				COMPONENT component(args ...);
				component.mEntityId = entityId;
				
#ifndef ECS_USE_VECTOR
				components.insert(std::make_pair<unsigned int,COMPONENT>(entityId,component));
#else
				components.push_back(std::move(component)); // use move semantic with fallback to copy semantic
				entityIdToVectorIndex.insert(std::make_pair(entityId,(unsigned int)components.size()-1));
#endif
				return get(entityId);
			}
			/** This generic function will return a pointer to the Component
			 * with the given Entity-ID.
			 * @param entityId The id of the Entity.
			 * @return The found Component or nullptr if no Component is registered.
			 */
			COMPONENT* get(unsigned int entityId) 
			{

#ifndef ECS_USE_VECTOR
				auto it = components.find(entityId);
				if(it != components.end()) 
				{
					return &(it->second);
				}
#else
				// nearly same work, but components are now linear in memory!
				auto it = entityIdToVectorIndex.find(entityId);
				if(it != entityIdToVectorIndex.end()) 
				{
					return &components[it->second];
				}
#endif
				return nullptr;
			}
			/** This function removes any registered component for the given entityId.
			 * It is automatically called 
			 */
			virtual void deleteComponentsForEntity(unsigned int entityId) override sealed 
			{
				// generic call used when deleting an entity
				remove(entityId);
			}
			/** This generic function will remove the Component
			 * with the given Entity-ID.
			 * @param entityId The id of the Entity.
			 */
			void remove(unsigned int entityId) 
			{
#ifndef ECS_USE_VECTOR
				components.erase(entityId);
#else
				auto it = entityIdToVectorIndex.find(entityId);
				if(it != entityIdToVectorIndex.end()) 
				{
					unsigned int vectorIndexTmp = it->second;
					// swap vector indices
					entityIdToVectorIndex.find(components.back().mEntityId)->second = it->second;
					// swap components
					std::swap(components[vectorIndexTmp], components.back());
					// drop last component
					components.pop_back();
					// drop linkage for component
					entityIdToVectorIndex.erase(entityId);
				}
#endif
			}
			/** This function allows you to reserve size for components managed 
			 * by the derived system you call the function on.
			 * This prevents costly reallocations at runtime.
			 * @param size The amount of components you want to reserve memory for.
			 */
			void reserveSize(unsigned int size)
			{
				if(size > components.size())
				{
					// only allow upscale
					components.resize(size);
				}
			}

		protected:

#ifndef ECS_USE_VECTOR
			std::unordered_map<unsigned int,COMPONENT>		components; 
#else
			std::unordered_map<unsigned int,unsigned int>	entityIdToVectorIndex;
			std::vector<COMPONENT>							components; // components are sequentially in memory
#endif
	};
};