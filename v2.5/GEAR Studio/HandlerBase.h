#pragma once
#include "Defines.h"

namespace G2S
{
	/** This class is the base class of a Handler, which handles basic transformations of the selected ManagedEntity.
	 * @created:	2014/01/13
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class HandlerBase
	{
		public:
			/** This constructs a new HandlerBase.
			 * @param responsibleMode The HandleMode this HandlerBase is responsible for.
			 */
			HandlerBase(G2S::HandleMode::Name responsibleMode);

			~HandlerBase();

		protected:
			/** This function can be overridden by any sub class to receive
			 * calles whenever the HandlerBase was activated/deactivated.
			 */
			virtual void handleActivityChanged() {}
			/** Returns whether this HandlerBase' handle is active or not.
			 * @return True if this HandlerBase' handle is active, false otherwise.
			 */
			bool isHandleActive() const { return mActivated; }
		private:
			/** This function will try to activate the HandlerBase object with the given mode.
			 * If it's not the mode this HandlerBase is responsible for, it will call the deactivate function.
			 * @param mode The HandleMode to try with.
			 */
			void _tryActivate(G2S::HandleMode::Name mode);

			bool					mActivated;		// The flag telling whether this HandlerBase object is currently activated or not.
			G2S::HandleMode::Name	mHandleMode;	// HandleMode this HandlerBase is responsible for.
	};
};
