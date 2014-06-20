#pragma once
#include "Defines.h"

#include <G2/Texture.h>
#include <G2Core/Entity.h>

#include <string>

namespace G2 
{

	namespace Terrain 
	{
		/** This class defines...
		 * @created:	2014/06/20
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class Terrain 
		{
			public:
				/// This constructs a new Terrain.
				TERRAINSDLL_API Terrain(std::shared_ptr<G2::Texture> const& heightMap);

			private:
				
				// UNSURE VARIABLES
				unsigned int	mClipmapWidth;
				float			mTileSize;
				// UNSURE VARIABLES END

				std::shared_ptr<G2::Texture> mHeightMap;

				G2::Entity	mTerrainEntity;
		};
	};
};