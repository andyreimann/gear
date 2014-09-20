#pragma once
#include "GlGfxApi.h"

#include <Cg/cgGL.h>

#include <G2Core/GfxDevice.h>

#include <unordered_map>

namespace G2GL
{
	enum Type 
	{
		INVALID_RESOURCE = 0,
		GLSL_SHADER,
		CG_SHADER,
		VAO,
		VBO,
	};
	struct GlResource : G2Core::GfxResource
	{
		GlResource(Type type) : type(type) {}
		Type type;
	};

	typedef void (*BindShader)(G2Core::GfxResource const* shaderResource);
	typedef void (*SetShaderUniformMat4)(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat4 const& value);
	typedef void (*SetShaderUniformMat3)(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value);
	typedef void (*SetShaderUniformVec4)(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value);
	typedef void (*SetShaderUniformVec3)(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value);
	typedef void (*SetShaderUniformVec2)(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value);
	typedef void (*SetShaderUniformFloat)(G2Core::GfxResource* shaderResource, std::string const& property, float value);
	typedef void (*SetShaderUniformInt)(G2Core::GfxResource* shaderResource, std::string const& property, int value);
	typedef void (*FreeGfxResource)(G2Core::GfxResource* resource);

	struct ShaderResource : GlResource
	{
		ShaderResource(Type type) : GlResource(type) {}

		BindShader bindShader;
		SetShaderUniformMat4 setShaderUniformMat4;
		SetShaderUniformMat3 setShaderUniformMat3;
		SetShaderUniformVec4 setShaderUniformVec4;
		SetShaderUniformVec3 setShaderUniformVec3;
		SetShaderUniformVec2 setShaderUniformVec2;
		SetShaderUniformFloat setShaderUniformFloat;
		SetShaderUniformInt setShaderUniformInt;
		FreeGfxResource freeGfxResource;
	};

	struct CgShaderResource : ShaderResource
	{
		CgShaderResource()
			: vertexShaderId(nullptr),
			geometryShaderId(nullptr),
			fragmentShaderId(nullptr),
			ShaderResource(G2GL::CG_SHADER) {}
		~CgShaderResource()
		{
			cgDestroyProgram(vertexShaderId);
			if(geometryShaderId != nullptr)
			{
				cgDestroyProgram(geometryShaderId);
			}
			cgDestroyProgram(fragmentShaderId);
		}
		/** This function will try to cache and get the location of the 
		 * uniform variable for the given name.
		 * @param name The name of the uniform to get the location for.
		 * @return The location of the uniform or -1 if it could not be found.
		 */
		std::pair<CGparameter,CGprogram> const& getAndCacheUniformLocation(std::string const& name)
		{
			auto it = uniformCache.find(name);
			if(it != uniformCache.end()) 
			{
				return it->second;
			}

			CGparameter location = cgGetNamedParameter(vertexShaderId, name.c_str());
			if(location == nullptr) 
			{
				location = cgGetNamedParameter(fragmentShaderId, name.c_str());
				uniformCache[name] = std::make_pair(location,fragmentShaderId);
			}
			else 
			{
				uniformCache[name] = std::make_pair(location,vertexShaderId);
			}
			return uniformCache[name];
		}

		CGprogram vertexShaderId;
		CGprogram geometryShaderId;
		CGprogram fragmentShaderId;
		std::unordered_map<std::string,std::pair<CGparameter,CGprogram>> uniformCache;	// The cache for the uniform locations
	};

	struct GlslShaderResource : ShaderResource
	{
		GlslShaderResource()
			: vertexShaderId(0),
			geometryShaderId(0),
			fragmentShaderId(0),
			programId(0),
			ShaderResource(G2GL::GLSL_SHADER) {}
		~GlslShaderResource()
		{
			GLCHECK( glDetachShader(programId, vertexShaderId) );
			GLCHECK( glDetachShader(programId, geometryShaderId) );
			GLCHECK( glDetachShader(programId, fragmentShaderId) );
	
			GLCHECK( glDeleteShader(fragmentShaderId) );
			GLCHECK( glDeleteShader(geometryShaderId) );
			GLCHECK( glDeleteShader(vertexShaderId) );

			GLCHECK( glDeleteProgram(programId) );
		}
		/** This function will try to cache and get the location of the 
		 * uniform variable for the given name.
		 * @param name The name of the uniform to get the location for.
		 * @return The location of the uniform or -1 if it could not be found.
		 */
		int getAndCacheUniformLocation(std::string const& name)
		{
			auto it = uniformCache.find(name);
			if(it != uniformCache.end()) 
			{
				return it->second;
			}
			GLCHECK( int location = glGetUniformLocation(programId, name.c_str()) );
			// here we also cache not found uniforms to not endless search for them
			uniformCache[name] = location;
			return location;
		}
		int			vertexShaderId;	// The vertex shader id
		int			geometryShaderId;	// The geometry shader id
		int			fragmentShaderId;	// The fragment shader id
		int			programId;			// The program id
		std::unordered_map<std::string,int> uniformCache; // The cache of all uniform locations
	};

	struct VertexBufferObjectResource : GlResource
	{
		VertexBufferObjectResource(unsigned int vboId) 
			: vboId(vboId), 
			GlResource(G2GL::VBO) {}

		~VertexBufferObjectResource()
		{
			if(vboId != GL_INVALID_VALUE)
			{
				GLCHECK( glDeleteBuffers(1, &vboId) );
			}
		}

		unsigned int vboId;
	};

	struct VertexArrayObjectResource : GlResource
	{
		VertexArrayObjectResource(unsigned int vaoId) 
			: vaoId(vaoId), 
			GlResource(G2GL::VAO) {}

		~VertexArrayObjectResource()
		{
			for(auto it = vbos.begin(); it != vbos.end(); ++it)
			{
				if(it->second != nullptr)
				{
					delete it->second;
				}
			}
			if(vaoId != GL_INVALID_VALUE)
			{
				GLCHECK( glDeleteVertexArrays(1, &vaoId) );
			}
		}

		unsigned int vaoId;
		std::unordered_map<G2Core::Semantics::Name,VertexBufferObjectResource*> vbos; // the contained VertexBufferObjects

	};

};