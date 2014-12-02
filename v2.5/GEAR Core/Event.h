// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"

#include <vector>

namespace G2
{	
	template <typename ... ARGS>
	class HandlerBase
	{
	public:
		virtual void Process(ARGS ... args) = 0;
		virtual bool operator==(HandlerBase<ARGS ...>& rhs) = 0;
		virtual bool isSameObject(HandlerBase<ARGS ...>& rhs) = 0;
	};

	template <typename TargetT, typename ... ARGS>
	class Handler : public HandlerBase<ARGS ...> 
	{
		private:
			typedef void (TargetT::*method_t)(ARGS ...);
			TargetT* mObject;

		public:
			method_t mMethod;
			Handler(TargetT* object, method_t method)
				: mObject(object), mMethod(method) 
			{ }

			virtual void Process(ARGS ... args) 
			{
				(mObject->*mMethod)(args ...);
			}

			virtual bool isSameObject(HandlerBase<ARGS ...>& rhs) 
			{
				Handler<TargetT,ARGS ...>* handler = dynamic_cast<Handler<TargetT,ARGS ...>*>(&rhs);

				return handler != nullptr && mObject == handler->mObject;
			}

			virtual bool operator==(HandlerBase<ARGS ...>& rhs)
			{
				Handler<TargetT,ARGS ...>* handler = dynamic_cast<Handler<TargetT,ARGS ...>*>(&rhs);
				// Important note: For classes with templates, this dynamic cast can fail -> all share same base class
				return handler != nullptr && mObject == handler->mObject && mMethod == handler->mMethod;
			}
	};

	template <typename ... ARGS>
	class EventReceiver
	{ };

	/** This generic function implements an universal Event with variadic templates, on which listeners can connect itself to.
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	template <typename ... ARGS>
	class Event
	{
		public:
			/** Default constructor of the Event.
			 */
			Event() {}

			~Event() 
			{
				typename std::vector<HandlerBase<ARGS ...>*>::iterator it = mEventListeners_.begin();
				while (it != mEventListeners_.end())
				{
					delete *it;
					++it;
				}
			}
			/** This function will invoke the Event and call all registered listeners functions
			 * with the given arguments.
			 * @param args The list of arguments.
			 */
			void operator()(ARGS ... args) 
			{
				for(int i = 0; i < mEventListeners_.size(); ++i)
				{
					mEventListeners_[i]->Process(args ...);
				}
			}

			/** This function will hook a given member function pointer of an object to the event.
			* @param t The object on which you want a member function pointer to be registered.
			* @param method The member function pointer to register.
			*/
			template<typename TargetT>
			void hook(TargetT* t, void (TargetT::*method)(ARGS ... args))
			{
				mEventListeners_.push_back(new Handler<TargetT, ARGS ...>(t, method));
			}

			/** This function will unhook a given member function pointer of an object from the event.
			* @param t The object on which you want a member function pointer to be unhooked.
			* @param method The member function pointer to unhook.
			*/
			template<typename TargetT>
			void unHook(TargetT* t, void (TargetT::*method)(ARGS ... args)) 
			{
				typename std::vector<HandlerBase<ARGS ...>*>::iterator it = mEventListeners_.begin();
				while(it != mEventListeners_.end())
				{
					Handler<TargetT, ARGS ...> handler(t, nullptr);
					if((**it) == handler) 
					{
						delete *it;
						it = mEventListeners_.erase(it);
					}
					++it;
				}
			}

			/** This function will unhook all registered member function pointers of an object from the event.
			* @param t The object on which you want all member function pointers to be unhooked.
			*/
			template<typename TargetT>
			void unHookAll(TargetT* t)
			{
				typename std::vector<HandlerBase<ARGS ...>*>::iterator it = mEventListeners_.begin();
				while(it != mEventListeners_.end())
				{
					Handler<TargetT, ARGS ...> handler(t, nullptr);
					if((**it).isSameObject(handler)) 
					{
						delete *it;
						it = mEventListeners_.erase(it);
						continue;
					}
					++it;
				}
			}

		private:
			std::vector<HandlerBase<ARGS ...>*> mEventListeners_;	// the vector of all registered listeners
	};
};