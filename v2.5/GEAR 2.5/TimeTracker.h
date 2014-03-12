// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once

#include "timevalue_linux.h"
#include "timevalue_win.h"


namespace G2 
{
	
	/** This class provides a Timer. You can use a Timer for your application to get the Time from one Event to another Event.
	 * @brief Use a Timer to get the Time from one Event to another Event in your Application.
	 * @author Andy Reimann 
	 */
	class TimeTracker 
	{
		public:
			/** This creates a new timer.
			*/
			TimeTracker();
			/** This starts the timer.
			 * @param reset If true, the timer will be reseted.
			 */
			void start( bool reset = true );
			/** This stops the Timer.
			 */
			void stop();
			/** This resets the timer.
			 */
			void reset();
			/** This function returns the time elapsed since start of timer in microseconds.
			*/
			unsigned getMicroSeconds();
			/** This function returns the time elapsed since start of timer in milli seconds.
			*/
			double getMilliSeconds();
			/** This function returns the time elapsed since start of timer in seconds.
			*/
			double getSeconds();

			~TimeTracker(); //!< destructor

		private:
        #ifndef __APPLE__
			timer::timevalue m_time;
			timer::timevalue m_start;
            #ifdef _WIN32
                clock_t endtime;
                clock_t starttime;
            #endif
        #endif
	};
};
