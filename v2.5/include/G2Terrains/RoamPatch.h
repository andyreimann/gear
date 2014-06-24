#pragma once
#include "Defines.h"

#include <glm/glm.hpp>

// Depth of variance tree: should be near SQRT(PATCH_SIZE) + 1
#define VARIANCE_DEPTH (9)



#define SQR(x) ((x) * (x))
#define MAX(a,b) ((a < b) ? (b) : (a))

namespace G2 
{
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
		/** This class defines...
		 * @created:	2014/06/24
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */

		class RoamPatch
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
				TERRAINSDLL_API virtual void Render(RoamTerrain* terrain);
				TERRAINSDLL_API virtual void ComputeVariance(RoamTerrain* terrain);

				// The recursive half of the Patch Class
				TERRAINSDLL_API virtual void			Split(RoamTerrain* terrain, TriTreeNode *tri);
				TERRAINSDLL_API virtual void			RecursTessellate(RoamTerrain* terrain, glm::vec3 const& cameraPosition, TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY, int node );
				TERRAINSDLL_API virtual void			RecursRender( RoamTerrain* terrain, TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY );
				TERRAINSDLL_API virtual unsigned char	RecursComputeVariance(	RoamTerrain* terrain, int leftX,  int leftY,  unsigned char leftZ,
																int rightX, int rightY, unsigned char rightZ,
																int apexX,  int apexY,  unsigned char apexZ,
																int node);

			private:
				void calcNormal(glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3, glm::vec3& normal);
		};
	};
};