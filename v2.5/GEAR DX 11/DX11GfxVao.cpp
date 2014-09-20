#include "DX11GfxApi.h"
#include "DX11GfxData.h"

#include <G2/Logger.h>

G2Core::GfxResource* CreateVAO()
{
	return nullptr;
} 

void UpdateVAOVertexBufferVec4(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec4 const* data, int numElements)
{
	return;
}

void UpdateVAOVertexBufferVec3(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec3 const* data, int numElements)
{
	return;
}

void UpdateVAOVertexBufferVec2(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec2 const* data, int numElements)
{
	return;
}

void BindVAO(G2Core::GfxResource* vao)
{
	return;
}

void UnbindVAO(G2Core::GfxResource* vao)
{
	return;
}

void DrawVAO(G2Core::GfxResource* vao, G2Core::GfxResource* alreadyboundShader, G2Core::DrawMode::Name drawMode, int numVertices)
{
	return;
}


	
void* GetVaoDataPointer(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, G2Core::BufferAccessMode::Name mode)
{
	return nullptr;
}

void ReturnVaoDataPointer(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic)
{
	return;
}