#pragma once
#include <string>

namespace G2 
{
	/** This class defines the basis of an importer responsible for importing a block
	 * of an Effect file.
	 * @created:	2014/11/09
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class EffectBlockImporter 
	{
		public:
			/// This constructs a new EffectBlockImporter.
			EffectBlockImporter() {}
			/** This callback function will be called automatically on the 
			 * EffectBlockImporter instance if a new shader block will be read.
			 */
			virtual void beginBlock() = 0;
			/** Processes one line read from the Effect file.
			 * @param line The line to process.
			 */
			virtual void processLine(std::string const& line) = 0;
			/** This callback function will be called automatically on the
			 * EffectBlockImporter instance if a shader block has ended.
			 */
			virtual void endBlock() = 0;

	};
};