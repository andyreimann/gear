// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "NonInstanceable.h"

#include <string>

namespace G2 
{
	namespace Tools 
	{
		/** This class provides some easy to use helper functions based on std::string objects.
		 * @created:	2014/02/09
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class String : public NonInstanceable
		{
			public:

			static std::string trim(std::string const& str);
			/** Checks if the given string ends with the given suffix.
			 * @param str The string to search in.
			 * @param suffix The suffix to search for.
			 * @return True if the given string ends with the suffix, false if not.
			 */
			static bool endsWith(std::string const& str, std::string const& suffix);



			private:
				~String();
		};

	};
};
