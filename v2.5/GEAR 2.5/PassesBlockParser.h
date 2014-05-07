#pragma once
#include "PassBlockParser.h"
#include "Effect.h"

#include <vector>

namespace G2 
{
	class FileResource;
	/// This class is responsible for parsing a passes block inside of an UberShader file.
	/// @created:	2014/03/10
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class PassesBlockParser 
	{
		public:
			/** This constructs a new ShaderBlockParser.
			 */
			PassesBlockParser(Effect::Builder* builder, FileResource* file);

			void parse();
			std::vector<PassBlockParser> const& getParsedPasses() const;

		private:
			Effect::Builder*				mBuilder;
			FileResource*					mFile;
			std::vector<PassBlockParser>	mPassBlockParser;
	};
};