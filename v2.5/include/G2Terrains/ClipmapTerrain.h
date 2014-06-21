#pragma once
#include "ClipmapTerrainSystem.h"

#include <G2Core/BaseComponent.h>
#include <G2/Texture.h>
#include <G2/VertexArrayObject.h>
#include <G2/Effect.h>
#include <memory>


namespace G2 
{
	namespace Terrain 
	{
		struct IndexBuffer
		{
			unsigned int id;
			unsigned int numIndices;
		};
		/** This class defines a component holding informations for a terrain
		 * @created:	2014/06/21
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class ClipmapTerrain : public BaseComponent<ClipmapTerrainSystem>
		{
			friend class ClipmapTerrainSystem;
			public:
				/// This constructs a new TerrainComponent.
				TERRAINSDLL_API ClipmapTerrain();

				TERRAINSDLL_API void setup(std::shared_ptr<G2::Texture> const& heightMap, std::shared_ptr<G2::Effect> effect);
			private:
				
				/** This function returns the (i-1)'th jacobsthal number.
				 * This can be used to get the shift needed for the i'th ring to adjust them in the grid.
				 * The use of these numbers in the clipmap algorithm was found in an empirical way by Andy Reimann.
				 * sequence directive: getJacobsthalNumber(i) = getJacobsthalNumber(i-1) + (2*getJacobsthalNumber(i-2))
				 */
				int _getJacobsthalNumber( int i );
				/** This function will return the Texture Coordinate offset, which is needed for the ring i.
				 * To connect the texture coordinates of the rings across all rings, we have to shift them in every ring.
				 * This function will calculate this offsets using the Jacobsthal sequence:
				 * sequence directive: a(i) = a(i-2) + (1 / (getJacobsthalNumber(i-1)+getJacobsthalNumber(i))
				 * @return the Texture Coordinate offset for the ring i.
				 */
				glm::vec2 const& _getJacobsthalNumberTexCoordOffset(int i );
				

				bool mInitialized;
				
				std::shared_ptr<G2::Texture> mHeightMap;
				std::shared_ptr<G2::Effect> mEffect;
				G2::VertexArrayObject	mVertexArray;		// The vertex array of the ClipmapTerrain
				
				unsigned int			mClipmapWidth;
				float					mTileSize;

				/* Index buffers */
				IndexBuffer				mViewport;
				IndexBuffer				mMMInstanceBlock;
				IndexBuffer				mOuterDegenerateX;
				IndexBuffer				mOuterDegenerateZ;
				IndexBuffer				mRingFixupX;
				IndexBuffer				mRingFixupZ;
				IndexBuffer				mInteriorTrimX;
				IndexBuffer				mInteriorTrimZ;

				std::vector<int>		mJacobsthalNumbers;
				std::vector<glm::vec2>	mJacobsthalNumbersTexCoordOffsets;

				glm::vec3				mStartPosition;

		};
	};
};