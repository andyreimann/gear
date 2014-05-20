// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "FrameInfo.h"

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <mutex>

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
			 * @note This operation will block access to the components from other threads.
			 */
			void run(std::string const& name, FrameInfo const& frameInfo) 
			{
				std::lock_guard<std::mutex> lock(componentsMutex);
				((DERIVED_SYSTEM*)this)->runPhase(name, frameInfo);
			}
			/** This generic function will create a Component
			 * with the given Entity-ID.
			 * @param entityId The id of the Entity.
			 * @return The created Component.
			 * @note This operation will block access to the components from other threads.
			 */
			template<typename ... ARGS>
			COMPONENT* create(unsigned int entityId, ARGS ... args)
			{
				std::lock_guard<std::mutex> lock(componentsMutex);
				COMPONENT component(args ...);
				component.mEntityId = entityId;
				components.push_back(std::move(component)); // use move semantic with fallback to copy semantic
				entityIdToVectorIndex.insert(std::make_pair(entityId,(unsigned int)components.size()-1));
				return get(entityId);
			}
			/** This generic function will return a pointer to the Component
			 * with the given Entity-ID.
			 * @param entityId The id of the Entity.
			 * @return The found Component or nullptr if no Component is registered.
			 */
			COMPONENT* get(unsigned int entityId) 
			{
				// nearly same work, but components are now linear in memory!
				auto it = entityIdToVectorIndex.find(entityId);
				if(it != entityIdToVectorIndex.end()) 
				{
					return &components[it->second];
				}
				return nullptr;
			}
			/** This function removes any registered component for the given entityId.
			 * It is automatically called 
			 * @note This operation will block access to the components from other threads.
			 */
			virtual void deleteComponentsForEntity(unsigned int entityId) override sealed 
			{
				// generic call used when deleting an entity
				remove(entityId);
			}
			/** This generic function will remove the Component
			 * with the given Entity-ID.
			 * @param entityId The id of the Entity.
			 * @note This operation will block access to the components from other threads.
			 */
			void remove(unsigned int entityId) 
			{
				std::lock_guard<std::mutex> lock(componentsMutex);
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
			}
			/** This function allows you to reserve size for components managed 
			 * by the derived system you call the function on.
			 * This prevents costly reallocations at runtime.
			 * @param size The amount of components you want to reserve memory for.
			 * @note This operation will block access to the components from other threads.
			 */
			void reserveSize(unsigned int size)
			{
				std::lock_guard<std::mutex> lock(componentsMutex);
				if(size > components.size())
				{
					// only allow upscale
					components.resize(size);
				}
			}
			/// Returns true if the system runs in the main thread or false
			/// if it runs in the side thread.
			// @note A system can only run in one thread
			virtual bool runsOnMainThread() { return true; }
		protected:
			std::mutex										componentsMutex;
			std::unordered_map<unsigned int,unsigned int>	entityIdToVectorIndex;
			std::vector<COMPONENT>							components; // components are sequentially in memory
	};
};