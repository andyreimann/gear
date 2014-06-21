#pragma once
#include "TerrainSystem.h"

#include <G2Core/BaseComponent.h>

namespace G2 
{
	namespace Terrain 
	{
		/** This class defines a component holding informations for a terrain
		 * @created:	2014/06/21
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class TerrainComponent : public BaseComponent<TerrainSystem>
		{
			public:
				/// This constructs a new TerrainComponent.
				TERRAINSDLL_API TerrainComponent();
			private:
		};
	};
};