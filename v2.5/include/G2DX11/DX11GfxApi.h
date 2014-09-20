#pragma once
#include "Defines.h"
#include "DX11GfxData.h"
#include <G2Core/GfxDevice.h>

void _initCgRuntime(ID3D11Device* device);
G2Core::GfxResource* error();
G2Core::GfxResource* make(G2DX11::Type type,G2Core::GfxResource* resource);
G2DX11::Type type(G2Core::GfxResource const* resource);
void release(G2Core::GfxResource* resource);

ID3D11Device* gDevicePtr();
ID3D11DeviceContext* gDeviceContextPtr();

extern "C"
{
	// General API
	GEARDX11DLL_API bool	Init(void* data);
	GEARDX11DLL_API void	Shutdown();
	GEARDX11DLL_API int		GfxVersion();
	GEARDX11DLL_API bool	SupportsShadingLanguage(std::string const& shadingLanguage);
	GEARDX11DLL_API void	ClearColor(glm::vec4 const& color);
	GEARDX11DLL_API void	SetViewport(G2::rect const& viewport);
	GEARDX11DLL_API void	ClearBuffers(G2Core::BufferFlags flags, G2Core::GfxResource* buffer);
	GEARDX11DLL_API void	FreeGfxResource(G2Core::GfxResource* resource);
	
	// Shader API
	GEARDX11DLL_API G2Core::GfxResource* CompileShader(G2Core::VertexInputLayout const& vertexInputLayout, std::string const& shadingLanguage, std::string const& vertexCode, std::string const& geometryCode, std::string const& fragmentCode);
	GEARDX11DLL_API void BindShader(G2Core::GfxResource const* shaderResource);
	GEARDX11DLL_API void SetShaderUniformMat4(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat4 const& value);
	GEARDX11DLL_API void SetShaderUniformMat3(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value);
	GEARDX11DLL_API void SetShaderUniformVec4(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value);
	GEARDX11DLL_API void SetShaderUniformVec3(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value);
	GEARDX11DLL_API void SetShaderUniformVec2(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value);
	GEARDX11DLL_API void SetShaderUniformFloat(G2Core::GfxResource* shaderResource, std::string const& property, float value);
	GEARDX11DLL_API void SetShaderUniformInt(G2Core::GfxResource* shaderResource, std::string const& property, int value);

	// VAO
	GEARDX11DLL_API G2Core::GfxResource* CreateVAO();
	GEARDX11DLL_API void UpdateVAOVertexBufferVec4(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec4 const* data, int numElements);
	GEARDX11DLL_API void UpdateVAOVertexBufferVec3(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec3 const* data, int numElements);
	GEARDX11DLL_API void UpdateVAOVertexBufferVec2(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec2 const* data, int numElements);
	GEARDX11DLL_API void BindVAO(G2Core::GfxResource* vao);
	GEARDX11DLL_API void UnbindVAO(G2Core::GfxResource* vao);
	
	GEARDX11DLL_API void* GetVaoDataPointer(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, G2Core::BufferAccessMode::Name mode);
	GEARDX11DLL_API void ReturnVaoDataPointer(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic);

	// Drawing
	GEARDX11DLL_API void DrawVAO(G2Core::GfxResource* vao, G2Core::GfxResource* alreadyboundShader, G2Core::DrawMode::Name drawMode, int numVertices);
};