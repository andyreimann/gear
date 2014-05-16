// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"

namespace G2
{
	class NonCopyable 
	{

		protected:
			COREDLL_API NonCopyable() {}
			COREDLL_API ~NonCopyable() {}

		private:
			COREDLL_API NonCopyable(NonCopyable const&) {}
			COREDLL_API NonCopyable const& operator=(NonCopyable const&) { return *this; }
	};

};