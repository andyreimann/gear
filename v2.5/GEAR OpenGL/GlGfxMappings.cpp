#include "GlGfxApi.h"
#include "GlGfxData.h"

#include <G2/Logger.h>

#include <unordered_map>

std::unordered_map<G2Core::BufferAccessMode::Name,unsigned int> bufferAccessMapping;

void _initMappings()
{
	bufferAccessMapping[G2Core::BufferAccessMode::READ_ONLY] = GL_READ_ONLY;
	bufferAccessMapping[G2Core::BufferAccessMode::READ_WRITE] = GL_READ_WRITE;
	bufferAccessMapping[G2Core::BufferAccessMode::WRITE_ONLY] = GL_WRITE_ONLY;
}

unsigned int toGlBufferAccessMode(G2Core::BufferAccessMode::Name mode)
{
	return bufferAccessMapping[mode];
}