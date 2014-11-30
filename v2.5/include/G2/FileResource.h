// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"

#include <string>
#include <fstream>

namespace G2 
{
	/** With a FileResource object, you can read files from a hard drive.
	 * @created:	2014/01/19
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class FileResource 
	{
	
		public:
			/** This constructs a new FileResource.
			 */
			FileResource(std::string const& name);
			/** Copy constructor.
			 */
			FileResource(FileResource const& rhs);
			/** Basic assignment operator.
			 */
			FileResource& operator=(FileResource const& rhs);

			G2_DEPRECATED bool load();

			std::string const& getContent() const { return mContent; }

			bool isOpen() const;

			bool eof() const;

			std::string getLine();

			/** normal destructor
			 */
			~FileResource();
		protected:
			// protected members
			bool		mLoaded;
			std::string mName; // The name of the FileResource
			std::string mContent; // The content of the FileResource

			std::ifstream	mFileHandle;

		private:
			// private members
	};
};