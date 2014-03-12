#pragma once

namespace G2 
{
	class UberShader;
	class FileResource;
	/// This class takes care of parsing a Pass block from an UberShader file.
	/// @created:	2014/03/10
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class PassBlockParser 
	{
		public:
			/// This constructs a new PassBlockParser.
			PassBlockParser(UberShader* uberShader, FileResource* file);

			void parse();

		private:
			UberShader*		mUberShader;
			FileResource*	mFile;
	};
};