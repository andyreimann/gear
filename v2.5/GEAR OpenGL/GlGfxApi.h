#pragma once
#include "Defines.h"
#include "GlGfxData.h"
#include <glm/glm.hpp>

#include <G2Core/GfxDevice.h>

void _initCgRuntime();
G2Core::GfxResource* error();
G2Core::GfxResource* make(G2GL::Type type,G2Core::GfxResource* resource);
G2GL::Type type(G2Core::GfxResource const* resource);
void release(G2Core::GfxResource* resource);

void releaseTextureCache();

extern "C"
{
	// General API
	GEAROPENGLDLL_API bool Init(void* data);
	GEAROPENGLDLL_API void Shutdown();
	GEAROPENGLDLL_API int  GfxVersion();
	GEAROPENGLDLL_API bool SupportsShadingLanguage(std::string const& shadingLanguage);
	GEAROPENGLDLL_API void ClearColor(glm::vec4 const& color);
	GEAROPENGLDLL_API void SetViewport(G2::rect const& viewport);
	GEAROPENGLDLL_API void ClearBuffers(G2Core::BufferFlags flags, G2Core::GfxResource* buffer);
	GEAROPENGLDLL_API void FreeGfxResource(G2Core::GfxResource* resource);
	GEAROPENGLDLL_API void UpdateRenderStates(G2Core::FaceCulling::Name cullFaceState, G2Core::PolygonDrawMode::Name polygonDrawMode, float polygonOffsetFactor, float polygonOffsetUnits, G2Core::BlendFactor::Name blendFuncSourceFactor, G2Core::BlendFactor::Name blendFuncDestinationFactor);
	GEAROPENGLDLL_API void GetPerspectiveProjection(glm::mat4& target, int width, int height, float zNear, float zFar, float fovY);
	GEAROPENGLDLL_API void AdjustCameraSpaceMatrix(glm::mat4& camSpaceMatrix);
	GEAROPENGLDLL_API void SetCullFaceEnabled(bool mode);
	GEAROPENGLDLL_API void SetDepthWritesEnabled(bool mode);
	GEAROPENGLDLL_API void SetDepthBias(bool enabled, float depthBias, float depthBiasClamp, float slopeScaledDepthBias);

	// ShaderAPI
	GEAROPENGLDLL_API G2Core::GfxResource* CompileShader(G2Core::VertexInputLayout const& vertexInputLayout, std::string const& shadingLanguage, std::string const& vertexCode, std::string const& geometryCode, std::string const& fragmentCode);
	GEAROPENGLDLL_API void BindShader(G2Core::GfxResource const* shaderResource);
	GEAROPENGLDLL_API void SetShaderUniformMat4(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat4 const& value);
	GEAROPENGLDLL_API void SetShaderUniformMat3(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value);
	GEAROPENGLDLL_API void SetShaderUniformVec4(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value);
	GEAROPENGLDLL_API void SetShaderUniformVec3(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value);
	GEAROPENGLDLL_API void SetShaderUniformVec2(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value);
	GEAROPENGLDLL_API void SetShaderUniformFloat(G2Core::GfxResource* shaderResource, std::string const& property, float value);
	GEAROPENGLDLL_API void SetShaderUniformInt(G2Core::GfxResource* shaderResource, std::string const& property, int value);
	
	// VAO
	GEAROPENGLDLL_API G2Core::GfxResource* CreateVAO();
	GEAROPENGLDLL_API void UpdateVAOVertexBufferVec4(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec4 const* data, int numElements);
	GEAROPENGLDLL_API void UpdateVAOVertexBufferVec3(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec3 const* data, int numElements);
	GEAROPENGLDLL_API void UpdateVAOVertexBufferVec2(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec2 const* data, int numElements);
	GEAROPENGLDLL_API void BindVAO(G2Core::GfxResource* vao, G2Core::GfxResource* alreadyboundShader);
	GEAROPENGLDLL_API void UnbindVAO(G2Core::GfxResource* vao);
	GEAROPENGLDLL_API void* GetVAODataPointer(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, G2Core::BufferAccessMode::Name mode);
	GEAROPENGLDLL_API void ReturnVAODataPointer(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic);
	
	// IndexBufferObject
	GEAROPENGLDLL_API G2Core::GfxResource* CreateIBO();
	GEAROPENGLDLL_API void UpdateIBOIndices(G2Core::GfxResource* ibo, unsigned int const* data, int numElements);
	GEAROPENGLDLL_API void BindIBO(G2Core::GfxResource* ibo);
	GEAROPENGLDLL_API void UnbindIBO(G2Core::GfxResource* ibo);
	GEAROPENGLDLL_API unsigned int* GetIBODataPointer(G2Core::GfxResource* ibo, G2Core::BufferAccessMode::Name mode);
	GEAROPENGLDLL_API void ReturnIBODataPointer(G2Core::GfxResource* ibo);
	
	// Drawing
	GEAROPENGLDLL_API void DrawVAO(G2Core::GfxResource* vao, G2Core::DrawMode::Name drawMode, int numVertices);
	GEAROPENGLDLL_API void DrawIBO(G2Core::GfxResource* ibo, G2Core::DrawMode::Name drawMode, int numIndices);

	// RenderTarget
	GEAROPENGLDLL_API G2Core::GfxResource* CreateRenderTarget(unsigned int width, unsigned int height, G2Core::DataFormat::Internal::Name format);
	GEAROPENGLDLL_API void AttachTextureToRenderTarget(G2Core::GfxResource* renderTarget, G2Core::GfxResource* texture, G2Core::FrameBufferAttachmentPoint::Name attachment, G2Core::TextureFormat::Name texTarget, int mipmapLevel, int layer);
	GEAROPENGLDLL_API void BindRenderTarget(G2Core::GfxResource* renderTarget);
	GEAROPENGLDLL_API void UnbindRenderTarget(G2Core::GfxResource* renderTarget);	

	// Texture
	GEAROPENGLDLL_API G2Core::GfxResource* CreateTexture2D(
		unsigned int width, 
		unsigned int height, 
		G2Core::DataFormat::Base::Name format, 
		G2Core::DataFormat::Internal::Name internalFormat, 
		G2Core::FilterMode::Name minFilter, 
		G2Core::FilterMode::Name magFilter, 
		G2Core::WrapMode::Name wrapS, 
		G2Core::WrapMode::Name wrapT, 
		G2Core::DataType::Name dataType, 
		void * data
	);
	GEAROPENGLDLL_API G2Core::GfxResource* CreateTexture2DArray(
		unsigned int width, 
		unsigned int height, 
		unsigned int size, 
		G2Core::DataFormat::Base::Name format, 
		G2Core::DataFormat::Internal::Name internalFormat, 
		G2Core::FilterMode::Name minFilter, 
		G2Core::FilterMode::Name magFilter,
		G2Core::WrapMode::Name wrapS, 
		G2Core::WrapMode::Name wrapT,
		G2Core::DataType::Name dataType,
		void * data
	);
	GEAROPENGLDLL_API G2Core::GfxResource* CreateTextureCube(
		unsigned int width, 
		unsigned int height, 
		G2Core::DataFormat::Base::Name format, 
		G2Core::DataFormat::Internal::Name internalFormat, 
		G2Core::FilterMode::Name minFilter, 
		G2Core::FilterMode::Name magFilter,
		G2Core::WrapMode::Name wrapS, 
		G2Core::WrapMode::Name wrapT
	);
	GEAROPENGLDLL_API G2Core::GfxResource* CreateTexture3D(
		unsigned int width, 
		unsigned int height, 
		unsigned int depth, 
		G2Core::DataFormat::Base::Name format, 
		G2Core::DataFormat::Internal::Name internalFormat, 
		G2Core::FilterMode::Name minFilter,
		G2Core::FilterMode::Name magFilter,
		G2Core::WrapMode::Name wrapS, 
		G2Core::WrapMode::Name wrapT, 
		G2Core::WrapMode::Name wrapR, 
		G2Core::DataType::Name dataType,
		void * data
	);
	GEAROPENGLDLL_API G2Core::GfxResource* CreateTexture2DFromFile(
		std::string const& fileName,
		G2Core::DataFormat::Internal::Name internalFormat,
		G2Core::FilterMode::Name minFilter,
		G2Core::FilterMode::Name magFilter,
		G2Core::WrapMode::Name wrapS,
		G2Core::WrapMode::Name wrapT,
		G2Core::TextureMetaData* metaData
	);


	GEAROPENGLDLL_API void BindTexture(G2Core::GfxResource* texResource, G2Core::TexSlot::Name texSlot);
	GEAROPENGLDLL_API void UnbindTexture(G2Core::GfxResource* texResource, G2Core::TexSlot::Name texSlot);
	
};