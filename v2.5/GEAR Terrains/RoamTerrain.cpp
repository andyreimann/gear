#include "RoamTerrain.h"
#include "RoamPatch.h"
#include "Defines.h"

#include <G2Core/ECSManager.h>
#include <G2/RenderComponent.h>
#include <G2/TransformComponent.h>
#include <G2/Logger.h>
#include <G2/Frustum.h>
#include <G2/NormalMapGenerator.h>

#include <glm/gtx/spline.hpp>

using namespace G2::Terrain;

RoamTerrain::RoamTerrain()
	: mNumPatchesPerSide(0),
	mMapSize(0),
	mFrameVariance(50.f), // beginning frame variance (should be high, it will adjust automatically)
	mNumTrisRendered(0),
	mDesiredTriangleTesselations(0),
	mIsValid(false),
	mUseFrustumCulling(true)
{
}

RoamTerrain::RoamTerrain(RoamTerrain && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

RoamTerrain& RoamTerrain::operator=(RoamTerrain && rhs) 
{
	mIsValid = rhs.mIsValid;
	mUseFrustumCulling = rhs.mUseFrustumCulling;
	mMapSize = rhs.mMapSize;
	mInvMapSize = rhs.mInvMapSize;
	mPatchSize = rhs.mPatchSize;
	mMaxHeight = rhs.mMaxHeight;
	mNumPatchesPerSide = rhs.mNumPatchesPerSide;
	mPatches = std::move(rhs.mPatches);
	mHeightMap = std::move(rhs.mHeightMap);
	mHeightMapData = rhs.mHeightMapData;
	mNextTriNode = std::move(rhs.mNextTriNode);
	mTriangePool = std::move(rhs.mTriangePool);
	mFrameVariance = rhs.mFrameVariance;
	mNumTrisRendered = rhs.mNumTrisRendered;
	mDesiredTriangleTesselations = rhs.mDesiredTriangleTesselations;
	mRenderedVertices = std::move(rhs.mRenderedVertices);
	mRenderedTexCoords = std::move(rhs.mRenderedTexCoords);
	mNormalMap = rhs.mNormalMap;
	
	rhs.mPatches.clear();
	rhs.mTriangePool.clear();
	rhs.mHeightMapData = nullptr;
	rhs.mTriangePool.clear();
	
	return static_cast<RoamTerrain&>(BaseComponent::operator=(std::move(rhs)));
}

RoamTerrain::~RoamTerrain()
{
	delete[] mHeightMapData;
}

bool
RoamTerrain::setup(
		std::shared_ptr<G2::Texture2D> heightMap, 
		float maxHeight, 
		unsigned int desiredTriangles, 
		unsigned int maxTriangles) 
{
	bool isPow2 = !(heightMap->getWidth() == 0) && !(heightMap->getWidth() & (heightMap->getWidth() - 1));
	if(desiredTriangles > maxTriangles || 
	   heightMap->getWidth() != heightMap->getHeight() ||
	   !isPow2)
	{
		if (!isPow2)
		{
			G2::logger << "[G2::Terrain] Heightmap Texture needs to be power of 2!" << G2::endl;
		}
		// don't change anything
		return false;
	}
	mDesiredTriangleTesselations = desiredTriangles;

	mHeightMap = heightMap;
	mMaxHeight = maxHeight;
	mMapSize = (float)mHeightMap->getWidth();
	mTriangePool.resize(maxTriangles);
	mRenderedVertices.resize(maxTriangles*3);
	mRenderedTexCoords.resize(maxTriangles*3);

	if(mMapSize == 512)
	{
		mNumPatchesPerSide = 8;
	}
	else if(mMapSize == 1024)
	{
		mNumPatchesPerSide = 16;
	}
	else if(mMapSize == 2048)
	{
		mNumPatchesPerSide = 32;
	}
	mPatchSize = mMapSize / (float)mNumPatchesPerSide;
	mInvMapSize = 1.f / mMapSize;
	
	mPatches.resize(mNumPatchesPerSide);
	for(unsigned int y = 0; y < mNumPatchesPerSide; ++y)
	{
		mPatches[y].resize(mNumPatchesPerSide);
	}

	mHeightMap->bind(G2Core::TexSlot::TEX_SLOT1);
	// Optimization:  Add an extra row above and below the height map.
	//   - The extra top row contains a copy of the last row in the height map.
	//   - The extra bottom row contains a copy of the first row in the height map.
	// This simplifies the wrapping of height values to a trivial case. 
	mHeightMapData = new unsigned char[mHeightMap->getWidth()*mHeightMap->getHeight()*sizeof(unsigned char) + mHeightMap->getHeight() * 2];
	
	// Give the rest of the application a pointer to the actual start of the height map.
	unsigned char* dest = mHeightMapData + mHeightMap->getWidth();
	GLDEBUG(glGetTexImage(GL_TEXTURE_2D, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, dest));
	mHeightMap->unbind(G2Core::TexSlot::TEX_SLOT1);

	// Copy the last row of the height map into the extra first row.
	memcpy( mHeightMapData, mHeightMapData + mHeightMap->getWidth() * mHeightMap->getWidth(), mHeightMap->getWidth() );
	// Copy the first row of the height map into the extra last row.
	memcpy( mHeightMapData + mHeightMap->getWidth() * mHeightMap->getWidth() + mHeightMap->getWidth(), mHeightMapData + mHeightMap->getWidth(), mHeightMap->getWidth() );

	// Initialize all terrain patches
	RoamPatch* patch = nullptr;
	for ( unsigned int y = 0; y < mNumPatchesPerSide; ++y)
	{
		for ( unsigned int x = 0; x < mNumPatchesPerSide; ++x )
		{
			patch = &(mPatches[y][x]);
			patch->init( this, int(x*mPatchSize), int(y*mPatchSize), int(x*mPatchSize), int(y*mPatchSize), dest);
			patch->computeVariance(this);
		}
	}

	// put in some initial vertices
	mRenderedVertices[0] = glm::vec3(mNumPatchesPerSide * mPatchSize, 255.f, mNumPatchesPerSide * mPatchSize);

	// setup RenderComponent
	auto* renderSystem = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>();
	auto* renderComponent = renderSystem->get(getEntityId());
	if(renderComponent == nullptr)
	{
		renderComponent = renderSystem->create(getEntityId());
	}
	renderComponent->allocateVertexArrays(1);
	auto& vao = renderComponent->getVertexArray(0);
	vao.resizeElementCount(maxTriangles*3);
	vao.writeData(G2Core::Semantics::POSITION, &mRenderedVertices[0]);
	// setup TransformComponent
	auto* transformSystem = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>();
	auto* transformComponent = transformSystem->get(getEntityId());
	if(transformComponent == nullptr)
	{
		transformComponent = transformSystem->create(getEntityId());
	}
	transformComponent->setScale(glm::vec3(1.f,1.f/255.f*maxHeight,1.f));
	mIsValid = true;

	unsigned char* data = &mHeightMapData[(int)mMapSize];
	mNormalMap = G2::NormalMapGenerator::generateFromHeightMap(data, (int)mMapSize, mMaxHeight);
	renderComponent->material.setTexture(G2::Sampler::NORMAL, mNormalMap);

	// add one single DrawCall for Terrain
	renderComponent->addDrawCall(G2::DrawCall()
		.setDrawMode(G2Core::DrawMode::TRIANGLES)
		.setEnabled(true)
		.setVaoIndex(0)
		.setAABBCalculationMode(AUTOMATIC)
	);

	return mIsValid;
}

void
RoamTerrain::_reset(G2::Frustum const* cameraFrustum) 
{
	// reset the rendered triangle count
	mNumTrisRendered = 0;

	// Set the next free triangle pointer back to the beginning
	_setNextTriNode(0);

	/**
	 Supporting Larger Topologies: 
	 The included engine is structured to simplify the creation of very large worlds. 
	 By loading separate height maps for each Landscape class and then rendering each Landscape, 
	 there is no limit to its size! There are other limits however, like RAM and computational power.

	The Landscape class was designed to hold a paged-in piece of the world, 
	along with other Landscape classes holding other blocks. 
	Each Landscape must link its patches to those in the other Landscapes nearby. 
	This is done in Patch::Reset(), instead of setting the Neighbor pointers for edge nodes to NULL, 
	lookup the correct patch in the Landscape which borders that side.
	 */

	unsigned int X, Y;
	RoamPatch *patch;
	// Go through the patches performing resets, compute variances, and linking.
	for ( Y=0; Y < mNumPatchesPerSide; Y++ )
	{
		for ( X=0; X < mNumPatchesPerSide; X++)
		{
			patch = &(mPatches[Y][X]);
			
			// Reset the patch
			patch->reset();
			// do not forward the frustum in case we disabled frustum culling for this RoamTerrain
			patch->setVisibility( this, mUseFrustumCulling == true ? cameraFrustum : nullptr );
			
			// Check to see if this patch has been deformed since last frame.
			// If so, recompute the variance tree for it.
			if ( patch->isDirty() )
				patch->computeVariance(this);

			if ( patch->isVisibile() )
			{
				// Link all the patches together.
				if ( X > 0 )
					patch->getBaseLeft()->LeftNeighbor = mPatches[Y][X-1].getBaseRight();
				else
					patch->getBaseLeft()->LeftNeighbor = NULL;		// Link to bordering Landscape here..

				if ( X < (mNumPatchesPerSide-1) )
					patch->getBaseRight()->LeftNeighbor = mPatches[Y][X+1].getBaseLeft();
				else
					patch->getBaseRight()->LeftNeighbor = NULL;		// Link to bordering Landscape here..

				if ( Y > 0 )
					patch->getBaseLeft()->RightNeighbor = mPatches[Y-1][X].getBaseRight();
				else
					patch->getBaseLeft()->RightNeighbor = NULL;		// Link to bordering Landscape here..

				if ( Y < (mNumPatchesPerSide-1) )
					patch->getBaseRight()->RightNeighbor = mPatches[Y+1][X].getBaseLeft();
				else
					patch->getBaseRight()->RightNeighbor = NULL;	// Link to bordering Landscape here..
			}
		}
	}
}

void
RoamTerrain::_tesselate(glm::vec3 const& cameraPosition) 
{
	// Perform Tessellation
	RoamPatch *patch;
	for(unsigned int y = 0; y < mNumPatchesPerSide; ++y)
	{
		for(unsigned int x = 0; x < mNumPatchesPerSide; ++x)
		{
			patch = &(mPatches[y][x]);
			if (patch->isVisibile())
				patch->tessellate(this, cameraPosition);
		} 
	}
}

void
RoamTerrain::_draw() 
{
	// Scale the terrain by the terrain scale specified at compile time.
	RoamPatch *patch;
	for(unsigned int y = 0; y < mNumPatchesPerSide; ++y)
	{
		for(unsigned int x = 0; x < mNumPatchesPerSide; ++x)
		{
			patch = &(mPatches[y][x]);
			if (patch->isVisibile())
				patch->render(this);
		} 
	}

	// Check to see if we got close to the desired number of triangles.
	// Adjust the frame variance to a better value.
	if ( _getNextTriNode() != mDesiredTriangleTesselations )
		mFrameVariance += ((float)_getNextTriNode() - (float)mDesiredTriangleTesselations) / (float)mDesiredTriangleTesselations;

	// Bounds checking.
	if ( mFrameVariance < 0 )
		mFrameVariance = 0;

	auto* renderComponent = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>()->get(getEntityId());
	auto& vao = renderComponent->getVertexArray(0);
	// this resize effectively does not change the available memory inside of the VAO
	// since we initially allocated enough memory. It just ensures, that when drawing the VAO
	// only mNumTrisRendered*3 vertices are drawn!
	vao.resizeElementCount(mNumTrisRendered*3);
	vao.writeData(G2Core::Semantics::POSITION, &mRenderedVertices[0], mNumTrisRendered*3);
	vao.writeData(G2Core::Semantics::TEXCOORD_0, &mRenderedTexCoords[0], mNumTrisRendered*3);
}

TriTreeNode*
RoamTerrain::_allocateTriange() 
{
	TriTreeNode *pTri;

	// IF we've run out of TriTreeNodes, just return NULL (this is handled gracefully)
	if ( mNextTriNode >= mTriangePool.size() )
	{
		return NULL;
	}
	pTri = &(mTriangePool[mNextTriNode++]);
	pTri->LeftChild = pTri->RightChild = NULL;

	return pTri;
}

void
RoamTerrain::_updatePatchPositions(glm::mat4 const& worldSpaceMatrix) 
{
	RoamPatch* patch = nullptr;
	for ( unsigned int y = 0; y < mNumPatchesPerSide; ++y)
	{
		for ( unsigned int x = 0; x < mNumPatchesPerSide; ++x )
		{
			patch = &(mPatches[y][x]);
			patch->updatePatchPositions(this, worldSpaceMatrix);
		}
	}
}

void
RoamTerrain::setFrustumCullingMode(bool mode) 
{
	mUseFrustumCulling = mode;
}