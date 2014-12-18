#pragma once
#include "Importer.h"
#include "EffectBlockImporter.h"
#include "Effect.h"

//#define USE_NEW_EFFECT_IMPORTER 1

#ifdef USE_NEW_EFFECT_IMPORTER
// core block importers of the engine
#include "RootImporter.h"
#include <stack>
#endif

namespace G2 
{
	/// This class implements the generic resource importer interface
	/// to provide an importer for Effect objects from files.
	/// @created:	2014/03/11
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class EffectImporter : public Importer<EffectImporter, std::shared_ptr<Effect>, Effect::Builder>
	{
		public:

			std::shared_ptr<Effect> importResource(std::string const& fileName);
			std::pair<std::string,std::shared_ptr<Effect::Builder>> produceResourceBuilder(std::string const& fileName);

		protected:

#ifdef USE_NEW_EFFECT_IMPORTER
			RootImporter		mRootImporter;
			
			EffectBlockImporter* mCurrentBlockImporter;

			std::stack<EffectBlockImporter*> mBlockImporterStack;
#endif

	};
};