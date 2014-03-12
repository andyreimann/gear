// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#ifdef _WIN32

#include "clock_win.h"
#include "timevalue_win.h"

#include <time.h>
#include <iostream>

namespace timer {

  /*static*/ const timevalue clockwin::time(){
    return timevalue((uint64_t)(clock()));
  }

}

#endif // #ifdef WIN
