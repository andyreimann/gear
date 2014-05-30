#pragma once
#include "Importer.h"
#include "Effect.h"

namespace G2 
{
	/// This class implements the generic resource importer interface
	/// to provide an importer for Effect objects from files.
	/// @created:	2014/03/11
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class EffectImporter : public Importer<EffectImporter,Effect, Effect::Builder>
	{
		public:
			std::shared_ptr<Effect> importResource(std::string const& fileName);
			std::pair<std::string,std::shared_ptr<Effect::Builder>> produceResourceBuilder(std::string const& fileName);
	};
};