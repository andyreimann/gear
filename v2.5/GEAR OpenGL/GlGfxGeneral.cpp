#include "GlGfxApi.h"
#include "GlGfxData.h"
#include "GlGfxMappings.h"

#include <G2/Logger.h>

#include <glm/ext.hpp>

void _initCgRuntime();

G2Core::GfxResource* error()
{
	G2Core::GfxResource* res = new G2Core::GfxResource;
	res->valid = false;
	return res;
}

G2GL::Type type(G2Core::GfxResource const* resource)
{
	if(resource->valid)
	{
		return static_cast<G2GL::GlResource const*>(resource)->type;
	}
	return G2GL::INVALID_RESOURCE;
}

void release(G2Core::GfxResource* resource)
{
	delete resource;
}

bool
Init(void* data) 
{
	// ensure that all mappings of the GL Gfx API are initialized before doing anything!
	_initMappings();

	GLenum GlewInitResult;

	glewExperimental = GL_TRUE;
	GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult)
	{
		G2::logger << "ERROR: " << glewGetErrorString(GlewInitResult) << G2::endl;
		return false;
	}
		
	GLCHECK( G2::logger << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << G2::endl );

	// enable transparency and so on
	GLCHECK( glEnable(GL_BLEND) );
	GLCHECK( glEnable(GL_DEPTH_TEST) );
	GLCHECK( glEnable(GL_MULTISAMPLE) );
	GLCHECK( glEnable(GL_CULL_FACE) );
	GLCHECK( glEnable(GL_BLEND) );
	int max;
	GLCHECK( glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max));
	G2::logger << "Info: GL_MAX_VERTEX_ATTRIBS=" << max << G2::endl;
	
	GLint bufs;
	GLint samples;
	GLCHECK( glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs) );
	GLCHECK( glGetIntegerv(GL_SAMPLES, &samples) );

	G2::logger << "Have " << bufs << " buffers and " << samples << " samples" << G2::endl;

	// init CG Runtime
	_initCgRuntime();
	return true;
}

void Shutdown()
{

}

int GfxVersion()
{
	return 1;
}

void ClearColor(glm::vec4 const& color)
{
	GLCHECK( glClearColor(color.r,color.g,color.b,color.a) );
}

void
SetViewport(G2::rect const& viewport) 
{
	GLCHECK( glViewport(viewport.x, viewport.y, viewport.z, viewport.w) );
}

bool SupportsShadingLanguage(std::string const& shadingLanguage)
{
	return shadingLanguage == "GLSL" || shadingLanguage == "CG";
}

void
ClearBuffers(G2Core::BufferFlags flags, G2Core::GfxResource* buffer) 
{
	if(buffer != nullptr)
	{
		assert(buffer->valid);
	}
	int glFlags = 0;
	glFlags |= (flags & G2Core::Buffer::COLOR) == G2Core::Buffer::COLOR ? GL_COLOR_BUFFER_BIT : 0;
	glFlags |= (flags & G2Core::Buffer::DEPTH) == G2Core::Buffer::DEPTH ? GL_DEPTH_BUFFER_BIT : 0;
	glFlags |= (flags & G2Core::Buffer::STENCIL) == G2Core::Buffer::STENCIL ? GL_STENCIL_BUFFER_BIT : 0;
	GLCHECK( glClear(glFlags) );
}

void FreeGfxResource(G2Core::GfxResource* resource)
{
	release(resource);
}