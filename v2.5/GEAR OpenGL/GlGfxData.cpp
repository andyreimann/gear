#include "GlGfxData.h"
#include "GlGfxApi.h"

using namespace G2GL;


G2GL::ShaderResource::ShaderResource(Type type) : GlResource(type)
{

}

G2GL::GlslShaderResource::~GlslShaderResource()
{
	if (programId != 0)
	{
		if (vertexShaderId != 0)
		{
			GLCHECK(glDetachShader(programId, vertexShaderId));
			GLCHECK(glDeleteShader(vertexShaderId));
		}
		if (geometryShaderId != 0)
		{
			GLCHECK(glDetachShader(programId, geometryShaderId));
			GLCHECK(glDeleteShader(geometryShaderId));
		}
		if (fragmentShaderId != 0)
		{
			GLCHECK(glDetachShader(programId, fragmentShaderId));
			GLCHECK(glDeleteShader(fragmentShaderId));
		}
		GLCHECK(glDeleteProgram(programId));
	}
}

int
G2GL::GlslShaderResource::getAndCacheUniformLocation(std::string const& name)
{
	auto it = uniformCache.find(name);
	if (it != uniformCache.end())
	{
		return it->second;
	}
	GLCHECK(int location = glGetUniformLocation(programId, name.c_str()));
	// here we also cache not found uniforms to not endless search for them
	uniformCache[name] = location;
	return location;
}


G2GL::VertexBufferObjectResource::~VertexBufferObjectResource()
{
	if (vboId != GL_INVALID_VALUE)
	{
		GLCHECK(glDeleteBuffers(1, &vboId));
	}
}


G2GL::VertexArrayObjectResource::~VertexArrayObjectResource()
{
	for (auto it = vbos.begin(); it != vbos.end(); ++it)
	{
		if (it->second != nullptr)
		{
			delete it->second;
		}
	}
	if (vaoId != GL_INVALID_VALUE)
	{
		GLCHECK(glDeleteVertexArrays(1, &vaoId));
	}
}


G2GL::IndexBufferObjectResource::~IndexBufferObjectResource()
{
	if (iboId != GL_INVALID_VALUE)
	{
		GLCHECK(glDeleteBuffers(1, &iboId));
	}
}


G2GL::RenderTargetResource::~RenderTargetResource()
{
	GLCHECK(glDeleteFramebuffers(1, &fboId));
	if (renderBufferId != GL_INVALID_VALUE)
	{
		GLCHECK(glDeleteRenderbuffers(1, &renderBufferId));
	}
}

TextureResource::~TextureResource()
{
	GLCHECK(glDeleteTextures(1, &texId));
}

G2GL::UniformBufferResource::UniformBufferResource(Type type, int uboId) :
	GlResource(type),
	uboId(uboId)
{}

G2GL::GlslUniformBufferResource::GlslUniformBufferResource(int uboId) :
	UniformBufferResource(GLSL_UBO, uboId)
{}

G2GL::GlslUniformBufferResource::~GlslUniformBufferResource()
{
	if (uboId != GL_INVALID_VALUE)
	{
		GLCHECK(glDeleteBuffers(1, &uboId));
	}
}