// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "NonInstanceable.h"
#include "Effect.h"

#include <string>
#include <memory>

namespace G2 
{
	/** This class provides a single interface for parsing G2::UberShaders.
	 * @created:	2014/02/09
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class UberShaderParser : public NonInstanceable 
	{
		public:
			/** This function will parse an UberShader from the given file name.
			 * @param fileName The name of the file to parse an UberShader from.
			 * @return The parsed UberShader.
			 */
			static std::shared_ptr<Effect::Builder> parse(std::string const& fileName);
		private:



			~UberShaderParser();
	};
};