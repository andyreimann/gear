#include "ClipmapTerrain.h"

#include <G2/RenderComponent.h>

using namespace G2::Terrain;

ClipmapTerrain::ClipmapTerrain()
	: mClipmapWidth(16),
	mTileSize(1.0f) // only 1.f works
{
	
}

void
ClipmapTerrain::setup(std::shared_ptr<G2::Texture> const& heightMap, std::shared_ptr<G2::Effect> effect) 
{
	mEffect = effect;
	mHeightMap = heightMap;
	unsigned int vdSize =  4 * mClipmapWidth + 1; // dont understand this
	unsigned int numVertices = vdSize * vdSize;
	mVertexArray.resizeElementCount(numVertices);
	
	std::vector<glm::vec3> geometry(numVertices);
	std::vector<glm::vec2> texCoords(numVertices);

	
	for(unsigned int y = 0; y < vdSize; ++y )
	{
		for(unsigned int x = 0; x < vdSize; ++x ) 
		{
			glm::vec3& pos = geometry[y*vdSize+x];
			glm::vec2& texCoord = texCoords[y*vdSize+x];
			pos.x = -(x * mTileSize);
			pos.y = 0.0f; // height is calculated in shader
			pos.z = -(y * mTileSize);
			texCoord.s = x / (float)mHeightMap->getWidth();
			texCoord.t = y / (float)mHeightMap->getHeight();
		}
	}
	mVertexArray.writeData(G2Core::Semantics::POSITION, &geometry[0]);
	mVertexArray.writeData(G2Core::Semantics::TEXCOORD_0, &texCoords[0]);

	// create the index buffer for the viewport mesh in the middle (8x8 Block)
	int size = 2 * mClipmapWidth + 1;
	mViewport.numIndices = (size-1) * (size*2 +2) - 2;
	std::vector<unsigned int> indices(mViewport.numIndices);
	int i = 0;
	int cnt = 0;
	int startX = 0;
	int startY = 0;
	for( int y = startY; y < startY+size-1; ++y )
	{
		for( int x = startX; x < startX+size; ++x ) 
		{
			if( startX == x && startY != y ) {
				i = x + (y * vdSize);
				indices[cnt++] = i;
			}

			i = x + (y * vdSize);
			indices[cnt++] = i;
				
			i = x + ((y+1) * vdSize);
			indices[cnt++] = i;

			if( startX+size-1 == x && (startY+size-2) != y ) {
				i = x + ((y+1) * vdSize);
				indices[cnt++] = i;
			}
		}
	}
	GLDEBUG( glGenBuffers(1, &mViewport.id) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mViewport.id) );
	GLDEBUG( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mViewport.numIndices, &indices[0], GL_STATIC_DRAW) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL) );
	
	// create the index buffer for the mxm block which starts at [0,0] in world coordinates
	size = mClipmapWidth;
	mMMInstanceBlock.numIndices = (size-1) * (size*2 +2) - 2;
	cnt = 0;
	startX = 0;
	startY = 0;
	for( int y = startY; y < startY+size-1; ++y )
	{
		for( int x = startX; x < startX+size; ++x ) 
		{
			if( startX == x && startY != y ) {
				i = x + (y * vdSize);
				indices[cnt++] = i;
			}

			i = x + (y * vdSize);
			indices[cnt++] = i;
				
			i = x + ((y+1) * vdSize);
			indices[cnt++] = i;

			if( startX+size-1 == x && (startY+size-2) != y ) {
				i = x + ((y+1) * vdSize);
				indices[cnt++] = i;
			}
		}
	}
	GLDEBUG( glGenBuffers(1, &mMMInstanceBlock.id) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mMMInstanceBlock.id) );
	GLDEBUG( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mMMInstanceBlock.numIndices, &indices[0], GL_STATIC_DRAW) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL) );
	
	// create the mx3 block for the x-axis instance which starts at [0,0] in world coordinates 
	int width = mClipmapWidth;
	int height = 3;
	mRingFixupX.numIndices = (height-1) * (width*2 +2) - 2;
	indices.resize(mRingFixupX.numIndices);
	cnt = 0;
	startX = 0;
	startY = 0;
	for( int y = startY; y < startY+height-1; ++y )
	{
		for( int x = startX; x < startX+width; ++x ) 
		{
			if( startX == x && startY != y ) {
				i = x + (y * vdSize);
				indices[cnt++] = i;
			}

			i = x + (y * vdSize);
			indices[cnt++] = i;
				
			i = x + ((y+1) * vdSize);
			indices[cnt++] = i;

			if( startX+width-1 == x && (startY+height-2) != y ) {
				i = x + ((y+1) * vdSize);
				indices[cnt++] = i;
			}
		}
	}
	GLDEBUG( glGenBuffers(1, &mRingFixupX.id) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRingFixupX.id) );
	GLDEBUG( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mRingFixupX.numIndices, &indices[0], GL_STATIC_DRAW) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL) );
	
	// create the 3xm block for the z-axis instance which starts at [0,0] in world coordinates 
	width = 3;
	height = mClipmapWidth;
	mRingFixupZ.numIndices = (height-1) * (width*2 +2) - 2;
	indices.resize(mRingFixupZ.numIndices);
	cnt = 0;
	startX = 0;
	startY = 0;
	for( int y = startY; y < startY+height-1; ++y )
	{
		for( int x = startX; x < startX+width; ++x ) 
		{
			if( startX == x && startY != y ) {
				i = x + (y * vdSize);
				indices[cnt++] = i;
			}

			i = x + (y * vdSize);
			indices[cnt++] = i;
				
			i = x + ((y+1) * vdSize);
			indices[cnt++] = i;

			if( startX+width-1 == x && (startY+height-2) != y ) {
				i = x + ((y+1) * vdSize);
				indices[cnt++] = i;
			}
		}
	}
	GLDEBUG( glGenBuffers(1, &mRingFixupZ.id) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRingFixupZ.id) );
	GLDEBUG( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mRingFixupZ.numIndices, &indices[0], GL_STATIC_DRAW) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL) );
	
	// create the interior trim block for the x-axis instance which starts at [0,0] in world coordinates 
	width = 2 * mClipmapWidth;
	height = 2;
	mInteriorTrimX.numIndices = (height-1) * (width*2 +2) - 2;
	indices.resize(mInteriorTrimX.numIndices);
	cnt = 0;
	startX = 0;
	startY = 0;
	for( int y = startY; y < startY+height-1; ++y )
	{
		for( int x = startX; x < startX+width; ++x ) 
		{
			if( startX == x && startY != y ) {
				i = x + (y * vdSize);
				indices[cnt++] = i;
			}

			i = x + (y * vdSize);
			indices[cnt++] = i;
				
			i = x + ((y+1) * vdSize);
			indices[cnt++] = i;

			if( startX+width-1 == x && (startY+height-2) != y ) {
				i = x + ((y+1) * vdSize);
				indices[cnt++] = i;
			}
		}
	}
	GLDEBUG( glGenBuffers(1, &mInteriorTrimX.id) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mInteriorTrimX.id) );
	GLDEBUG( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mInteriorTrimX.numIndices, &indices[0], GL_STATIC_DRAW) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL) );
	
	// create the interior trim block for the z-axis instance which starts at [0,0] in world coordinates 
	width = 2;
	height = 2 * mClipmapWidth + 1; // plus one is right here, but not in mInteriorTrimX
	mInteriorTrimZ.numIndices = (height-1) * (width*2 +2) - 2;
	indices.resize(mInteriorTrimZ.numIndices);
	cnt = 0;
	startX = 0;
	startY = 0;
	for( int y = startY; y < startY+height-1; ++y )
	{
		for( int x = startX; x < startX+width; ++x ) 
		{
			if( startX == x && startY != y ) {
				i = x + (y * vdSize);
				indices[cnt++] = i;
			}

			i = x + (y * vdSize);
			indices[cnt++] = i;
				
			i = x + ((y+1) * vdSize);
			indices[cnt++] = i;

			if( startX+width-1 == x && (startY+height-2) != y ) {
				i = x + ((y+1) * vdSize);
				indices[cnt++] = i;
			}
		}
	}
	GLDEBUG( glGenBuffers(1, &mInteriorTrimZ.id) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mInteriorTrimZ.id) );
	GLDEBUG( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mInteriorTrimZ.numIndices, &indices[0], GL_STATIC_DRAW) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL) );
	





	
	
	// create the outer degenerate
	startX = 0;
	startY = 0;
	mOuterDegenerateX.numIndices = 3 * (2 * mClipmapWidth + 1);
	indices.clear();
	indices.resize(mOuterDegenerateX.numIndices);
	cnt = 0;
	for( unsigned int x = startX; x < startX+(4 * (mClipmapWidth-1)+2); x+=2 ) {
		indices[cnt++] = x;
		indices[cnt++] = x+2;
		indices[cnt++] = x+1;
	}
	GLDEBUG( glGenBuffers(1, &mOuterDegenerateX.id) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mOuterDegenerateX.id) );
	GLDEBUG( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mOuterDegenerateX.numIndices, &indices[0], GL_STATIC_DRAW) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL) );
	startX = 0;
	startY = 0;
	mOuterDegenerateZ.numIndices = 3 * (2 * mClipmapWidth +1 );
	indices.resize(mOuterDegenerateZ.numIndices);
	cnt = 0;
	for(unsigned int y = startY; y < startY+(4 * (mClipmapWidth-1)+2); y+=2 ) {
		indices[cnt++] = y * vdSize;
		indices[cnt++] = (y+2) * vdSize;
		indices[cnt++] = (y+1) * vdSize;
	}
	GLDEBUG( glGenBuffers(1, &mOuterDegenerateZ.id) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mOuterDegenerateZ.id) );
	GLDEBUG( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mOuterDegenerateZ.numIndices, &indices[0], GL_STATIC_DRAW) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL) );
	
	// init the  Jacobsthal numbers with a_0 = 0 and a_1 = 1
	mJacobsthalNumbers.push_back(0);
	mJacobsthalNumbers.push_back(1);
	mJacobsthalNumbersTexCoordOffsets.push_back(glm::vec2(0.0f,0.0f));
	float w = (float)mHeightMap->getWidth();
	float h = (float)mHeightMap->getHeight();
	mJacobsthalNumbersTexCoordOffsets.push_back(glm::vec2( 1.0f / w, 1.0f / h));
	mJacobsthalNumbersTexCoordOffsets.push_back(glm::vec2( 0.5f / w, 0.5f / h));
}


int 
ClipmapTerrain::_getJacobsthalNumber( int clipmapLevel ) {
	while( mJacobsthalNumbers.size() < clipmapLevel ) // Jacobsthal sequence (or Jacobsthal numbers): a(n) = a(n-1) + 2a(n-2), with a(0) = 0, a(1) = 1
		mJacobsthalNumbers.push_back(mJacobsthalNumbers[mJacobsthalNumbers.size()-1] + (2 * mJacobsthalNumbers[mJacobsthalNumbers.size()-2]));
	return mJacobsthalNumbers[clipmapLevel-1];
}

glm::vec2 const& 
ClipmapTerrain::_getJacobsthalNumberTexCoordOffset(int clipmapLevel ) {
	float w = (float)mHeightMap->getWidth();
	float h = (float)mHeightMap->getHeight();
	while( mJacobsthalNumbersTexCoordOffsets.size() < clipmapLevel )
	{
		int divisor = (_getJacobsthalNumber(clipmapLevel-1)+_getJacobsthalNumber(clipmapLevel));
		mJacobsthalNumbersTexCoordOffsets.push_back(
		mJacobsthalNumbersTexCoordOffsets[mJacobsthalNumbersTexCoordOffsets.size()-2] + (glm::vec2( (1.0f / w) / divisor, (1.0f / h) / divisor)));
	}
	return mJacobsthalNumbersTexCoordOffsets[clipmapLevel-1];
}