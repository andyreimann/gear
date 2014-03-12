// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once

namespace G2 
{
	typedef int Version;
	/** This class defines...
	 * @created:	2014/02/13
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class VersionTracker 
	{
		public:

			VersionTracker() : mVersion(0) 
			{}

			Version getVersion() const
			{
				return mVersion;
			}

			virtual ~VersionTracker() = 0 {}
		protected:
			
			void updateVersion() 
			{
				++mVersion;
			}
			
			void setVersion(Version version) 
			{
				mVersion = version;
			}

		private:
			Version	mVersion;
	};
};