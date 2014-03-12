// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#ifdef _WIN32

#include "timevalue_win.h"

#include <time.h>


namespace timer{

  timevalue::timevalue()
    : m_t(0)
  {}


  timevalue::timevalue(uint64_t t)
    : m_t(t)
  {}

  const timevalue&
  timevalue::operator=(const timevalue& rhs){
    if(this != &rhs){
      m_t = rhs.m_t;
    }
    return *this;
  }

  timevalue::~timevalue()
  {}


  unsigned
  timevalue::usec(){
    return (uint64_t) (clock());
  }

  extern const timevalue operator-(const timevalue& a, const timevalue& b){
    return (a.m_t - b.m_t);
  }


}

#endif // #ifdef _WIN32

