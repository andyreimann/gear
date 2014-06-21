#pragma once
#include "Defines.h"
#include <G2Core/BaseSystem.h>

namespace G2 
{
	namespace Terrain 
	{
		class TerrainComponent;
		/** This class defines...
		 * @created:	2014/06/21
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class TerrainSystem : public BaseSystem<TerrainSystem,TerrainComponent>
		{
			public:
				/// This constructs a new TerrainSystem.
				TERRAINSDLL_API TerrainSystem();
				
				TERRAINSDLL_API void runPhase(std::string const& name, G2::FrameInfo const& frameInfo);

			private:
		};
	};
};