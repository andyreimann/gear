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
	class Frustum;
	namespace Terrain 
	{

		/** This class defines a component holding informations for a terrain
		 * surface rendered using the ROAM algorithm.
		 * @created:	2014/06/21
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class RoamTerrain : public BaseComponent<RoamTerrainSystem>
		{
				friend class RoamPatch;
				friend class RoamTerrainSystem;
			public:
				/** This constructs a new empty RoamTerrain.
				 * @note You have to call the setup function after creating the RoamTerrain to fully setup the surface.
				 */
				TERRAINSDLL_API RoamTerrain();

				/** This function will initialize the RoamTerrain.
				 * You have to call this function after creating an instance of RoamTerrain
				 * in order to load the terrain data into the RoamTerrain.
				 * @param heightMap The single channel heightmap to use.
				 * @param maxHeight The maximum height of the terrain
				 * @param desiredTriangles The maximum number of desired triangles
				 * @param maxTriangles The maximum number of triangles.
				 * @return True if the setup process finished successfully, false if not.
				 * @note Always make sure that desiredTriangles is less than maxTriangles!
				 * @note It is expected that the width and height of the heightmap is power of 2 and equal.
				 */
				TERRAINSDLL_API bool setup(
					std::shared_ptr<G2::Texture2D> heightMap, 
					float maxHeight, 
					unsigned int desiredTriangles, 
					unsigned int maxTriangles);

				TERRAINSDLL_API ~RoamTerrain();
				/** Move constructor to move a TransformComponent.
				 */
				TERRAINSDLL_API RoamTerrain(RoamTerrain && rhs);
				/** Move assignment operator to move a TransformComponent.
				 */
				TERRAINSDLL_API RoamTerrain& operator=(RoamTerrain && rhs);
				/** This function will return the size of the terrain.
				* @return The size of the terrain.
				*/
				float const& getTerrainSize() const { return mMapSize; }
				/** Updates the frustum culling mode to use.
				 * By default, the RoamTerrain will use frustum culling to cull each RoamPatch
				 * which will not be visible by the main camera. Thus there are more triangle tessellations available 
				 * to reduce the visible error of the visible area. 
				 * However this does not work when using a multi pass rendering shader from a different point of view,
				 * to render the scene from. If you encounter issues when using a multi pass shader, try to disable the frustum culling for the terrain.
				 * @note When you disable the frustum culling, you should maybe also increase the number of triangles used for the terrain to get equal quality!.
				 */
				void setFrustumCullingMode(bool mode);
				/** This function will return the frustum culling mode of the RoamTerrain.
				* @return The frustum culling mode of the RoamTerrain.
				* @see setFrustumCullingMode for more details on frustum culling when using RoamTerrain
				*/
				bool const& getFrustumCullingMode() const { return mUseFrustumCulling; }
				/** This function will return the maximum height of the RoamTerrain. 
				* @return The maximum height of the RoamTerrain.
				*/
				float const& getMaxHeight() const { return mMaxHeight; }
				/** This function will return the number of patches per side of the RoamTerrain used for the tessellation. 
				* @return The number of patches per side.
				*/
				unsigned int const& getNumPatchesPerSide() const { return mNumPatchesPerSide; }
				/** This function will return the current amount of triangles produces in the last tessellation process. 
				* @return The current amount of triangles.
				* @note This value may vary from frame to frame and indicates, how many triangles the tessellation stage produced.
				* @note Tessellation is done in the postUpdate phase of the RoamTerrainSystem
				*/
				unsigned int const& getNumTriangles() const { return mNumTrisRendered; }
				/** This function will return the currently set amount of desired triangle tessellations. 
				* @return The currently set amount of desired triangle tessellations.
				*/
				unsigned int const& getDesiredTriangleTesselations() const { return mDesiredTriangleTesselations; }
				/** This function will return the maximum number of triangles available for this RoamTerrain. 
				* @return The maximum number of triangles available for this RoamTerrain. 
				*/
				unsigned int getMaxTriangles() const { return (unsigned int)mTriangePool.size(); }
				/** This function will set the amount of desired triangle tessellations to the given value.
				* @param value The amount of desired triangle tessellations.
				* @note The value is omitted, if it is bigger than the maximum number of triangles defined. 
				* @see See the setup() funciton for more details.
				*/
				void setDesiredTriangleTesselations(unsigned int const& value) { mDesiredTriangleTesselations = value; }
			private:

				void _reset(G2::Frustum const* cameraFrustum);
				void _tesselate(glm::vec3 const& cameraPosition);
				void _draw();

				/** This function will apply the given world space matrix to the AABB objects of all RoamPatch objects.
				 * It is called from the RoamTerrainSystem, whenever the Attached TransformComponent was recalculated.
				 * @param worldSpaceMatrix The world space matrix to recalculate all RoamPatch objects AABB with.
				 */
				void _updatePatchPositions(glm::mat4 const& worldSpaceMatrix);
				
				/** This function will return the next unused triangle tree node from the triangle pool.
				 */
				TriTreeNode*	_allocateTriange();

				int _getNextTriNode() { return mNextTriNode; }
				void _setNextTriNode( int nNextNode ) { mNextTriNode = nNextNode; }
				
				bool								mIsValid;			// Flag indicating whether the RoamTerrain is ready to be calculated and rendered
				bool								mUseFrustumCulling;	// Flag indicating whether to use frustum culling or not.
				float								mMapSize;			// The size of the entire terrain map
				float								mInvMapSize;		// The inverse size of the entire terrain map
				float								mPatchSize;			// The size of one patch
				float								mMaxHeight;			// The maximum height of the terrain
				unsigned int						mNumPatchesPerSide; // the number of patches per side
				std::vector<std::vector<RoamPatch>>	mPatches;			// Array of patches
				std::shared_ptr<G2::Texture2D>		mHeightMap;			// The height map to use for this terrain
				std::shared_ptr<G2::Texture2D>		mNormalMap;			// The normal map to use for this terrain

				unsigned char*						mHeightMapData;		// DEBUG ONLY UNTIL TEXTURE PROVIDES NEEDED INTERFACE!

				int									mNextTriNode;		// Index to next free TriTreeNode
				std::vector<TriTreeNode>			mTriangePool;		// The pool of triangles available for the ROAM algorithm
				
				float								mFrameVariance;		// The currently used frame variance (this value adapts over time)
				unsigned int						mNumTrisRendered;	// The internal counter of rendered triangles per frame
				unsigned int						mDesiredTriangleTesselations; // The desired number of triangle tessellations
				std::vector<glm::vec3>				mRenderedVertices;	// The rendered vertices in a linear memory layout
				std::vector<glm::vec2>				mRenderedTexCoords;	// The rendered texture coordinates in a linear memory layout
		};
	};
};