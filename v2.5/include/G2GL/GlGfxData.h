#pragma once

#include "Defines.h"

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
		IBO,
		FBO,
		TEX_2D,
		TEX_2D_ARRAY,
		TEX_CUBE,
		TEX_3D,
	};
	struct GlResource : G2Core::GfxResource
	{
		GlResource(Type type) : type(type) {}
		virtual ~GlResource() {}
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
		ShaderResource(Type type);
		virtual ~ShaderResource() {}

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
		CgShaderResource();
		~CgShaderResource();
		/** This function will try to cache and get the location of the 
		 * uniform variable for the given name.
		 * @param name The name of the uniform to get the location for.
		 * @return The location of the uniform or -1 if it could not be found.
		 */
		std::pair<CGparameter,CGprogram> const& getAndCacheUniformLocation(std::string const& name);

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
		~GlslShaderResource();
		/** This function will try to cache and get the location of the 
		 * uniform variable for the given name.
		 * @param name The name of the uniform to get the location for.
		 * @return The location of the uniform or -1 if it could not be found.
		 */
		int getAndCacheUniformLocation(std::string const& name);
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

		~VertexBufferObjectResource();

		unsigned int vboId;
	};

	struct VertexArrayObjectResource : GlResource
	{
		VertexArrayObjectResource(unsigned int vaoId) 
			: vaoId(vaoId), 
			GlResource(G2GL::VAO) {}

		~VertexArrayObjectResource();

		unsigned int vaoId;
		std::unordered_map<G2Core::Semantics::Name,VertexBufferObjectResource*> vbos; // the contained VertexBufferObjects

	};

	struct IndexBufferObjectResource : GlResource
	{
		IndexBufferObjectResource(unsigned int iboId) 
			: iboId(iboId), 
			GlResource(G2GL::IBO) {}

		~IndexBufferObjectResource();

		unsigned int iboId;
	};

	struct RenderTargetResource : GlResource
	{
		RenderTargetResource(unsigned int width, unsigned int height, G2Core::DataFormat::Internal::Name format, unsigned int fboId, unsigned int renderBufferId) 
			: GlResource(FBO),
			width(width),
			height(height),
			format(format),
			fboId(fboId),
			renderBufferId(renderBufferId) {}

		~RenderTargetResource();

		unsigned int width;
		unsigned int height;
		G2Core::DataFormat::Internal::Name format;
		unsigned int fboId;
		unsigned int renderBufferId;
	};

	struct TextureResource : GlResource
	{
		TextureResource(Type type, unsigned int texId, unsigned int texType, unsigned int dataFormat, unsigned int minFilter, unsigned int magFilter) 
			: GlResource(type),
			texId(texId),
			texType(texType),
			dataFormat(dataFormat),
			minFilter(minFilter),
			magFilter(magFilter) {}

		virtual ~TextureResource();
		
		unsigned int texId;
		unsigned int texType;
		unsigned int dataFormat;
		unsigned int minFilter;
		unsigned int magFilter;
	};

	struct Texture2DResource : TextureResource
	{
		Texture2DResource(
			unsigned int texId,
			unsigned int dataFormat, 
			unsigned int minFilter, 
			unsigned int magFilter, 
			unsigned int wrapS, 
			unsigned int wrapT) 
			: TextureResource(TEX_2D,texId,GL_TEXTURE_2D,dataFormat,minFilter,magFilter),
			wrapS(wrapS),
			wrapT(wrapT) {}
		
		unsigned int wrapS;
		unsigned int wrapT;
	};

	struct Texture2DArrayResource : TextureResource
	{
		Texture2DArrayResource(
			unsigned int texId,
			unsigned int dataFormat, 
			unsigned int minFilter, 
			unsigned int magFilter, 
			unsigned int wrapS, 
			unsigned int wrapT) 
			: TextureResource(TEX_2D_ARRAY,texId,GL_TEXTURE_2D_ARRAY,dataFormat,minFilter,magFilter),
			wrapS(wrapS),
			wrapT(wrapT) {}
		
		unsigned int wrapS;
		unsigned int wrapT;
	};

	struct TextureCubeResource : TextureResource
	{
		TextureCubeResource(
			unsigned int texId,
			unsigned int dataFormat, 
			unsigned int minFilter, 
			unsigned int magFilter, 
			unsigned int wrapS, 
			unsigned int wrapT) 
			: TextureResource(TEX_CUBE,texId,GL_TEXTURE_CUBE_MAP,dataFormat,minFilter,magFilter),
			wrapS(wrapS),
			wrapT(wrapT) {}
		
		unsigned int wrapS;
		unsigned int wrapT;
	};

	struct Texture3DResource : TextureResource
	{
		Texture3DResource(
			unsigned int texId,
			unsigned int dataFormat, 
			unsigned int minFilter, 
			unsigned int magFilter, 
			unsigned int wrapS, 
			unsigned int wrapT, 
			unsigned int wrapR) 
			: TextureResource(TEX_3D,texId,GL_TEXTURE_3D,dataFormat,minFilter,magFilter),
			wrapS(wrapS),
			wrapT(wrapT),
			wrapR(wrapR) {}
		
		unsigned int wrapS;
		unsigned int wrapT;
		unsigned int wrapR;
	};

};