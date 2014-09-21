#include "GlGfxApi.h"
#include "GlGfxData.h"
#include "GlGfxMappings.h"

#include <G2/Logger.h>

G2Core::GfxResource* CreateVAO()
{
	unsigned int vaoId = GL_INVALID_VALUE;
	GLCHECK( glGenVertexArrays(1, &vaoId) );
	G2GL::VertexArrayObjectResource* resource = new G2GL::VertexArrayObjectResource(vaoId);
	return resource;
} 

void UpdateVAOVertexBufferVec4(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec4 const* data, int numElements)
{
	G2GL::VertexArrayObjectResource* vaoPtr = static_cast<G2GL::VertexArrayObjectResource*>(vao);
	int bytes = sizeof(float) * 4;
	if(vaoPtr->vbos[semantic] == nullptr)
	{
		// add new VertexBufferObject to this VertexArrayObject
		unsigned int vboId;
		GLCHECK( glGenBuffers(1, &vboId) );
		GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, vboId) );
		GLCHECK( glBufferData(GL_ARRAY_BUFFER, bytes * numElements, data, GL_STATIC_DRAW) );
		GLCHECK( glVertexAttribPointer(semantic, 4, GL_FLOAT, GL_FALSE, 0, 0) );
		GLCHECK( glEnableVertexAttribArray(semantic) );
		G2GL::VertexBufferObjectResource* vboRes = new G2GL::VertexBufferObjectResource(vboId);
		vaoPtr->vbos[semantic] = vboRes;
		return;
	}
	G2GL::VertexBufferObjectResource* vboPtr = static_cast<G2GL::VertexBufferObjectResource*>(vaoPtr->vbos[semantic]);
	GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, vboPtr->vboId) );
	GLCHECK( glBufferData(GL_ARRAY_BUFFER, bytes * numElements, data, GL_STATIC_DRAW) );
	GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, 0) );
}

void UpdateVAOVertexBufferVec3(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec3 const* data, int numElements)
{
	G2GL::VertexArrayObjectResource* vaoPtr = static_cast<G2GL::VertexArrayObjectResource*>(vao);
	int bytes = sizeof(float) * 3;
	if(vaoPtr->vbos[semantic] == nullptr)
	{
		// add new VertexBufferObject to this VertexArrayObject
		unsigned int vboId;
		GLCHECK( glGenBuffers(1, &vboId) );
		GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, vboId) );
		GLCHECK( glBufferData(GL_ARRAY_BUFFER, bytes * numElements, data, GL_STATIC_DRAW) );
		GLCHECK( glVertexAttribPointer(semantic, 3, GL_FLOAT, GL_FALSE, 0, 0) );
		GLCHECK( glEnableVertexAttribArray(semantic) );
		G2GL::VertexBufferObjectResource* vboRes = new G2GL::VertexBufferObjectResource(vboId);
		vaoPtr->vbos[semantic] = vboRes;
		return;
	}
	G2GL::VertexBufferObjectResource* vboPtr = static_cast<G2GL::VertexBufferObjectResource*>(vaoPtr->vbos[semantic]);
	GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, vboPtr->vboId) );
	GLCHECK( glBufferData(GL_ARRAY_BUFFER, bytes * numElements, data, GL_STATIC_DRAW) );
	GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, 0) );
}

void UpdateVAOVertexBufferVec2(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec2 const* data, int numElements)
{
	G2GL::VertexArrayObjectResource* vaoPtr = static_cast<G2GL::VertexArrayObjectResource*>(vao);
	int bytes = sizeof(float) * 2;
	if(vaoPtr->vbos[semantic] == nullptr)
	{
		// add new VertexBufferObject to this VertexArrayObject
		unsigned int vboId;
		GLCHECK( glGenBuffers(1, &vboId) );
		GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, vboId) );
		GLCHECK( glBufferData(GL_ARRAY_BUFFER, bytes * numElements, data, GL_STATIC_DRAW) );
		GLCHECK( glVertexAttribPointer(semantic, 2, GL_FLOAT, GL_FALSE, 0, 0) );
		GLCHECK( glEnableVertexAttribArray(semantic) );
		G2GL::VertexBufferObjectResource* vboRes = new G2GL::VertexBufferObjectResource(vboId);
		vaoPtr->vbos[semantic] = vboRes;
		return;
	}
	G2GL::VertexBufferObjectResource* vboPtr = static_cast<G2GL::VertexBufferObjectResource*>(vaoPtr->vbos[semantic]);
	GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, vboPtr->vboId) );
	GLCHECK( glBufferData(GL_ARRAY_BUFFER, bytes * numElements, data, GL_STATIC_DRAW) );
	GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, 0) );
}

void BindVAO(G2Core::GfxResource* vao, G2Core::GfxResource* alreadyboundShader)
{
	GLCHECK( glBindVertexArray(static_cast<G2GL::VertexArrayObjectResource*>(vao)->vaoId) );
}

void UnbindVAO(G2Core::GfxResource* vao)
{
	GLCHECK( glBindVertexArray(0) );
}

void DrawVAO(G2Core::GfxResource* vao, G2Core::DrawMode::Name drawMode, int numVertices)
{
	GLCHECK( glDrawArrays(toGlDrawMode(drawMode), 0, numVertices) );
}


	
void* GetVAODataPointer(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, G2Core::BufferAccessMode::Name mode)
{
	G2GL::VertexArrayObjectResource* vaoPtr = static_cast<G2GL::VertexArrayObjectResource*>(vao);
	
	if(vaoPtr->vbos[semantic] == nullptr)
	{
		return nullptr;
	}
	G2GL::VertexBufferObjectResource* vboPtr = static_cast<G2GL::VertexBufferObjectResource*>(vaoPtr->vbos[semantic]);
	
	GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, vboPtr->vboId) );
	GLCHECK( void* dest = (void*)glMapBuffer( GL_ARRAY_BUFFER, toGlBufferAccessMode(mode) ) );
	GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, 0) );
	return dest;
}

void ReturnVAODataPointer(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic)
{
	G2GL::VertexArrayObjectResource* vaoPtr = static_cast<G2GL::VertexArrayObjectResource*>(vao);
	
	if(vaoPtr->vbos[semantic] == nullptr)
	{
		return;
	}
	G2GL::VertexBufferObjectResource* vboPtr = static_cast<G2GL::VertexBufferObjectResource*>(vaoPtr->vbos[semantic]);
	
	GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, vboPtr->vboId) );
	GLCHECK( glUnmapBuffer( GL_ARRAY_BUFFER ) );
	GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, 0) );
}