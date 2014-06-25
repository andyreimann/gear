#include "RoamPatch.h"
#include "RoamTerrain.h"

#include <G2/Frustum.h>

using namespace G2::Terrain;

// ---------------------------------------------------------------------
// Split a single Triangle and link it into the mesh.
// Will correctly force-split diamonds.
//
void RoamPatch::split(RoamTerrain* terrain, TriTreeNode *tri)
{
	// We are already split, no need to do it again.
	if (tri->LeftChild)
		return;

	// If this triangle is not in a proper diamond, force split our base neighbor
	if ( tri->BaseNeighbor && (tri->BaseNeighbor->BaseNeighbor != tri) )
		split(terrain,tri->BaseNeighbor);

	// Create children and link into mesh
	tri->LeftChild  = terrain->_allocateTriange();
	tri->RightChild = terrain->_allocateTriange();

	// If creation failed, just exit.
	if ( !tri->LeftChild )
		return;

	// Fill in the information we can get from the parent (neighbor pointers)
	tri->LeftChild->BaseNeighbor  = tri->LeftNeighbor;
	tri->LeftChild->LeftNeighbor  = tri->RightChild;

	tri->RightChild->BaseNeighbor  = tri->RightNeighbor;
	tri->RightChild->RightNeighbor = tri->LeftChild;

	// Link our Left Neighbor to the new children
	if (tri->LeftNeighbor != NULL)
	{
		if (tri->LeftNeighbor->BaseNeighbor == tri)
			tri->LeftNeighbor->BaseNeighbor = tri->LeftChild;
		else if (tri->LeftNeighbor->LeftNeighbor == tri)
			tri->LeftNeighbor->LeftNeighbor = tri->LeftChild;
		else if (tri->LeftNeighbor->RightNeighbor == tri)
			tri->LeftNeighbor->RightNeighbor = tri->LeftChild;
		else
			;// Illegal Left Neighbor!
	}

	// Link our Right Neighbor to the new children
	if (tri->RightNeighbor != NULL)
	{
		if (tri->RightNeighbor->BaseNeighbor == tri)
			tri->RightNeighbor->BaseNeighbor = tri->RightChild;
		else if (tri->RightNeighbor->RightNeighbor == tri)
			tri->RightNeighbor->RightNeighbor = tri->RightChild;
		else if (tri->RightNeighbor->LeftNeighbor == tri)
			tri->RightNeighbor->LeftNeighbor = tri->RightChild;
		else
			;// Illegal Right Neighbor!
	}

	// Link our Base Neighbor to the new children
	if (tri->BaseNeighbor != NULL)
	{
		if ( tri->BaseNeighbor->LeftChild )
		{
			tri->BaseNeighbor->LeftChild->RightNeighbor = tri->RightChild;
			tri->BaseNeighbor->RightChild->LeftNeighbor = tri->LeftChild;
			tri->LeftChild->RightNeighbor = tri->BaseNeighbor->RightChild;
			tri->RightChild->LeftNeighbor = tri->BaseNeighbor->LeftChild;
		}
		else
			split(terrain,tri->BaseNeighbor);  // Base Neighbor (in a diamond with us) was not split yet, so do that now.
	}
	else
	{
		// An edge triangle, trivial case.
		tri->LeftChild->RightNeighbor = NULL;
		tri->RightChild->LeftNeighbor = NULL;
	}
}

// ---------------------------------------------------------------------
// Tessellate a Patch.
// Will continue to split until the variance metric is met.
//
void RoamPatch::recursTessellate(RoamTerrain* terrain,
							 glm::vec3 const& cameraPosition, 
							 TriTreeNode *tri,
							 int leftX,  int leftY,
							 int rightX, int rightY,
							 int apexX,  int apexY,
							 int node )
{
	float triangeVariance;
	int centerX = (leftX + rightX)>>1; // Compute X coordinate of center of Hypotenuse
	int centerY = (leftY + rightY)>>1; // Compute Y coord...

	if ( node < (1<<VARIANCE_DEPTH) )
	{
		// Extremely slow distance metric (sqrt is used).
		// Replace this with a faster one!
		float distance = 1.0f + sqrtf( SQR((float)centerX - cameraPosition.x) +
									   SQR((float)centerY - cameraPosition.z) );
		
		// Egads!  A division too?  What's this world coming to!
		// This should also be replaced with a faster operation.
		triangeVariance = ((float)mCurrentVariance[node] * terrain->mMapSize * 2)/distance;	// Take both distance and variance into consideration
	}

	if ( (node >= (1<<VARIANCE_DEPTH)) ||	// IF we do not have variance info for this node, then we must have gotten here by splitting, so continue down to the lowest level.
		 (triangeVariance > terrain->mFrameVariance))	// OR if we are not below the variance tree, test for variance.
	{
		split(terrain,tri);														// Split this triangle.
		
		if (tri->LeftChild &&											// If this triangle was split, try to split it's children as well.
			((abs(leftX - rightX) >= 3) || (abs(leftY - rightY) >= 3)))	// Tessellate all the way down to one vertex per height field entry
		{
			recursTessellate( terrain, cameraPosition, tri->LeftChild,   apexX,  apexY, leftX, leftY, centerX, centerY,    node<<1  );
			recursTessellate( terrain, cameraPosition, tri->RightChild, rightX, rightY, apexX, apexY, centerX, centerY, 1+(node<<1) );
		}
	}
}

// ---------------------------------------------------------------------
// Render the tree.  Simple no-fan method.
//
void RoamPatch::recursRender(RoamTerrain* terrain, TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY )
{
	if ( tri->LeftChild )					// All non-leaf nodes have both children, so just check for one
	{
		int centerX = (int)((leftX + rightX) * 0.5f);	// Compute X coordinate of center of Hypotenuse
		int centerY = (int)((leftY + rightY) * 0.5f);	// Compute Y coord...

		recursRender( terrain, tri->LeftChild,  apexX,   apexY, leftX, leftY, centerX, centerY );
		recursRender( terrain, tri->RightChild, rightX, rightY, apexX, apexY, centerX, centerY );
	}
	else									// A leaf node!  Output a triangle to be rendered.
	{
		float leftZ  = mHeightMap[(leftY *(unsigned int)terrain->mMapSize)+leftX ];
		float rightZ = mHeightMap[(rightY*(unsigned int)terrain->mMapSize)+rightX];
		float apexZ  = mHeightMap[(apexY *(unsigned int)terrain->mMapSize)+apexX ];
		
		glm::vec3& v1 = terrain->mRenderedVertices[terrain->mNumTrisRendered*3];
		v1.x = (float) leftX + (float)mWorldX;
		v1.y = (float) leftZ;
		v1.z = (float) leftY + (float)mWorldY;
		glm::vec2& t1 = terrain->mRenderedTexCoords[terrain->mNumTrisRendered*3];
		t1.x = (v1.x) * terrain->mInvMapSize;
		t1.y = (v1.z) * terrain->mInvMapSize;

		glm::vec3& v2 = terrain->mRenderedVertices[terrain->mNumTrisRendered*3+1];
		v2.x = (float) rightX + (float)mWorldX;
		v2.y = (float) rightZ;
		v2.z = (float) rightY + (float)mWorldY;
		glm::vec2& t2 = terrain->mRenderedTexCoords[terrain->mNumTrisRendered*3+1];
		t2.x = (v2.x) * terrain->mInvMapSize;
		t2.y = (v2.z) * terrain->mInvMapSize;

		glm::vec3& v3 = terrain->mRenderedVertices[terrain->mNumTrisRendered*3+2];
		v3.x = (float) apexX + (float)mWorldX;
		v3.y = (float) apexZ;
		v3.z = (float) apexY + (float)mWorldY;
		glm::vec2& t3 = terrain->mRenderedTexCoords[terrain->mNumTrisRendered*3+2];
		t3.x = (v3.x) * terrain->mInvMapSize;
		t3.y = (v3.z) * terrain->mInvMapSize;

		//_calculateSharpNormal( v1, v2, v3, terrain->mRenderedNormals[terrain->mNumTrisRendered*3] );
		//terrain->mRenderedNormals[terrain->mNumTrisRendered*3+1] = terrain->mRenderedNormals[terrain->mNumTrisRendered*3+2] = terrain->mRenderedNormals[terrain->mNumTrisRendered*3];
		
		// Actual number of rendered triangles...
		++terrain->mNumTrisRendered;
	}
}

// ---------------------------------------------------------------------
// Computes Variance over the entire tree.  Does not examine node relationships.
//
unsigned char RoamPatch::recursComputeVariance( RoamTerrain* terrain, int leftX,  int leftY,  unsigned char leftZ,
											int rightX, int rightY, unsigned char rightZ,
											int apexX,  int apexY,  unsigned char apexZ,
											int node)
{
	//        /|\
	//      /  |  \
	//    /    |    \
	//  /      |      \
	//  ~~~~~~~*~~~~~~~  <-- Compute the X and Y coordinates of '*'
	//
	int centerX = (leftX + rightX) >>1;		// Compute X coordinate of center of Hypotenuse
	int centerY = (leftY + rightY) >>1;		// Compute Y coord...
	unsigned char myVariance;

	// Get the height value at the middle of the Hypotenuse
	unsigned char centerZ  = mHeightMap[(centerY * (unsigned int)terrain->mMapSize) + centerX];

	// Variance of this triangle is the actual height at it's hypotenuse midpoint minus the interpolated height.
	// Use values passed on the stack instead of re-accessing the Height Field.
	myVariance = abs((int)centerZ - (((int)leftZ + (int)rightZ)>>1));

	// Since we're after speed and not perfect representations,
	//    only calculate variance down to an 8x8 block
	if ( (abs(leftX - rightX) >= 8) ||
		 (abs(leftY - rightY) >= 8) )
	{
		// Final Variance for this node is the max of it's own variance and that of it's children.
		myVariance = MAX( myVariance, recursComputeVariance( terrain, apexX,   apexY,  apexZ, leftX, leftY, leftZ, centerX, centerY, centerZ,    node<<1 ) );
		myVariance = MAX( myVariance, recursComputeVariance( terrain, rightX, rightY, rightZ, apexX, apexY, apexZ, centerX, centerY, centerZ, 1+(node<<1)) );
	}

	// Store the final variance for this node.  Note Variance is never zero.
	if (node < (1<<VARIANCE_DEPTH))
		mCurrentVariance[node] = 1 + myVariance;

	return myVariance;
}

// -------------------------------------------------------------------------------------------------
//	PATCH CLASS
// -------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------
// Initialize a patch.
//
void RoamPatch::init(RoamTerrain* terrain, int heightX, int heightY, int worldX, int worldY,unsigned char *map)
{

	// Store Patch offsets for the world and heightmap.
	mWorldX = worldX;
	mWorldY = worldY;

	// Store pointer to first byte of the height data for this patch.
	mHeightMap = &map[heightY * (unsigned int)terrain->mMapSize + heightX];

	// Initialize flags
	mVarianceDirty = 1;
	
	mAABB.merge(glm::vec3(worldX, 0.f, worldY));
	mAABB.merge(glm::vec3(worldX+terrain->mPatchSize, terrain->mMaxHeight, worldY+terrain->mPatchSize));

	reset();
}

// ---------------------------------------------------------------------
// Reset the patch.
//
void RoamPatch::reset()
{
	// Assume patch is not visible.
	mIsVisible = 0;

	// Reset the important relationships
	mBaseLeft.LeftChild = mBaseLeft.RightChild = mBaseRight.LeftChild = mBaseLeft.LeftChild = NULL;

	// Attach the two m_Base triangles together
	mBaseLeft.BaseNeighbor = &mBaseRight;
	mBaseRight.BaseNeighbor = &mBaseLeft;

	// Clear the other relationships.
	mBaseLeft.RightNeighbor = mBaseLeft.LeftNeighbor = mBaseRight.RightNeighbor = mBaseRight.LeftNeighbor = NULL;
}

// ---------------------------------------------------------------------
// Compute the variance tree for each of the Binary Triangles in this patch.
//
void RoamPatch::computeVariance(RoamTerrain* terrain)
{
	// Compute variance on each of the base triangles...
	unsigned int patchSize = (unsigned int)terrain->mPatchSize;
	unsigned int mapSize = (unsigned int)terrain->mMapSize;
	mCurrentVariance = mVarianceLeft;
	recursComputeVariance(terrain, 0,          patchSize, mHeightMap[patchSize * mapSize],
							patchSize, 0,          mHeightMap[patchSize],
							0,          0,          mHeightMap[0],
							1);

	mCurrentVariance = mVarianceRight;
	recursComputeVariance(terrain, patchSize, 0,          mHeightMap[ patchSize],
							0,          patchSize, mHeightMap[ patchSize * mapSize],
							patchSize, patchSize, mHeightMap[(patchSize * mapSize) + patchSize],
							1);

	// Clear the dirty flag for this patch
	mVarianceDirty = 0;
}

// ---------------------------------------------------------------------
// Discover the orientation of a triangle's points:
//
// Taken from "Programming Principles in Computer Graphics", L. Ammeraal (Wiley)
//
//inline int orientation( int pX, int pY, int qX, int qY, int rX, int rY )
//{
//	int aX, aY, bX, bY;
//	float d;
//
//	aX = qX - pX;
//	aY = qY - pY;
//
//	bX = rX - pX;
//	bY = rY - pY;
//
//	d = (float)aX * (float)bY - (float)aY * (float)bX;
//	return (d < 0) ? (-1) : (d > 0);
//}

// ---------------------------------------------------------------------
// Set patch's visibility flag.
//
void RoamPatch::setVisibility(RoamTerrain* terrain, G2::Frustum const* cameraFrustum )
{
	// Get patch's center point
	//int patchCenterX = m_WorldX + terrain->mPatchSize / 2;
	//int patchCenterY = m_WorldY + terrain->mPatchSize / 2;
	
	// Set visibility flag (orientation of both triangles must be counter clockwise)
	mIsVisible = true;
	if(cameraFrustum != nullptr)
	{
		mIsVisible = cameraFrustum->inside(mAABB);
	}
//	m_isVisible = (orientation( eyeX,  eyeY,  rightX, rightY, patchCenterX, patchCenterY ) < 0) &&
//				  (orientation( leftX, leftY, eyeX,   eyeY,   patchCenterX, patchCenterY ) < 0);
}

// ---------------------------------------------------------------------
// Create an approximate mesh.
//
void RoamPatch::tessellate(RoamTerrain* terrain, glm::vec3 const& cameraPosition)
{
	// Split each of the base triangles
	mCurrentVariance = mVarianceLeft;
	recursTessellate (	terrain, cameraPosition, 
						&mBaseLeft,
						mWorldX,							mWorldY+(int)terrain->mPatchSize,
						mWorldX+(int)terrain->mPatchSize,	mWorldY,
						mWorldX,							mWorldY,
						1 );
					
	mCurrentVariance = mVarianceRight;
	recursTessellate(	terrain, cameraPosition, 
						&mBaseRight,
						mWorldX+(int)terrain->mPatchSize,	mWorldY,
						mWorldX,							mWorldY+(int)terrain->mPatchSize,
						mWorldX+(int)terrain->mPatchSize,	mWorldY+(int)terrain->mPatchSize,
						1 );
}

// ---------------------------------------------------------------------
// Render the mesh.
//
void RoamPatch::render(RoamTerrain* terrain)
{
	recursRender ( terrain,	&mBaseLeft,
		0,							(int)terrain->mPatchSize,
		(int)terrain->mPatchSize,	0,
		0,							0);
		
	recursRender( terrain, &mBaseRight,
		(int)terrain->mPatchSize,	0,
		0,							(int)terrain->mPatchSize,
		(int)terrain->mPatchSize,	(int)terrain->mPatchSize);
	
}

void
RoamPatch::_calculateSharpNormal(glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3, glm::vec3& normal) 
{
	// ---------------------------------------------------------------------
	// Points p1, p2, & p3 specified in counter clock-wise order

	// Take the cross product of the two vectors to get
	// the normal vector
	normal = glm::cross(p1 - p2,p2 - p3);
}

void
RoamPatch::updatePatchPositions(RoamTerrain* terrain, glm::mat4 const& worldSpaceMatrix) 
{
	mAABB.clear();
	mAABB.merge(worldSpaceMatrix * glm::vec4((float)mWorldX, 0.f, (float)mWorldY, 1.f));
	mAABB.merge(worldSpaceMatrix * glm::vec4((float)mWorldX+terrain->mPatchSize, 255.f, (float)mWorldY+terrain->mPatchSize, 1.f));
}