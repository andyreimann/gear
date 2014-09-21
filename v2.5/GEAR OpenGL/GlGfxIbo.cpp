#include "GlGfxApi.h"
#include "GlGfxData.h"
#include "GlGfxMappings.h"

#include <G2/Logger.h>

G2Core::GfxResource* CreateIBO()
{
	return new G2GL::IndexBufferObjectResource(GL_INVALID_VALUE);
}

void UpdateIBOIndices(G2Core::GfxResource* ibo, unsigned int const* data, int numElements)
{
	G2GL::IndexBufferObjectResource* iboPtr = static_cast<G2GL::IndexBufferObjectResource*>(ibo);
	if(iboPtr->iboId == GL_INVALID_VALUE)
	{
		GLCHECK( glGenBuffers(1, &iboPtr->iboId) );
		GLCHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboPtr->iboId) );
		GLCHECK( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numElements, data, GL_STATIC_DRAW) );
		GLCHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
		return;
	}
	GLCHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboPtr->iboId) );
	GLCHECK( unsigned int* destination = (unsigned int*)glMapBuffer( GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY ) );
	memcpy(destination, data, sizeof(unsigned int) * numElements);
	GLCHECK( glUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER ) );
	GLCHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
}

void BindIBO(G2Core::GfxResource* ibo)
{
	G2GL::IndexBufferObjectResource* iboPtr = static_cast<G2GL::IndexBufferObjectResource*>(ibo);
	GLCHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboPtr->iboId) );
}

void UnbindIBO(G2Core::GfxResource* ibo)
{
	GLCHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
}

unsigned int* GetIBODataPointer(G2Core::GfxResource* ibo, G2Core::BufferAccessMode::Name mode)
{
	G2GL::IndexBufferObjectResource* iboPtr = static_cast<G2GL::IndexBufferObjectResource*>(ibo);
	GLCHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboPtr->iboId) );
	GLCHECK( unsigned int* destination = (unsigned int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, toGlBufferAccessMode(mode)) );
	GLCHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
	return destination;
}

void ReturnIBODataPointer(G2Core::GfxResource* ibo)
{
	G2GL::IndexBufferObjectResource* iboPtr = static_cast<G2GL::IndexBufferObjectResource*>(ibo);
	GLCHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboPtr->iboId) );
	GLCHECK( glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) );
	GLCHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
}

void DrawIBO(G2Core::GfxResource* ibo, G2Core::DrawMode::Name drawMode, int numIndices)
{
	GLCHECK( glDrawElements(toGlDrawMode(drawMode), numIndices, GL_UNSIGNED_INT, 0) );
}
