#include "GlGfxData.h"
#include "GlGfxApi.h"

using namespace G2GL;


G2GL::ShaderResource::ShaderResource(Type type) : GlResource(type)
{

}


G2GL::CgShaderResource::CgShaderResource() : vertexShaderId(nullptr),
geometryShaderId(nullptr),
fragmentShaderId(nullptr),
ShaderResource(G2GL::CG_SHADER)
{

}


G2GL::CgShaderResource::~CgShaderResource()
{
	cgDestroyProgram(vertexShaderId);
	if (geometryShaderId != nullptr)
	{
		cgDestroyProgram(geometryShaderId);
	}
	cgDestroyProgram(fragmentShaderId);
}

std::pair<CGparameter, CGprogram> const&
G2GL::CgShaderResource::getAndCacheUniformLocation(std::string const& name)
{
	auto it = uniformCache.find(name);
	if (it != uniformCache.end())
	{
		return it->second;
	}

	CGparameter location = cgGetNamedParameter(vertexShaderId, name.c_str());
	if (location == nullptr)
	{
		location = cgGetNamedParameter(fragmentShaderId, name.c_str());
		uniformCache[name] = std::make_pair(location, fragmentShaderId);
	}
	else
	{
		uniformCache[name] = std::make_pair(location, vertexShaderId);
	}
	return uniformCache[name];
}


G2GL::GlslShaderResource::~GlslShaderResource()
{
	GLCHECK(glDetachShader(programId, vertexShaderId));
	GLCHECK(glDetachShader(programId, geometryShaderId));
	GLCHECK(glDetachShader(programId, fragmentShaderId));

	GLCHECK(glDeleteShader(fragmentShaderId));
	GLCHECK(glDeleteShader(geometryShaderId));
	GLCHECK(glDeleteShader(vertexShaderId));

	GLCHECK(glDeleteProgram(programId));
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

G2GL::UniformBufferResource::UniformBufferResource(Type type) :
	GlResource(type)
{}

G2GL::GlslUniformBufferResource::GlslUniformBufferResource(int uboId) :
	UniformBufferResource(GLSL_UBO),
	uboId(uboId)
{}

G2GL::GlslUniformBufferResource::~GlslUniformBufferResource()
{
	if (uboId != GL_INVALID_VALUE)
	{
		GLCHECK(glDeleteBuffers(1, &uboId));
	}
}

G2GL::CgUniformBufferResource::CgUniformBufferResource(int glUboId, CGbuffer cgUboId) :
	UniformBufferResource(CG_UBO),
	glUboId(glUboId),
	cgUboId(cgUboId)
{}

G2GL::CgUniformBufferResource::~CgUniformBufferResource()
{
	//if (glUboId != GL_INVALID_VALUE)
	//{
	//	GLCHECK(glDeleteBuffers(1, &glUboId));
	//}
	if (cgUboId != nullptr)
	{
		// should destroy the GL buffer as well :)
		cgDestroyBuffer(cgUboId);
	}
}