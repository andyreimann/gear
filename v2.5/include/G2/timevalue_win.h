// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#ifdef _WIN32
#ifndef TIMEVALUE_WIN_H
#define TIMEVALUE_WIN_H

#include <time.h>
#include <stdio.h>
#include <stdint.h>



namespace timer {


  class timevalue{
  public:
    timevalue();
    timevalue(uint64_t t);
    const timevalue& operator=(const timevalue& rhs);
    ~timevalue();
    unsigned usec();
    uint64_t m_t;
  };

  extern const timevalue operator-(const timevalue& a, const timevalue& b);

}



#endif // #ifnef  TIMEVALUE_WIN_H
#endif //  #ifdef WIN32

