// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once

#include <vector>

namespace G2
{
	template <typename param1, typename param2=void, typename param3=void, typename param4=void>
	class HandlerBase 
	{
	public:
		virtual void Process(param1 parameter1, param2 parameter2, param3 parameter3, param4 parameter4) = 0;
		virtual bool operator==(HandlerBase<param1,param2,param3,param4>& rhs) = 0;
	};
	
	template <typename param1, typename param2, typename param3>
	class HandlerBase<param1,param2,param3,void>
	{
	public:
		virtual void Process(param1 parameter1, param2 parameter2, param3 parameter3) = 0;
		virtual bool operator==(HandlerBase<param1,param2,param3>& rhs) = 0;
	};
	
	template <typename param1, typename param2>
	class HandlerBase<param1,param2,void,void> 
	{
	public:
		virtual void Process(param1 parameter1, param2 parameter2) = 0;
		virtual bool operator==(HandlerBase<param1,param2>& rhs) = 0;
	};
	
	template <typename param1>
	class HandlerBase<param1,void,void,void> 
	{
	public:
		virtual void Process(param1 parameter1) = 0;
		virtual bool operator==(HandlerBase<param1>& rhs) = 0;
	};

	template <typename TargetT, typename param1, typename param2=void, typename param3=void, typename param4=void>
	class Handler : public HandlerBase<param1,param2,param3,param4> 
	{
		private:
			typedef void (TargetT::*method_t)(param1,param2,param3,param4);
			TargetT* mObject;

		public:
			method_t mMethod;
			Handler(TargetT* object, method_t method)
				: mObject(object), mMethod(method)
			{ }

			virtual void Process(param1 parameter1, param2 parameter2, param3 parameter3, param4 parameter4)
			{
				(mObject->*mMethod)(parameter1,parameter2, parameter3, parameter4);
			}

			virtual bool operator==(HandlerBase<param1,param2,param3,param4>& rhs)
			{
				Handler<TargetT,param1,param2,param3,param4>* handler = dynamic_cast<Handler<TargetT,param1,param2,param3,param4>*>(&rhs);
				return mObject == handler->mObject && mMethod == handler->mMethod;
			}
	};

	template <typename TargetT, typename param1, typename param2, typename param3>
	class Handler<TargetT, param1, param2, param3, void> : public HandlerBase<param1,param2,param3>
	{
		private:
			typedef void (TargetT::*method_t)(param1, param2, param3);
			TargetT* mObject;

		public:
			method_t mMethod;
			Handler(TargetT* object, method_t method)
				: mObject(object), mMethod(method) 
			{ }

			virtual void Process(param1 parameter1, param2 parameter2, param3 parameter3)
			{
				(mObject->*mMethod)(parameter1, parameter2, parameter3);
			}

			virtual bool operator==(HandlerBase<param1,param2,param3>& rhs) 
			{
				Handler<TargetT,param1,param2,param3>* handler = dynamic_cast<Handler<TargetT,param1,param2,param3>*>(&rhs);
				return mObject == handler->mObject && mMethod == handler->mMethod;
			}
	};

	template <typename TargetT, typename param1, typename param2>
	class Handler<TargetT, param1, param2, void, void> : public HandlerBase<param1,param2>
	{
		private:
			typedef void (TargetT::*method_t)(param1,param2);
			TargetT* mObject;

		public:
			method_t mMethod;
			Handler(TargetT* object, method_t method)
				: mObject(object), mMethod(method)
			{ }

			virtual void Process(param1 parameter1, param2 parameter2) 
			{
				(mObject->*mMethod)(parameter1, parameter2);
			}

			virtual bool operator==(HandlerBase<param1,param2>& rhs)
			{
				Handler<TargetT,param1,param2>* handler = dynamic_cast<Handler<TargetT,param1,param2>*>(&rhs);
				return mObject == handler->mObject && mMethod == handler->mMethod;
			}
	};

	template <typename TargetT, typename param1>
	class Handler<TargetT, param1, void, void, void> : public HandlerBase<param1> 
	{
		private:
			typedef void (TargetT::*method_t)(param1);
			TargetT* mObject;

		public:
			method_t mMethod;
			Handler(TargetT* object, method_t method)
				: mObject(object), mMethod(method) 
			{ }

			virtual void Process(param1 parameter1) 
			{
				(mObject->*mMethod)(parameter1);
			}

			virtual bool operator==(HandlerBase<param1>& rhs)
			{
				Handler<TargetT,param1>* handler = dynamic_cast<Handler<TargetT,param1>*>(&rhs);
				return mObject == handler->mObject && mMethod == handler->mMethod;
			}
	};
	
	template <typename param1, typename param2=void, typename param3=void, typename param4=void>
	class EventReceiver 
	{ };
	
	template <typename param1, typename param2, typename param3>
	class EventReceiver<param1, param2, param3, void>
	{ };
	
	template <typename param1, typename param2>
	class EventReceiver<param1, param2, void, void>
	{ };

	template <typename param1>
	class EventReceiver<param1, void, void, void>
	{ };

	template <typename param1, typename param2=void, typename param3=void, typename param4=void>
	class Event 
	{
		public:

			void operator()(param1 parameter1, param2 parameter2, param3 parameter3, param4 parameter4) 
			{
				for(int i = 0; i < mEventListeners_.size(); ++i) 
				{
					mEventListeners_[i]->Process(parameter1,parameter2,parameter3,parameter4);
				}
			}
			
			template<typename TargetT>
			void hook(TargetT* t, void (TargetT::*method)(param1, param2, param3, param4)) 
			{
				mEventListeners_.push_back(new Handler<TargetT, param1, param2, param3, param4>(t, method));
			}
			
			template<typename TargetT>
			void unHook(TargetT* t, void (TargetT::*method)(param1, param2, param3, param4)) 
			{
				typename std::vector<HandlerBase<param1,param2,param3,param4>*>::iterator it = mEventListeners_.begin();
				while(it != mEventListeners_.end()) 
				{
					Handler<TargetT, param1, param2, param3, param4> handler(t, method);
					if((**it) == handler) 
					{
						it = mEventListeners_.erase(it);
						return;
					}
					++it;
				}
			}

		private:
			std::vector<HandlerBase<param1,param2,param3,param4>*> mEventListeners_;
	};

	template <typename param1, typename param2, typename param3>
	class Event<param1,param2, param3, void> 
	{
		public:

			void operator()(param1 parameter1, param2 parameter2, param3 parameter3) 
			{
				for(int i = 0; i < mEventListeners_.size(); ++i) 
				{
					mEventListeners_[i]->Process(parameter1,parameter2,parameter3);
				}
			}
			
			template<typename TargetT>
			void hook(TargetT* t, void (TargetT::*method)(param1, param2, param3)) 
			{
				mEventListeners_.push_back(new Handler<TargetT, param1, param2, param3>(t, method));
			}
			
			template<typename TargetT>
			void unHook(TargetT* t, void (TargetT::*method)(param1, param2, param3)) 
			{
				typename std::vector<HandlerBase<param1,param2,param3>*>::iterator it = mEventListeners_.begin();
				while(it != mEventListeners_.end()) 
				{
					Handler<TargetT, param1, param2, param3> handler(t, method);
					if((**it) == handler)
					{
						it = mEventListeners_.erase(it);
						return;
					}
					++it;
				}
			}

		private:
			std::vector<HandlerBase<param1,param2,param3>*> mEventListeners_;
	};
	
	template <typename param1, typename param2>
	class Event<param1,param2, void, void>
	{
		public:

			void operator()(param1 parameter1, param2 parameter2)
			{
				for(int i = 0; i < mEventListeners_.size(); ++i) 
				{
					mEventListeners_[i]->Process(parameter1,parameter2);
				}
			}
			
			template<typename TargetT>
			void hook(TargetT* t, void (TargetT::*method)(param1, param2))
			{
				mEventListeners_.push_back(new Handler<TargetT, param1, param2>(t, method));
			}
			
			template<typename TargetT>
			void unHook(TargetT* t, void (TargetT::*method)(param1, param2))
			{
				typename std::vector<HandlerBase<param1,param2>*>::iterator it = mEventListeners_.begin();
				while(it != mEventListeners_.end())
				{
					Handler<TargetT, param1, param2> handler(t, method);
					if((**it) == handler) 
					{
						it = mEventListeners_.erase(it);
						return;
					}
					++it;
				}
			}

		private:
			std::vector<HandlerBase<param1,param2>*> mEventListeners_;
	};

	template <typename param1>
	class Event<param1,void, void, void>
	{
		public:

			Event() {}
			
			void operator()(param1 parameter1) 
			{
				for(int i = 0; i < mEventListeners_.size(); ++i)
				{
					mEventListeners_[i]->Process(parameter1);
				}
			}
			
			template<typename TargetT>
			void hook(TargetT* t, void (TargetT::*method)(param1))
			{
				mEventListeners_.push_back(new Handler<TargetT, param1>(t, method));
			}
			
			template<typename TargetT>
			void unHook(TargetT* t, void (TargetT::*method)(param1)) 
			{
				typename std::vector<HandlerBase<param1>*>::iterator it = mEventListeners_.begin();
				while(it != mEventListeners_.end())
				{
					Handler<TargetT, param1> handler(t, method);
					if((**it) == handler) 
					{
						it = mEventListeners_.erase(it);
						return;
					}
					++it;
				}
			}

		private:
			std::vector<HandlerBase<param1>*> mEventListeners_;
	};
};