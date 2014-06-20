#include "Terrain.h"

#include <G2/RenderComponent.h>
#include <glm/glm.hpp>
#include <vector>

using namespace G2::Terrain;

Terrain::Terrain(std::shared_ptr<G2::Texture> const& heightMap)
	: mHeightMap(heightMap),
	mClipmapWidth(50),
	mTileSize(0.1f)
{
	auto* renderComponent = mTerrainEntity.addComponent<G2::RenderComponent>();
	renderComponent->allocateVertexArrays(1);

	// allocate for each pixel one element in the VAO ?!? not really
	auto& vao = renderComponent->getVertexArray(0);
	
	unsigned int size =  4 * mClipmapWidth + 1; // dont understand this
	unsigned int numVertices = size * size;
	vao.resizeElementCount(numVertices);
	
	std::vector<glm::vec3> geometry(numVertices);
	std::vector<glm::vec2> texCoords(numVertices);

	
	for(unsigned int y = 0; y < size; ++y )
	{
		for(unsigned int x = 0; x < size; ++x ) 
		{
			glm::vec3& pos = geometry[y*size+x];
			glm::vec2& texCoord = texCoords[y*size+x];
			pos.x = -(x * mTileSize);
			pos.y = 0.0f; // height is calculated in shader
			pos.z = -(y * mTileSize);
			texCoord.s = x / (float)mHeightMap->getWidth();
			texCoord.t = y / (float)mHeightMap->getHeight();
		}
	}
	vao.writeData(G2::Semantics::POSITION, &geometry[0]);
	vao.writeData(G2::Semantics::TEXCOORD_0, &texCoords[0]);


}