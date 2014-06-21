#include "ClipmapTerrain.h"

#include <G2/RenderComponent.h>

using namespace G2::Terrain;

ClipmapTerrain::ClipmapTerrain()
	: mClipmapWidth(50),
	mTileSize(1.f) // only 1.f works
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
	mVertexArray.writeData(G2::Semantics::POSITION, &geometry[0]);
	mVertexArray.writeData(G2::Semantics::TEXCOORD_0, &texCoords[0]);

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
	mMMInstanceBlock.numIndices = mViewport.numIndices;
	//std::vector<unsigned int> indices((size-1) * (size*2 +2) - 2);
	i = 0;
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