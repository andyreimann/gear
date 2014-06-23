#include "RoamTerrain.h"

#include <G2/RenderComponent.h>
#include <G2/Logger.h>

using namespace G2::Terrain;

RoamTerrain::RoamTerrain(std::shared_ptr<G2::Texture2D>	heightMap, std::shared_ptr<G2::Effect> effect, unsigned int maxTriangles)
	: mHeightMap(heightMap),
	mEffect(effect),
	mNumPatchesPerSide(0),
	mMapSize(heightMap->getWidth()),
	mFrameVariance(50.f), // beginning frame variance (should be high, it will adjust automatically)
	mNumTrisRendered(0),
	mDesiredTris(50000)
{
	mTriangePool.resize(maxTriangles);
	mRenderedVertices.resize(maxTriangles*3);
	mVertexArray.resizeElementCount(maxTriangles*3);

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
	
	// Copy the last row of the height map into the extra first row.
	memcpy( mHeightMapData, mHeightMapData + mHeightMap->getWidth() * mHeightMap->getWidth(), mHeightMap->getWidth() );

	// Copy the first row of the height map into the extra last row.
	memcpy( mHeightMapData + mHeightMap->getWidth() * mHeightMap->getWidth() + mHeightMap->getWidth(), mHeightMapData + mHeightMap->getWidth(), mHeightMap->getWidth() );
	
	for ( unsigned int y = 0; y < mHeightMap->getWidth() + 2; ++y)
	{
		for ( unsigned int x = 0; x < mHeightMap->getWidth(); ++x )
		{
			//mHeightMapData[y*mHeightMap->getWidth() + x] = 255 * (mHeightMap->getWidth() / (float)x);
			//G2::logger << (int)mHeightMapData[y*mHeightMap->getWidth() + x] << " ";
		}
	}
	// Initialize all terrain patches
	Patch* patch = nullptr;
	for ( unsigned int y = 0; y < mNumPatchesPerSide; ++y)
	{
		for ( unsigned int x = 0; x < mNumPatchesPerSide; ++x )
		{
			patch = &(mPatches[y][x]);
			patch->Init( this, x*mPatchSize, y*mPatchSize, x*mPatchSize, y*mPatchSize, dest);
			patch->ComputeVariance(this);
		}
	}
}

RoamTerrain::RoamTerrain(RoamTerrain && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

RoamTerrain& RoamTerrain::operator=(RoamTerrain && rhs) 
{
	mEffect = rhs.mEffect;
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
	mVertexArray = std::move(rhs.mVertexArray);
	
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

	int X, Y;
	Patch *patch;

	// Set the next free triangle pointer back to the beginning
	setNextTriNode(0);

	// Go through the patches performing resets, compute variances, and linking.
	for ( Y=0; Y < mNumPatchesPerSide; Y++ )
		for ( X=0; X < mNumPatchesPerSide; X++)
		{
			patch = &(mPatches[Y][X]);
			
			// Reset the patch
			patch->Reset();
			patch->SetVisibility( this, eyeX, eyeY, leftX, leftY, rightX, rightY );
			
			// Check to see if this patch has been deformed since last frame.
			// If so, recompute the varience tree for it.
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
}

void
RoamTerrain::_draw(std::shared_ptr<G2::Shader> shader, glm::mat4 const& cameraSpaceMatrix, glm::mat4 const& modelMatrix) 
{

	// Scale the terrain by the terrain scale specified at compile time.
	
	for(unsigned int y = 0; y < mNumPatchesPerSide; ++y)
	{
		for(unsigned int x = 0; x < mNumPatchesPerSide; ++x)
		{
			if (mPatches[y][x].isVisibile())
				mPatches[y][x].Render(this, shader, cameraSpaceMatrix, modelMatrix);
		} 
	}

	// Check to see if we got close to the desired number of triangles.
	// Adjust the frame variance to a better value.
	if ( getNextTriNode() != mDesiredTris )
		mFrameVariance += ((float)getNextTriNode() - (float)mDesiredTris) / (float)mDesiredTris;

	// Bounds checking.
	if ( mFrameVariance < 0 )
		mFrameVariance = 0;

	mVertexArray.writeData(G2::Semantics::POSITION, &mRenderedVertices[0], mNumTrisRendered*3);
	mVertexArray.bind();
	mVertexArray.draw(GL_TRIANGLES, 0, mNumTrisRendered*3);
	mVertexArray.unbind();
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














// -------------------------------------------------------------------------------------------------
//	PATCH CLASS
// -------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------
// Split a single Triangle and link it into the mesh.
// Will correctly force-split diamonds.
//
void Patch::Split(RoamTerrain* terrain, TriTreeNode *tri)
{
	// We are already split, no need to do it again.
	if (tri->LeftChild)
		return;

	// If this triangle is not in a proper diamond, force split our base neighbor
	if ( tri->BaseNeighbor && (tri->BaseNeighbor->BaseNeighbor != tri) )
		Split(terrain,tri->BaseNeighbor);

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
			Split(terrain,tri->BaseNeighbor);  // Base Neighbor (in a diamond with us) was not split yet, so do that now.
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
void Patch::RecursTessellate(RoamTerrain* terrain,
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
		triangeVariance = ((float)m_CurrentVariance[node] * terrain->mMapSize * 2)/distance;	// Take both distance and variance into consideration
	}

	if ( (node >= (1<<VARIANCE_DEPTH)) ||	// IF we do not have variance info for this node, then we must have gotten here by splitting, so continue down to the lowest level.
		 (triangeVariance > terrain->mFrameVariance))	// OR if we are not below the variance tree, test for variance.
	{
		Split(terrain,tri);														// Split this triangle.
		
		if (tri->LeftChild &&											// If this triangle was split, try to split it's children as well.
			((abs(leftX - rightX) >= 3) || (abs(leftY - rightY) >= 3)))	// Tessellate all the way down to one vertex per height field entry
		{
			RecursTessellate( terrain, cameraPosition, tri->LeftChild,   apexX,  apexY, leftX, leftY, centerX, centerY,    node<<1  );
			RecursTessellate( terrain, cameraPosition, tri->RightChild, rightX, rightY, apexX, apexY, centerX, centerY, 1+(node<<1) );
		}
	}
}

// ---------------------------------------------------------------------
// Render the tree.  Simple no-fan method.
//
void Patch::RecursRender(RoamTerrain* terrain, TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY )
{
	if ( tri->LeftChild )					// All non-leaf nodes have both children, so just check for one
	{
		int centerX = (leftX + rightX)>>1;	// Compute X coordinate of center of Hypotenuse
		int centerY = (leftY + rightY)>>1;	// Compute Y coord...

		RecursRender( terrain, tri->LeftChild,  apexX,   apexY, leftX, leftY, centerX, centerY );
		RecursRender( terrain, tri->RightChild, rightX, rightY, apexX, apexY, centerX, centerY );
	}
	else									// A leaf node!  Output a triangle to be rendered.
	{

		float leftZ  = m_HeightMap[(leftY *(unsigned int)terrain->mMapSize)+leftX ];
		float rightZ = m_HeightMap[(rightY*(unsigned int)terrain->mMapSize)+rightX];
		float apexZ  = m_HeightMap[(apexY *(unsigned int)terrain->mMapSize)+apexX ];

		// Perform lighting calculations if requested.
		//if (gDrawMode == DRAW_USE_LIGHTING)
		//{
		//	float v[3][3];
		//	float out[3];
		//	
		//	// Create a vertex normal for this triangle.
		//	// NOTE: This is an extremely slow operation for illustration purposes only.
		//	//       You should use a texture map with the lighting pre-applied to the texture.
		//	v[0][0] = (GLfloat) leftX;
		//	v[0][1] = (GLfloat) leftZ;
		//	v[0][2] = (GLfloat) leftY;
		//	
		//	v[1][0] = (GLfloat) rightX;
		//	v[1][1] = (GLfloat) rightZ ;
		//	v[1][2] = (GLfloat) rightY;
		//	
		//	v[2][0] = (GLfloat) apexX;
		//	v[2][1] = (GLfloat) apexZ ;
		//	v[2][2] = (GLfloat) apexY;
		//	
		//	calcNormal( v, out );
		//	glNormal3fv( out );
		//}

		// Perform polygon coloring based on a height sample
		//float fColor = (60.0f + leftZ) / 256.0f;
		//if ( fColor > 1.0f )  fColor = 1.0f;
		//glColor3f( fColor, fColor, fColor );

		// Output the LEFT VERTEX for the triangle
		//glVertex3f(		(float) leftX,
		//				(float) leftZ,
		//				(float) leftY );
		glm::vec3& v1 = terrain->mRenderedVertices[terrain->mNumTrisRendered*3];
		v1.x = (float) leftX + (float)m_WorldX;
		v1.y = (float) leftZ;
		v1.z = (float) leftY + (float)m_WorldY;

		//if ( gDrawMode == DRAW_USE_TEXTURE ||	// Gaurad shading based on height samples instead of light normal
		//	 gDrawMode == DRAW_USE_FILL_ONLY )
		//{
		//	float fColor = (60.0f + rightZ) / 256.0f;
		//	if ( fColor > 1.0f )  fColor = 1.0f;
		//	glColor3f( fColor, fColor, fColor );
		//}

		// Output the RIGHT VERTEX for the triangle
		//glVertex3f(		(GLfloat) rightX,
		//				(GLfloat) rightZ,
		//				(GLfloat) rightY );
		glm::vec3& v2 = terrain->mRenderedVertices[terrain->mNumTrisRendered*3+1];
		v2.x = (float) rightX + (float)m_WorldX;
		v2.y = (float) rightZ;
		v2.z = (float) rightY + (float)m_WorldY;


		//if ( gDrawMode == DRAW_USE_TEXTURE ||	// Gaurad shading based on height samples instead of light normal
		//	 gDrawMode == DRAW_USE_FILL_ONLY )
		//{
		//	float fColor = (60.0f + apexZ) / 256.0f;
		//	if ( fColor > 1.0f )  fColor = 1.0f;
		//	glColor3f( fColor, fColor, fColor );
		//}

		// Output the APEX VERTEX for the triangle
		//glVertex3f(		(GLfloat) apexX,
		//				(GLfloat) apexZ,
		//				(GLfloat) apexY );
		glm::vec3& v3 = terrain->mRenderedVertices[terrain->mNumTrisRendered*3+2];
		v3.x = (float) apexX + (float)m_WorldX;
		v3.y = (float) apexZ;
		v3.z = (float) apexY + (float)m_WorldY;
		// Actual number of rendered triangles...
		++terrain->mNumTrisRendered;
	}
}

// ---------------------------------------------------------------------
// Computes Variance over the entire tree.  Does not examine node relationships.
//
unsigned char Patch::RecursComputeVariance( RoamTerrain* terrain, int leftX,  int leftY,  unsigned char leftZ,
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
	unsigned char centerZ  = m_HeightMap[(centerY * (unsigned int)terrain->mMapSize) + centerX];

	// Variance of this triangle is the actual height at it's hypotenuse midpoint minus the interpolated height.
	// Use values passed on the stack instead of re-accessing the Height Field.
	myVariance = abs((int)centerZ - (((int)leftZ + (int)rightZ)>>1));

	// Since we're after speed and not perfect representations,
	//    only calculate variance down to an 8x8 block
	if ( (abs(leftX - rightX) >= 8) ||
		 (abs(leftY - rightY) >= 8) )
	{
		// Final Variance for this node is the max of it's own variance and that of it's children.
		myVariance = MAX( myVariance, RecursComputeVariance( terrain, apexX,   apexY,  apexZ, leftX, leftY, leftZ, centerX, centerY, centerZ,    node<<1 ) );
		myVariance = MAX( myVariance, RecursComputeVariance( terrain, rightX, rightY, rightZ, apexX, apexY, apexZ, centerX, centerY, centerZ, 1+(node<<1)) );
	}

	// Store the final variance for this node.  Note Variance is never zero.
	if (node < (1<<VARIANCE_DEPTH))
		m_CurrentVariance[node] = 1 + myVariance;

	return myVariance;
}

// -------------------------------------------------------------------------------------------------
//	PATCH CLASS
// -------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------
// Initialize a patch.
//
void Patch::Init(RoamTerrain* terrain, int heightX, int heightY, int worldX, int worldY,unsigned char *map)
{
	// Clear all the relationships
	m_BaseLeft.RightNeighbor = m_BaseLeft.LeftNeighbor = m_BaseRight.RightNeighbor = m_BaseRight.LeftNeighbor =
	m_BaseLeft.LeftChild = m_BaseLeft.RightChild = m_BaseRight.LeftChild = m_BaseLeft.LeftChild = NULL;

	// Attach the two m_Base triangles together
	m_BaseLeft.BaseNeighbor = &m_BaseRight;
	m_BaseRight.BaseNeighbor = &m_BaseLeft;

	// Store Patch offsets for the world and heightmap.
	m_WorldX = worldX;
	m_WorldY = worldY;

	// Store pointer to first byte of the height data for this patch.
	m_HeightMap = &map[heightY * (unsigned int)terrain->mMapSize + heightX];

	// Initialize flags
	m_VarianceDirty = 1;
	m_isVisible = 0;
}

// ---------------------------------------------------------------------
// Reset the patch.
//
void Patch::Reset()
{
	// Assume patch is not visible.
	m_isVisible = 0;

	// Reset the important relationships
	m_BaseLeft.LeftChild = m_BaseLeft.RightChild = m_BaseRight.LeftChild = m_BaseLeft.LeftChild = NULL;

	// Attach the two m_Base triangles together
	m_BaseLeft.BaseNeighbor = &m_BaseRight;
	m_BaseRight.BaseNeighbor = &m_BaseLeft;

	// Clear the other relationships.
	m_BaseLeft.RightNeighbor = m_BaseLeft.LeftNeighbor = m_BaseRight.RightNeighbor = m_BaseRight.LeftNeighbor = NULL;
}

// ---------------------------------------------------------------------
// Compute the variance tree for each of the Binary Triangles in this patch.
//
void Patch::ComputeVariance(RoamTerrain* terrain)
{
	// Compute variance on each of the base triangles...
	unsigned int patchSize = terrain->mPatchSize;
	unsigned int mapSize = (unsigned int)terrain->mMapSize;
	m_CurrentVariance = m_VarianceLeft;
	RecursComputeVariance(terrain, 0,          patchSize, m_HeightMap[patchSize * mapSize],
							patchSize, 0,          m_HeightMap[patchSize],
							0,          0,          m_HeightMap[0],
							1);

	m_CurrentVariance = m_VarianceRight;
	RecursComputeVariance(terrain, patchSize, 0,          m_HeightMap[ patchSize],
							0,          patchSize, m_HeightMap[ patchSize * mapSize],
							patchSize, patchSize, m_HeightMap[(patchSize * mapSize) + patchSize],
							1);

	// Clear the dirty flag for this patch
	m_VarianceDirty = 0;
}

// ---------------------------------------------------------------------
// Discover the orientation of a triangle's points:
//
// Taken from "Programming Principles in Computer Graphics", L. Ammeraal (Wiley)
//
inline int orientation( int pX, int pY, int qX, int qY, int rX, int rY )
{
	int aX, aY, bX, bY;
	float d;

	aX = qX - pX;
	aY = qY - pY;

	bX = rX - pX;
	bY = rY - pY;

	d = (float)aX * (float)bY - (float)aY * (float)bX;
	return (d < 0) ? (-1) : (d > 0);
}

// ---------------------------------------------------------------------
// Set patch's visibility flag.
//
void Patch::SetVisibility(RoamTerrain* terrain, int eyeX, int eyeY, int leftX, int leftY, int rightX, int rightY )
{
	// Get patch's center point
	int patchCenterX = m_WorldX + terrain->mPatchSize / 2;
	int patchCenterY = m_WorldY + terrain->mPatchSize / 2;
	
	// Set visibility flag (orientation of both triangles must be counter clockwise)
	m_isVisible = true;
//	m_isVisible = (orientation( eyeX,  eyeY,  rightX, rightY, patchCenterX, patchCenterY ) < 0) &&
//				  (orientation( leftX, leftY, eyeX,   eyeY,   patchCenterX, patchCenterY ) < 0);
}

// ---------------------------------------------------------------------
// Create an approximate mesh.
//
void Patch::Tessellate(RoamTerrain* terrain, glm::vec3 const& cameraPosition)
{
	// Split each of the base triangles
	m_CurrentVariance = m_VarianceLeft;
	RecursTessellate (	terrain, cameraPosition, 
						&m_BaseLeft,
						m_WorldX,				m_WorldY+terrain->mPatchSize,
						m_WorldX+terrain->mPatchSize,	m_WorldY,
						m_WorldX,				m_WorldY,
						1 );
					
	m_CurrentVariance = m_VarianceRight;
	RecursTessellate(	terrain, cameraPosition, 
						&m_BaseRight,
						m_WorldX+terrain->mPatchSize,	m_WorldY,
						m_WorldX,				m_WorldY+terrain->mPatchSize,
						m_WorldX+terrain->mPatchSize,	m_WorldY+terrain->mPatchSize,
						1 );
}

// ---------------------------------------------------------------------
// Render the mesh.
//
void Patch::Render(RoamTerrain* terrain, std::shared_ptr<G2::Shader> shader, glm::mat4 const& cameraSpaceMatrix, glm::mat4 const& modelMatrix)
{
	
	// Translate the patch to the proper world coordinates
	//shader->setProperty(std::string("matrices.model_matrix"), modelMatrix * glm::translate((float)m_WorldX, 0.f, (float)m_WorldY));
	//shader->setProperty(std::string("matrices.modelview_matrix"), cameraSpaceMatrix * modelMatrix * glm::translate((float)m_WorldX, 0.f, (float)m_WorldY));
	//glBegin(GL_TRIANGLES);
		
		RecursRender ( terrain,	&m_BaseLeft,
			0,				terrain->mPatchSize,
			terrain->mPatchSize,		0,
			0,				0);
		
		RecursRender( terrain, &m_BaseRight,
			terrain->mPatchSize,		0,
			0,				terrain->mPatchSize,
			terrain->mPatchSize,		terrain->mPatchSize);
	
	//glEnd();
}