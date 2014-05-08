// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "LocationBinding.h"

#include <vector>

namespace G2 
{
	class FileResource;
	/** This class takes care of loading the content of an LocationBindings block
	 * inside of an UberShader file.
	 * @created:	2014/02/09
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class LocationBindingsBlockParser 
	{
		public:
			/** This constructs a new LocationBindingsBlockParser.
			 */
			LocationBindingsBlockParser(FileResource* file);
			void parse();
			/** This function will return the LocationBindings. 
			* @return The current LocationBindings.
			*/
			std::vector<LocationBinding> const& getLocationBindings() const { return mLocationBindings; }
		protected:
		private:

			FileResource*					mFile;
			std::vector<LocationBinding>	mLocationBindings;
	};
};