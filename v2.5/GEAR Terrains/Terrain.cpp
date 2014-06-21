#include "Terrain.h"

#include <G2/RenderComponent.h>
#include <G2/NameComponent.h>
#include <G2/Shader.h>
#include <glm/glm.hpp>
#include <vector>

using namespace G2::Terrain;

Terrain::Terrain(std::shared_ptr<G2::Texture> const& heightMap)
	: mHeightMap(heightMap),
	mClipmapWidth(50),
	mTileSize(0.1f),
	mGrid(64),
	mLodLevels(10)
{
	auto* nameComponent = mTerrainEntity.addComponent<G2::NameComponent>();
	nameComponent->name = "TerrainMesh";
	auto* renderComponent = mTerrainEntity.addComponent<G2::RenderComponent>();
	renderComponent->drawMode = GL_TRIANGLE_STRIP;
	renderComponent->setFaceCulling(G2::FaceCulling::FRONT_FACE);
	renderComponent->material.setTexture(G2::Sampler::SAMPLER_6, mHeightMap);
	
	unsigned int numVaos = 0;
	for(unsigned int l = 0; l < mLodLevels; ++l)
	{
		for(int i=-2; i<2; ++i)
		{
			for(int j=-2; j<2; ++j)
			{
				if(l>0) if(i==-1||i==0) if(j==-1||j==0) continue;
				++numVaos;
			}
		}
	}
	renderComponent->allocateVertexArrays(numVaos);
	
	float sxy=1; // scale x/y
	glm::vec2 viewpos(0.f,0.f);

	unsigned int vaoIndex = 0;
	for(unsigned int l = 0; l < mLodLevels; ++l)
	{
		float ox=(int(viewpos.x*(1<<l))&511)/float(512*mGrid);
		float oy=(int(viewpos.y*(1<<l))&511)/float(512*mGrid);
		
		glm::vec3 scale(sxy*0.25f,1.f,sxy*0.25f);

		// each level has 4x4 patches
		for(int i=-2; i<2; ++i)
		{
			for(int j=-2; j<2; ++j)
			{
				if(l>0) if(i==-1||i==0) if(j==-1||j==0) continue;

				glm::vec3 offset(ox+float(j),0.f,oy+float(i));

				if(i>=0) offset.z-=1.0/float(mGrid); // adjust offset for proper overlapping
				if(j>=0) offset.x-=1.0/float(mGrid); // adjust offset for proper overlapping


				std::vector<glm::vec3> geometry;
				// make vbo quad patch
				for(unsigned int z=0; z<mGrid+1; ++z)
				{
					for(unsigned int x=0; x<mGrid+2; ++x)
					{
						for(unsigned int k=0; k<((x==0) ? 2u : 1u); ++k)
						{
							geometry.push_back(scale*(offset+glm::vec3(float(x)/(float)mGrid,0.f,float(z)/(float)mGrid)));
						}
						++z;
						for(unsigned int k=0; k<((x==mGrid+1) ? 2u : 1u); ++k)
						{
							geometry.push_back(scale*(offset+glm::vec3(float(x)/(float)mGrid,0.f,float(z)/(float)mGrid)));
						}
						--z;
					}
				}
				auto& vao = renderComponent->getVertexArray(vaoIndex++);
				vao.resizeElementCount((unsigned int)geometry.size());
				vao.writeData(Semantics::POSITION, &geometry[0]);
			}
		}
		sxy*=2.f;
	}

	//mTerrainDrawModes[TerrainDrawStage::VIEWPORT_MESH] = GL_TRIANGLE_STRIP;
	//mTerrainDrawModes[TerrainDrawStage::MXM_BLOCK] = GL_TRIANGLE_STRIP;
	//mTerrainDrawModes[TerrainDrawStage::MXM_BLOCK] = GL_TRIANGLE_STRIP;
	//mTerrainDrawModes[TerrainDrawStage::MXM_BLOCK] = GL_TRIANGLE_STRIP;
	//mTerrainDrawModes[TerrainDrawStage::MXM_BLOCK] = GL_TRIANGLE_STRIP;

	//// listen to the draw calls of this RenderComponent
	//renderComponent->renderDrawCallEvent.hook(this, &Terrain::_onDrawCallEvent);

	//renderComponent->allocateVertexArrays(1);
	//renderComponent->drawMode = GL_TRIANGLE_STRIP;
	//// allocate for each pixel one element in the VAO ?!? not really
	//auto& vao = renderComponent->getVertexArray(0);
	//
	//unsigned int vdSize =  4 * mClipmapWidth + 1; // dont understand this
	//unsigned int numVertices = vdSize * vdSize;
	//vao.resizeElementCount(numVertices);
	//
	//std::vector<glm::vec3> geometry(numVertices);
	//std::vector<glm::vec2> texCoords(numVertices);

	//
	//for(unsigned int y = 0; y < vdSize; ++y )
	//{
	//	for(unsigned int x = 0; x < vdSize; ++x ) 
	//	{
	//		glm::vec3& pos = geometry[y*vdSize+x];
	//		glm::vec2& texCoord = texCoords[y*vdSize+x];
	//		pos.x = -(x * mTileSize);
	//		pos.y = 0.0f; // height is calculated in shader
	//		pos.z = -(y * mTileSize);
	//		texCoord.s = x / (float)mHeightMap->getWidth();
	//		texCoord.t = y / (float)mHeightMap->getHeight();
	//	}
	//}
	//vao.writeData(G2::Semantics::POSITION, &geometry[0]);
	//vao.writeData(G2::Semantics::TEXCOORD_0, &texCoords[0]);

	//renderComponent->allocateIndexArrays(0,2);

	//// create the index buffer for the viewport mesh in the middle (8x8 Block)
	//int size = 2 * mClipmapWidth + 1;
	//std::vector<unsigned int> indices((size-1) * (size*2 +2) - 2);
	//int i = 0;
	//int cnt = 0;
	//int startX = 0;
	//int startY = 0;
	//for( int y = startY; y < startY+size-1; ++y )
	//{
	//	for( int x = startX; x < startX+size; ++x ) 
	//	{
	//		if( startX == x && startY != y ) {
	//			i = x + (y * vdSize);
	//			indices[cnt++] = i;
	//		}

	//		i = x + (y * vdSize);
	//		indices[cnt++] = i;
	//			
	//		i = x + ((y+1) * vdSize);
	//		indices[cnt++] = i;

	//		if( startX+size-1 == x && (startY+size-2) != y ) {
	//			i = x + ((y+1) * vdSize);
	//			indices[cnt++] = i;
	//		}
	//	}
	//}
	//vao.writeIndices(0, &indices[0], indices.size());

	//// create the index buffer for the mxm block which starts at [0,0] in world coordinates
	//size = mClipmapWidth;
	////std::vector<unsigned int> indices((size-1) * (size*2 +2) - 2);
	//i = 0;
	//cnt = 0;
	//startX = 0;
	//startY = 0;
	//for( int y = startY; y < startY+size-1; ++y )
	//{
	//	for( int x = startX; x < startX+size; ++x ) 
	//	{
	//		if( startX == x && startY != y ) {
	//			i = x + (y * vdSize);
	//			indices[cnt++] = i;
	//		}

	//		i = x + (y * vdSize);
	//		indices[cnt++] = i;
	//			
	//		i = x + ((y+1) * vdSize);
	//		indices[cnt++] = i;

	//		if( startX+size-1 == x && (startY+size-2) != y ) {
	//			i = x + ((y+1) * vdSize);
	//			indices[cnt++] = i;
	//		}
	//	}
	//}
	//vao.writeIndices(1, &indices[0], indices.size());
}

//void
//Terrain::Terrain::_onDrawCallEvent(
//	G2::RenderComponent* renderComponent, 
//	unsigned int vertexArrayObjectIndex, 
//	unsigned int drawCallIndex, 
//	unsigned int& drawMode, 
//	glm::mat4 const& cameraSpaceMatrix, 
//	std::shared_ptr<Shader> const& boundShader,
//	bool& render) 
//{
//	if(drawCallIndex != TerrainDrawStage::VIEWPORT_MESH)
//	{
//		// depending on the stage, we may plan to omit certain draw calls
//	}
//}