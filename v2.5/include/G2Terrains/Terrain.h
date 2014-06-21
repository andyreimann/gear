#pragma once
#include "Defines.h"

#include <G2/Texture.h>
#include <G2Core/Entity.h>

#include <string>

namespace G2 
{
	class RenderComponent;
	class Shader;

	namespace Terrain 
	{
		enum TerrainDrawStage
		{
			VIEWPORT_MESH = 0,		// is rendered every time
			MXM_BLOCK_1,
			OUTER_DEGENERATE_X_1,
			OUTER_DEGENERATE_X_2,
			OUTER_DEGENERATE_Z_1,
			OUTER_DEGENERATE_Z_2,
			MXM_BLOCK_2,
			MXM_BLOCK_3,
			MXM_BLOCK_4,
			MXM_BLOCK_5,
			MXM_BLOCK_6,
			MXM_BLOCK_7,
			MXM_BLOCK_8,
			MXM_BLOCK_9,
			MXM_BLOCK_10,
			MXM_BLOCK_11,
			MXM_BLOCK_12,
			RING_FIXUP_X_1,
			RING_FIXUP_X_2,
			RING_FIXUP_Z_1,
			RING_FIXUP_Z_2,
			INTERIOR_TRIM_Z, // may be omitted
			INTERIOR_TRIM_X, // may be omitted
			NUM_DRAW_STAGES,
		};
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

				//void _onDrawCallEvent(
				//	G2::RenderComponent* renderComponent,
				//	unsigned int vertexArrayObjectIndex,
				//	unsigned int drawCallIndex,
				//	unsigned int& drawMode,
				//	glm::mat4 const& cameraSpaceMatrix,
				//	std::shared_ptr<Shader> const& boundShader,
				//	bool& render
				//);
				
				// UNSURE VARIABLES
				unsigned int	mClipmapWidth;
				float			mTileSize;
				// UNSURE VARIABLES END

				std::shared_ptr<G2::Texture> mHeightMap;

				G2::Entity	mTerrainEntity;

				unsigned int mTerrainDrawModes[TerrainDrawStage::NUM_DRAW_STAGES];


				unsigned int mGrid;			// patch resolution
				unsigned int mLodLevels;	// number of level of detail

		};
	};
};