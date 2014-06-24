#include "RoamTerrain.h"
#include "RoamPatch.h"

#include <G2Core/ECSManager.h>
#include <G2/RenderComponent.h>
#include <G2/TransformComponent.h>
#include <G2/Logger.h>

using namespace G2::Terrain;

RoamTerrain::RoamTerrain()
	: mNumPatchesPerSide(0),
	mMapSize(0),
	mFrameVariance(50.f), // beginning frame variance (should be high, it will adjust automatically)
	mNumTrisRendered(0),
	mDesiredTris(50000)
{
}

void
RoamTerrain::setup(std::shared_ptr<G2::Texture2D> heightMap, std::shared_ptr<G2::Effect> effect, float maxHeight, unsigned int maxTriangles) 
{
	mHeightMap = heightMap;
	mMapSize = mHeightMap->getWidth();
	mTriangePool.resize(maxTriangles);
	mRenderedVertices.resize(maxTriangles*3);
	mRenderedNormals.resize(maxTriangles*3);
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
	
	mPatches.resize(mNumPatchesPerSide);
	for(unsigned int y = 0; y < mNumPatchesPerSide; ++y)
	{
		mPatches[y].resize(mNumPatchesPerSide);
	}

	mHeightMap->bind(G2::TEX_SLOT1);
	// Optimization:  Add an extra row above and below the height map.
	//   - The extra top row contains a copy of the last row in the height map.
	//   - The extra bottom row contains a copy of the first row in the height map.
	// This simplifies the wrapping of height values to a trivial case. 
	mHeightMapData = new unsigned char[mHeightMap->getWidth()*mHeightMap->getHeight()*sizeof(unsigned char) + mHeightMap->getHeight() * 2];
	
	// Give the rest of the application a pointer to the actual start of the height map.
	unsigned char* dest = mHeightMapData + mHeightMap->getWidth();
	glGetTexImage(GL_TEXTURE_2D, 0,G2::LUMINANCE, GL_UNSIGNED_BYTE, dest);
	mHeightMap->unbind(G2::TEX_SLOT1);

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
			patch->Init( this, x*mPatchSize, y*mPatchSize, x*mPatchSize, y*mPatchSize, dest);
			patch->ComputeVariance(this);
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
	renderComponent->drawMode = GL_TRIANGLES;
	//renderComponent->setEffect(effect);
	renderComponent->allocateVertexArrays(1);
	auto& vao = renderComponent->getVertexArray(0);
	vao.resizeElementCount(maxTriangles*3);
	vao.writeData(Semantics::POSITION, &mRenderedVertices[0]);
	// setup TransformComponent
	auto* transformSystem = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>();
	auto* transformComponent = transformSystem->get(getEntityId());
	if(transformComponent == nullptr)
	{
		transformComponent = transformSystem->create(getEntityId());
	}
	transformComponent->setScale(glm::vec3(1.f,1.f/255.f*maxHeight,1.f));
}

RoamTerrain::RoamTerrain(RoamTerrain && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

RoamTerrain& RoamTerrain::operator=(RoamTerrain && rhs) 
{
	mMapSize = rhs.mMapSize;
	mPatchSize = rhs.mPatchSize;
	mNumPatchesPerSide = rhs.mNumPatchesPerSide;
	mPatches = std::move(rhs.mPatches);
	mHeightMap = std::move(rhs.mHeightMap);
	mHeightMapData = rhs.mHeightMapData;
	mNextTriNode = std::move(rhs.mNextTriNode);
	mTriangePool = std::move(rhs.mTriangePool);
	mFrameVariance = rhs.mFrameVariance;
	mNumTrisRendered = rhs.mNumTrisRendered;
	mDesiredTris = rhs.mDesiredTris;
	mRenderedVertices = std::move(rhs.mRenderedVertices);
	mRenderedNormals = std::move(rhs.mRenderedNormals);
	mRenderedTexCoords = std::move(rhs.mRenderedTexCoords);
	mNumRecalculations = rhs.mNumRecalculations;
	
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

void
RoamTerrain::_reset(glm::vec3 const& cameraPosition, float fovY, float cameraYawAngle) 
{
	// reset the rendered triangle count
	mNumTrisRendered = 0;

	//
	// Perform simple visibility culling on entire patches.
	//   - Define a triangle set back from the camera by one patch size, following
	//     the angle of the frustum.
	//   - A patch is visible if it's center point is included in the angle: Left,Eye,Right
	//   - This visibility test is only accurate if the camera cannot look up or down significantly.
	//
	const float PI_DIV_180 = M_PI / 180.0f;
	const float FOV_DIV_2 = fovY/2;

	int eyeX = (int)(cameraPosition.x - mPatchSize * sinf( cameraYawAngle * PI_DIV_180 ));
	int eyeY = (int)(cameraPosition.z + mPatchSize * cosf( cameraYawAngle * PI_DIV_180 ));

	int leftX  = (int)(eyeX + 100.0f * sinf( (cameraYawAngle-FOV_DIV_2) * PI_DIV_180 ));
	int leftY  = (int)(eyeY - 100.0f * cosf( (cameraYawAngle-FOV_DIV_2) * PI_DIV_180 ));

	int rightX = (int)(eyeX + 100.0f * sinf( (cameraYawAngle+FOV_DIV_2) * PI_DIV_180 ));
	int rightY = (int)(eyeY - 100.0f * cosf( (cameraYawAngle+FOV_DIV_2) * PI_DIV_180 ));


	// Set the next free triangle pointer back to the beginning
	setNextTriNode(0);

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

	
	int X, Y;
	RoamPatch *patch;
	// Go through the patches performing resets, compute variances, and linking.
	for ( Y=0; Y < mNumPatchesPerSide; Y++ )
	{
		for ( X=0; X < mNumPatchesPerSide; X++)
		{
			patch = &(mPatches[Y][X]);
			
			// Reset the patch
			patch->Reset();
			patch->SetVisibility( this, eyeX, eyeY, leftX, leftY, rightX, rightY );
			
			// Check to see if this patch has been deformed since last frame.
			// If so, recompute the variance tree for it.
			if ( patch->isDirty() )
				patch->ComputeVariance(this);

			if ( patch->isVisibile() )
			{
				// Link all the patches together.
				if ( X > 0 )
					patch->GetBaseLeft()->LeftNeighbor = mPatches[Y][X-1].GetBaseRight();
				else
					patch->GetBaseLeft()->LeftNeighbor = NULL;		// Link to bordering Landscape here..

				if ( X < (mNumPatchesPerSide-1) )
					patch->GetBaseRight()->LeftNeighbor = mPatches[Y][X+1].GetBaseLeft();
				else
					patch->GetBaseRight()->LeftNeighbor = NULL;		// Link to bordering Landscape here..

				if ( Y > 0 )
					patch->GetBaseLeft()->RightNeighbor = mPatches[Y-1][X].GetBaseRight();
				else
					patch->GetBaseLeft()->RightNeighbor = NULL;		// Link to bordering Landscape here..

				if ( Y < (mNumPatchesPerSide-1) )
					patch->GetBaseRight()->RightNeighbor = mPatches[Y+1][X].GetBaseLeft();
				else
					patch->GetBaseRight()->RightNeighbor = NULL;	// Link to bordering Landscape here..
			}
		}
	}
}

void
RoamTerrain::_tesselate(glm::vec3 const& cameraPosition) 
{
	// Perform Tessellation
	for(unsigned int y = 0; y < mNumPatchesPerSide; ++y)
	{
		for(unsigned int x = 0; x < mNumPatchesPerSide; ++x)
		{
			if (mPatches[y][x].isVisibile())
				mPatches[y][x].Tessellate(this, cameraPosition);
		} 
	}
	++mNumRecalculations;
}

void
RoamTerrain::_draw() 
{

	// Scale the terrain by the terrain scale specified at compile time.
	
	for(unsigned int y = 0; y < mNumPatchesPerSide; ++y)
	{
		for(unsigned int x = 0; x < mNumPatchesPerSide; ++x)
		{
			if (mPatches[y][x].isVisibile())
				mPatches[y][x].Render(this);
		} 
	}

	// Check to see if we got close to the desired number of triangles.
	// Adjust the frame variance to a better value.
	if ( getNextTriNode() != mDesiredTris )
		mFrameVariance += ((float)getNextTriNode() - (float)mDesiredTris) / (float)mDesiredTris;

	// Bounds checking.
	if ( mFrameVariance < 0 )
		mFrameVariance = 0;

	auto* renderComponent = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>()->get(getEntityId());
	auto& vao = renderComponent->getVertexArray(0);
	// this resize effectively does not change the available memory inside of the VAO
	// since we initially allocated enough memory. It just ensures, that when drawing the VAO
	// only mNumTrisRendered*3 vertices are drawn!
	vao.resizeElementCount(mNumTrisRendered*3);
	vao.writeData(G2::Semantics::POSITION, &mRenderedVertices[0], mNumTrisRendered*3);
	vao.writeData(G2::Semantics::NORMAL, &mRenderedNormals[0], mNumTrisRendered*3);
	vao.writeData(G2::Semantics::TEXCOORD_0, &mRenderedTexCoords[0], mNumTrisRendered*3);
}

TriTreeNode*
RoamTerrain::_allocateTriange() 
{
	TriTreeNode *pTri;

	// IF we've run out of TriTreeNodes, just return NULL (this is handled gracefully)
	if ( mNextTriNode >= mTriangePool.size() )
		return NULL;

	pTri = &(mTriangePool[mNextTriNode++]);
	pTri->LeftChild = pTri->RightChild = NULL;

	return pTri;
}