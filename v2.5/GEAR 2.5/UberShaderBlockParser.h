// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Effect.h"

namespace G2 
{
	class FileResource;
	/** This class takes care of loading the content of an UberShader block 
	 * inside of an UberShader file.
	 * @created:	2014/02/09
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class UberShaderBlockParser 
	{
		public:
			/** This constructs a new UberShaderBlockParser.
			 */
			UberShaderBlockParser(Effect::Builder* builder, FileResource* file);
			
			void parse();
		private:

			Effect::Builder*	mBuilder;
			FileResource*		mFile;
	};
};