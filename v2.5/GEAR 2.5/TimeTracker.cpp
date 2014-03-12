// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "TimeTracker.h"

#include "clock_linux.h"
#include "clock_win.h"

using namespace G2;

TimeTracker::TimeTracker()
#ifndef __APPLE__
:
#ifndef _WIN32
    m_time(timer::clock::time()),
    m_start(timer::clock::time())
#else
	m_time(timer::clockwin::time()),
    m_start(timer::clockwin::time()),
    endtime(clock()),
    starttime(clock())
#endif
#endif
{ }

void
TimeTracker::start( bool reset ) 
{
	if( reset )
	{ 
		// start the TimeTracker by reseting it
		this->reset();
	}
}

void
TimeTracker::stop() 
{
#ifndef __APPLE__
#ifndef _WIN32
    m_time = timer::clock::time() - m_start;
#endif
#endif
}

void
TimeTracker::reset()
{
#ifndef __APPLE__
#ifndef _WIN32
    m_start = timer::clock::time();
#else
	starttime = clock();
	endtime = clock();
#endif
#endif
}

unsigned
TimeTracker::getMicroSeconds()
{
#ifndef __APPLE__
#ifndef _WIN32
    return m_time.usec();
#else
    endtime = clock();
    clock_t temp = endtime - starttime;
    //std::cout << std::endl << "Start " << (unsigned) starttime << "End " << endtime << "diff: " << (unsigned)temp;
    //starttime = clock();
    return unsigned(temp * 1000);
#endif
#endif
}

double
TimeTracker::getMilliSeconds()
{
	return (getMicroSeconds() * 0.001);
}

double 
TimeTracker::getSeconds() 
{
	return (getMicroSeconds() * 0.000001);
}

TimeTracker::~TimeTracker() 
{ }
