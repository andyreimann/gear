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
				mPhaseUpdateEvent.hook(static_cast<BaseSystem<SYSTEM,COMPONENT>*>(system),&BaseSystem<SYSTEM,COMPONENT>::run);
				mRegisteredSystems.push_back(system);
				return system;
			}
			/** This function will start a run of a given phase for all managed systems.
			 * @param name The name of the phase to run.
			 * 
			 */
			void runPhaseOnSystems(std::string const& name, FrameInfo const& frameInfo) 
			{
				mPhaseUpdateEvent(name, frameInfo);
			}

			void deleteComponentsForEntity(unsigned int entityId) 
			{
				for(size_t i = 0; i < mRegisteredSystems.size(); ++i) 
				{
					mRegisteredSystems[i]->deleteComponentsForEntity(entityId);
				}
			}
			/** Get a reference to one single instance.
			 * @return A reference of one single instance.
			 */
			static ECSManager& getShared();
			/** Destroys the one single instance.
			 */
			static void destroy();

		private:
			// don't allow instances
			ECSManager() {}
			ECSManager(ECSManager const&) {}
			ECSManager& operator=(ECSManager const&) { return *this; }
			~ECSManager();

			static ECSManager*				mInstance_;			// The one single instance
			std::vector<BaseSystemWrapper*>	mRegisteredSystems; // The registered Systems
			Event<std::string const&,FrameInfo const&>		mPhaseUpdateEvent;	// The event used for the updates for all managed Systems
	};
};