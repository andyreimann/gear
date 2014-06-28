// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "BaseSystem.h"
#include "Event.h"

#include <vector>
#include <string>

namespace G2
{
	/** This singleton class is the central management of all BaseSystems of the ECS.
	 * It automatically creates the Systems needed at runtime.
	 * @created:	2014/01/30
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class ECSManager
	{
	
		public:
			/** This function will return the instance of a requested system class.
			 * @return a reference to the requested System class.
			 */
			template<class SYSTEM,class COMPONENT>
			SYSTEM* getSystem() 
			{
				for(size_t i = 0; i < mRegisteredSystems.size(); ++i) 
				{
					// we have to use a dynamic_cast here because
					if(SYSTEM* system = dynamic_cast<SYSTEM*>(mRegisteredSystems[i]))
						return system; // downcast successful
				}
				// register new
				SYSTEM* system = new SYSTEM();
				if(system->runsOnMainThread())
				{
					mMainThreadUpdateEvent.hook(static_cast<BaseSystem<SYSTEM,COMPONENT>*>(system),&BaseSystem<SYSTEM,COMPONENT>::run);
				}
				if(system->runsOnSideThread())
				{
					mSideThreadUpdateEvent.hook(static_cast<BaseSystem<SYSTEM,COMPONENT>*>(system),&BaseSystem<SYSTEM,COMPONENT>::run);
				}
				
				mRegisteredSystems.push_back(system);
				return system;
			}
			
			/** This function ensures that the given systems
			 * are invoked in a phase in the order they are given in the templates.
			 * @return True if the ordering was changed, false if not.
			 */
			template<class SYSTEM1,class COMPONENT1,class SYSTEM2,class COMPONENT2>
			bool updateInvokeOrder()
			{
				size_t idx1 = 0;
				size_t idx2 = 0;
				SYSTEM1* system1 = nullptr;
				SYSTEM2* system2 = nullptr; 
				for(size_t i = 0; i < mRegisteredSystems.size(); ++i) 
				{
					if(system1 == nullptr && dynamic_cast<SYSTEM1*>(mRegisteredSystems[i]))
					{
						idx1 = i;
						system1 = dynamic_cast<SYSTEM1*>(mRegisteredSystems[i]);
					}
					else if(system2 == nullptr && dynamic_cast<SYSTEM2*>(mRegisteredSystems[i]))
					{
						idx2 = i;
						system2 = dynamic_cast<SYSTEM2*>(mRegisteredSystems[i]);
					}
				}
				if(system1 != nullptr && system2 != nullptr)
				{
					if(idx1 > idx2)
					{
						bool changed = false;
						std::swap(mRegisteredSystems[idx1],mRegisteredSystems[idx2]);
						if(system1->runsOnMainThread() && system2->runsOnMainThread())
						{
							// unhook both
							mMainThreadUpdateEvent.unHookAll(static_cast<BaseSystem<SYSTEM1,COMPONENT1>*>(system1));
							mMainThreadUpdateEvent.unHookAll(static_cast<BaseSystem<SYSTEM2,COMPONENT2>*>(system2));
							// hook in right order
							mMainThreadUpdateEvent.hook(static_cast<BaseSystem<SYSTEM1,COMPONENT1>*>(system1),&BaseSystem<SYSTEM1,COMPONENT1>::run);
							mMainThreadUpdateEvent.hook(static_cast<BaseSystem<SYSTEM2,COMPONENT2>*>(system2),&BaseSystem<SYSTEM2,COMPONENT2>::run);
							changed = true;
						}
						//if(system1->runsOnSideThread() && system2->runsOnSideThread())
						//{
						//	// unhook both
						//	mSideThreadUpdateEvent.unHook(static_cast<BaseSystem<SYSTEM1,COMPONENT1>*>(system1),&BaseSystem<SYSTEM1,COMPONENT1>::run);
						//	mSideThreadUpdateEvent.unHook(static_cast<BaseSystem<SYSTEM2,COMPONENT2>*>(system2),&BaseSystem<SYSTEM2,COMPONENT2>::run);
						//	// hook in right order
						//	mSideThreadUpdateEvent.hook(static_cast<BaseSystem<SYSTEM1,COMPONENT1>*>(system1),&BaseSystem<SYSTEM1,COMPONENT1>::run);
						//	mSideThreadUpdateEvent.hook(static_cast<BaseSystem<SYSTEM2,COMPONENT2>*>(system2),&BaseSystem<SYSTEM2,COMPONENT2>::run);
						//	changed = true;
						//}
						return changed;
					}
				}
				return false;
			}

			/** This function will run all main thread phases for all managed systems.
			 * @param frameInfo The current frame info object.
			 * 
			 */
			COREDLL_API void runMainThread(FrameInfo const& frameInfo);
			/** This function will run all side thread phases for all managed systems.
			 * @param frameInfo The current frame info object.
			 * 
			 */
			COREDLL_API void runSideThread(FrameInfo const& frameInfo);

			COREDLL_API void deleteComponentsForEntity(unsigned int entityId);
			/** Get a reference to one single instance.
			 * @return A reference of one single instance.
			 */
			COREDLL_API static ECSManager& getShared();
			/** Destroys the one single instance.
			 */
			COREDLL_API static void destroy();

			/** Sets the phases to run for the main thread 
			 * Make sure the default phase "preUpdate","update","postUpdate" and "render" are provided in this order.
			 * In between you can add whatever you like.
			 * @param phases a pointer to the phases to use.
			 * @param numPhases The number of phases contained in 'phases'.
			 * @return True if the ECS now uses the given phases, false if not.
			 */
			COREDLL_API bool setMainThreadPhases(std::string* phases, unsigned int numPhases);
			/** This function will return the phases to run in the main thread. 
			 * @return The phases to run in the main thread.
			 */
			COREDLL_API std::vector<std::string> const& getMainThreadPhases() const { return mMainThreadPhases; }

			/** Sets the phases to run for the side thread 
			 * Make sure the default phase "updateSideThread" is provided.
			 * @param phases a pointer to the phases to use.
			 * @param numPhases The number of phases contained in 'phases'.
			 * @return True if the ECS now uses the given phases, false if not.
			 */
			COREDLL_API bool setSideThreadPhases(std::string* phases, unsigned int numPhases);
			/** This function will return the phases to run in the main thread. 
			 * @return The phases to run in the main thread.
			 */
			COREDLL_API std::vector<std::string> const& getSideThreadPhases() const { return mSideThreadPhases; }

		private:
			// don't allow instances
			COREDLL_API ECSManager();
			COREDLL_API ECSManager(ECSManager const&) {}
			COREDLL_API ECSManager& operator=(ECSManager const&) { return *this; }
			COREDLL_API ~ECSManager();

			COREDLL_API static ECSManager*				mInstance_;				// The one single instance
			std::vector<BaseSystemWrapper*>				mRegisteredSystems;		// The registered Systems
			Event<std::string const&,FrameInfo const&>	mMainThreadUpdateEvent;	// The event used for the updates for all managed Systems running in the main thread
			Event<std::string const&,FrameInfo const&>	mSideThreadUpdateEvent;	// The event used for the updates for all managed Systems running in the side thread
			std::vector<std::string>					mMainThreadPhases;		// The phases to run in the main thread
			std::vector<std::string>					mSideThreadPhases;		// The phases to run in the side thread

	};
};