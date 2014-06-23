#pragma once
#include "RoamTerrainSystem.h"

#include <G2Core/BaseComponent.h>
#include <G2/Texture2D.h>
#include <G2/VertexArrayObject.h>
#include <G2/Effect.h>

#include <vector>

// Depth of variance tree: should be near SQRT(PATCH_SIZE) + 1
#define VARIANCE_DEPTH (9)



#define SQR(x) ((x) * (x))
#define MAX(a,b) ((a < b) ? (b) : (a))

namespace G2 
{
		

	namespace Terrain 
	{
		struct TriTreeNode
		{
			TriTreeNode *LeftChild;
			TriTreeNode *RightChild;
			TriTreeNode *BaseNeighbor;
			TriTreeNode *LeftNeighbor;
			TriTreeNode *RightNeighbor;
		};

		class RoamTerrain;
		//
		// Patch Class
		// Store information needed at the Patch level
		//
		class Patch
		{
		protected:
	unsigned char *m_HeightMap;											// Pointer to height map to use
			int m_WorldX, m_WorldY;										// World coordinate offset of this patch.

			unsigned char	m_VarianceLeft [1<<(VARIANCE_DEPTH)];			// Left variance tree
			unsigned char	m_VarianceRight[1<<(VARIANCE_DEPTH)];			// Right variance tree

			unsigned char*	m_CurrentVariance;							// Which variance we are currently using. [Only valid during the Tessellate and ComputeVariance passes]
			unsigned char	m_VarianceDirty;								// Does the variance Tree need to be recalculated for this Patch?
			unsigned char	m_isVisible;									// Is this patch visible in the current frame?

			TriTreeNode m_BaseLeft;										// Left base triangle tree node
			TriTreeNode m_BaseRight;									// Right base triangle tree node

		public:
			// Some encapsulation functions & extras
			TERRAINSDLL_API TriTreeNode* GetBaseLeft()  { return &m_BaseLeft; }
			TERRAINSDLL_API TriTreeNode* GetBaseRight() { return &m_BaseRight; }
			TERRAINSDLL_API char isDirty()     { return m_VarianceDirty; }
			TERRAINSDLL_API int  isVisibile( ) { return m_isVisible; }
			TERRAINSDLL_API void SetVisibility(RoamTerrain* terrain, int eyeX, int eyeY, int leftX, int leftY, int rightX, int rightY );

			// The static half of the Patch Class
			TERRAINSDLL_API virtual void Init(RoamTerrain* terrain, int heightX, int heightY, int worldX, int worldY, unsigned char *map);
			TERRAINSDLL_API virtual void Reset();
			TERRAINSDLL_API virtual void Tessellate(RoamTerrain* terrain, glm::vec3 const& cameraPosition);
			TERRAINSDLL_API virtual void Render(RoamTerrain* terrain, glm::mat4 const& cameraSpaceMatrix, glm::mat4 const& modelMatrix);
			TERRAINSDLL_API virtual void ComputeVariance(RoamTerrain* terrain);

			// The recursive half of the Patch Class
			TERRAINSDLL_API virtual void			Split(RoamTerrain* terrain, TriTreeNode *tri);
			TERRAINSDLL_API virtual void			RecursTessellate(RoamTerrain* terrain, glm::vec3 const& cameraPosition, TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY, int node );
			TERRAINSDLL_API virtual void			RecursRender( RoamTerrain* terrain, TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY );
			TERRAINSDLL_API virtual unsigned char	RecursComputeVariance(	RoamTerrain* terrain, int leftX,  int leftY,  unsigned char leftZ,
															int rightX, int rightY, unsigned char rightZ,
															int apexX,  int apexY,  unsigned char apexZ,
															int node);
		};

		/** This class defines a component holding informations for a terrain
		 * @created:	2014/06/21
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class RoamTerrain : public BaseComponent<RoamTerrainSystem>
		{
				friend class Patch;
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
				void _draw(glm::mat4 const& cameraSpaceMatrix, glm::mat4 const& modelMatrix);
				
				TriTreeNode*	_allocateTriange();
				int getNextTriNode() { return mNextTriNode; }
				void setNextTriNode( int nNextNode ) { mNextTriNode = nNextNode; }
				
				float							mMapSize;			// The size of the entire terrain map
				float							mPatchSize;			// The size of one patch
				unsigned int					mNumPatchesPerSide; // the number of patches per side
				std::vector<std::vector<Patch>>	mPatches;			// Array of patches
				std::shared_ptr<G2::Texture2D>	mHeightMap;			// The height map to use for this terrain

				unsigned char*					mHeightMapData;		// DEBUG ONLY UNTIL TEXTURE PROVIDES NEEDED INTERFACE!

				int								mNextTriNode;		// Index to next free TriTreeNode
				std::vector<TriTreeNode>		mTriangePool;		// The pool of triangles available for the ROAM algorithm
				
				
				float							mFrameVariance;
				unsigned int					mNumTrisRendered;	// The internal counter of rendered triangles per frame
				unsigned int					mDesiredTris;
				
				std::vector<glm::vec3>			mRenderedVertices;	// The rendered vertices in a linear memory layout
				G2::VertexArrayObject			mVertexArray;		// The vertex array of the ClipmapTerrain
		};
	};
};