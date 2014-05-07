#pragma once
#include <memory>
#include "Effect.h"

namespace G2 
{
	class ShaderBlockParser;
	class FileResource;
	/// This class takes care of parsing a Pass block from an UberShader file.
	/// @created:	2014/03/10
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class PassBlockParser 
	{
		public:
			/// This constructs a new PassBlockParser.
			PassBlockParser(Effect::Builder* builder, FileResource* file);

			void parse();

			std::shared_ptr<ShaderBlockParser> getShaderBlockParser() const { return mShaderBlockParser; }

		private:
			Effect::Builder*					mBuilder;
			FileResource*						mFile;
			std::shared_ptr<ShaderBlockParser>	mShaderBlockParser;
	};
};