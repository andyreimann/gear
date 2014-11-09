#pragma once
#include "EffectBlockImporter.h"

namespace G2 
{
	/** This class imports all data contained in the root element of an Effect file.
	 * It is one of the core importers of the Effect file format.
	 * @created:	2014/11/09
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class RootImporter : public EffectBlockImporter 
	{
		public:


			/** This callback function will be called automatically on the
			* EffectBlockImporter instance if a new shader block will be read.
			*/
			virtual void beginBlock() override;
			/** Processes one line read from the Effect file.
			* @param line The line to process.
			*/
			virtual void processLine(std::string const& line) override;
			/** This callback function will be called automatically on the
			* EffectBlockImporter instance if a shader block has ended.
			*/
			virtual void endBlock() override;
	};
};