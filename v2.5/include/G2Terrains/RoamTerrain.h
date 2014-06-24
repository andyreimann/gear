#pragma once
#include "RoamTerrainSystem.h"
#include "RoamPatch.h"

#include <G2Core/BaseComponent.h>
#include <G2/Texture2D.h>
#include <G2/VertexArrayObject.h>
#include <G2/Effect.h>
#include <vector>

namespace G2 
{
	namespace Terrain 
	{

		/** This class defines a component holding informations for a terrain
		 * @created:	2014/06/21
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class RoamTerrain : public BaseComponent<RoamTerrainSystem>
		{
				friend class RoamPatch;
				friend class RoamTerrainSystem;
			public:
				/// This constructs a new TerrainComponent.
				TERRAINSDLL_API RoamTerrain();

				TERRAINSDLL_API void setup(std::shared_ptr<G2::Texture2D>	heightMap, std::shared_ptr<G2::Effect> effect, float maxHeight, unsigned int maxTriangles = 100000);

				TERRAINSDLL_API ~RoamTerrain();
				/** Move constructor to move a TransformComponent.
				 */
				TERRAINSDLL_API RoamTerrain(RoamTerrain && rhs);
				/** Move assignment operator to move a TransformComponent.
				 */
				TERRAINSDLL_API RoamTerrain& operator=(RoamTerrain && rhs);
			private:

				void _reset(glm::vec3 const& cameraPosition, float fovY, float cameraYawAngle);
				void _tesselate(glm::vec3 const& cameraPosition);
				void _draw();
				
				TriTreeNode*	_allocateTriange();
				int getNextTriNode() { return mNextTriNode; }
				void setNextTriNode( int nNextNode ) { mNextTriNode = nNextNode; }
				
				float							mMapSize;			// The size of the entire terrain map
				float							mPatchSize;			// The size of one patch
				unsigned int					mNumPatchesPerSide; // the number of patches per side
				std::vector<std::vector<RoamPatch>>	mPatches;			// Array of patches
				std::shared_ptr<G2::Texture2D>	mHeightMap;			// The height map to use for this terrain

				unsigned char*					mHeightMapData;		// DEBUG ONLY UNTIL TEXTURE PROVIDES NEEDED INTERFACE!

				int								mNextTriNode;		// Index to next free TriTreeNode
				std::vector<TriTreeNode>		mTriangePool;		// The pool of triangles available for the ROAM algorithm
				
				
				float							mFrameVariance;
				unsigned int					mNumTrisRendered;	// The internal counter of rendered triangles per frame
				unsigned int					mDesiredTris;
				
				std::vector<glm::vec3>			mRenderedVertices;	// The rendered vertices in a linear memory layout
				std::vector<glm::vec3>			mRenderedNormals;	// The rendered normals in a linear memory layout
				std::vector<glm::vec2>			mRenderedTexCoords;	// The rendered texture coordinates in a linear memory layout

				unsigned int					mNumRecalculations; 
		};
	};
};