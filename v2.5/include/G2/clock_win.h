// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#ifdef _WIN32
#ifndef CLOCK_WIN_H
#define CLOCK_WIN_H

#include "timevalue_win.h"

namespace timer{
  class clockwin{
  public:
    static const timevalue time() ;
  };
}


#endif // #ifndef CLOCK_WIN_H
#endif // #ifdef WIN32

