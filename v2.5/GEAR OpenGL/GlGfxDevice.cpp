#include "GlGfxDevice.h"

using namespace G2;

GlGfxDevice::GlGfxDevice() 
{
}

GlGfxDevice::~GlGfxDevice() 
{
}

void
GlGfxDevice::clearColor(glm::vec4 const& color) 
{
	GLCHECK( glClearColor(color.r,color.g,color.b,color.a) );
}

void
GlGfxDevice::draw(void* vbo, int drawMode, int startVertex, int numVertices, unsigned int strideBytes, unsigned int offsetBytes) 
{

}