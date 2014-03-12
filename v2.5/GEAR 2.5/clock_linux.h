// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#ifndef _WIN32
#ifndef __APPLE__
#if !defined(SENSOR_CLOCK_H)

#define SENSOR_CLOCK_H

// includes, system

//#include <boost/noncopyable.hpp> // boost::noncopyable
#include <string>                // std::string

// includes, project

#include <level_1/system/timevalue_linux.h>

// exported namespace

namespace timer {
  
  // types, exported (class, enum, struct, union, typedef)
    
//class clock : private boost::noncopyable {
    
class clock {
    
  public:

    static const timevalue   resolution;
    static const std::string name;
    
    static timevalue time();        // wraps ::clock_gettime
    static timevalue time_of_day(); // wraps ::gettimeofday

  };
  
  // variables, exported (extern)
  
  // functions, inlined (inline)
  
  // functions, exported (extern)
  
  timevalue current_time();
  timevalue current_time_of_day();

} // namespace timer {

#endif // #if !defined(SENSOR_CLOCK_H)
#endif
#endif

