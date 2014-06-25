#pragma once
#include "Defines.h"

#include <G2/AABB.h>
#include <glm/glm.hpp>

// Depth of variance tree: should be near SQRT(PATCH_SIZE) + 1
#define VARIANCE_DEPTH (9)



#define SQR(x) ((x) * (x))
#define MAX(a,b) ((a < b) ? (b) : (a))

namespace G2 
{ 
	class Frustum;
	namespace Terrain 
	{

		class RoamTerrain;

		struct TriTreeNode
		{
			TriTreeNode *LeftChild;
			TriTreeNode *RightChild;
			TriTreeNode *BaseNeighbor;
			TriTreeNode *LeftNeighbor;
			TriTreeNode *RightNeighbor;
		};
		/** A RoamPatch holds all informations for a small patch of a complete RoamTerrain.
		 * A RoamTerrain is split up into a certain amount of RoamPatches during setup.
		 * The amount of splits depends on the terrain size.
		 * @created:	2014/06/24
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class RoamPatch
		{
			public:
				// Some encapsulation functions & extras
				TERRAINSDLL_API TriTreeNode* getBaseLeft()  { return &mBaseLeft; }
				TERRAINSDLL_API TriTreeNode* getBaseRight() { return &mBaseRight; }
				/** Returns whether the patch needs a variance recalculation.
				 * This can happen if the underlying height map was modified.
				 */
				TERRAINSDLL_API char isDirty()     { return mVarianceDirty; }

				TERRAINSDLL_API int  isVisibile( ) { return mIsVisible; }
				TERRAINSDLL_API void setVisibility(RoamTerrain* terrain, G2::Frustum const* cameraFrustum );

				// The static half of the Patch Class
				TERRAINSDLL_API virtual void init(RoamTerrain* terrain, int heightX, int heightY, int worldX, int worldY, unsigned char *map);
				TERRAINSDLL_API virtual void reset();
				TERRAINSDLL_API virtual void tessellate(RoamTerrain* terrain, glm::vec3 const& cameraPosition);
				TERRAINSDLL_API virtual void render(RoamTerrain* terrain);
				TERRAINSDLL_API virtual void computeVariance(RoamTerrain* terrain);

				// The recursive half of the Patch Class
				TERRAINSDLL_API virtual void			split(RoamTerrain* terrain, TriTreeNode *tri);
				TERRAINSDLL_API virtual void			recursTessellate(RoamTerrain* terrain, glm::vec3 const& cameraPosition, TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY, int node );
				TERRAINSDLL_API virtual void			recursRender( RoamTerrain* terrain, TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY );
				TERRAINSDLL_API virtual unsigned char	recursComputeVariance(	RoamTerrain* terrain, int leftX,  int leftY,  unsigned char leftZ,
																int rightX, int rightY, unsigned char rightZ,
																int apexX,  int apexY,  unsigned char apexZ,
																int node);
				/** This function will apply the given world space matrix to the AABB object of this RoamPatch.
				 * @param worldSpaceMatrix The world space matrix to recalculate the RoamPatch AABB with.
				 */
				TERRAINSDLL_API void updatePatchPositions(RoamTerrain* terrain, glm::mat4 const& worldSpaceMatrix);
			private:
				/** This function will calculate a sharp normal based on the given counter clockwise triangle points 
				 * @param p1 The first point
				 * @param p2 The second point
				 * @param p3 The third  point
				 * @param normal A reference to the normal to write the calculated normal to.
				 */
				void _calculateSharpNormal(glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3, glm::vec3& normal);

				unsigned char* mHeightMap;									// Pointer to height map to use for this patch
				int mWorldX, mWorldY;										// World coordinate offset of this patch.

				unsigned char	mVarianceLeft [1<<(VARIANCE_DEPTH)];		// Left variance tree
				unsigned char	mVarianceRight[1<<(VARIANCE_DEPTH)];		// Right variance tree

				unsigned char*	mCurrentVariance;							// Which variance we are currently using. [Only valid during the Tessellate and ComputeVariance passes]
				unsigned char	mVarianceDirty;								// Does the variance Tree need to be recalculated for this Patch?
				unsigned char	mIsVisible;									// Is this patch visible in the current frame?

				TriTreeNode mBaseLeft;										// Left base triangle tree node
				TriTreeNode mBaseRight;										// Right base triangle tree node

				AABB		mAABB;											// The AABB of this Patch
		};
	};
};