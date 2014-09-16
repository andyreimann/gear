#pragma once
#include "Defines.h"
#include "DX11GfxData.h"
#include <G2Core/GfxDevice.h>

void _initCgRuntime();
G2Core::GfxResource* error();
G2Core::GfxResource* make(G2DX11::Type type,G2Core::GfxResource* resource);
G2DX11::Type type(G2Core::GfxResource const* resource);
void release(G2Core::GfxResource* resource);

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
	
	GEARDX11DLL_API void BindShader(G2Core::GfxResource const* shaderResource);
	GEARDX11DLL_API void SetShaderUniformMat4(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat4 const& value);
	GEARDX11DLL_API void SetShaderUniformMat3(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value);
	GEARDX11DLL_API void SetShaderUniformVec4(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value);
	GEARDX11DLL_API void SetShaderUniformVec3(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value);
	GEARDX11DLL_API void SetShaderUniformVec2(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value);
	GEARDX11DLL_API void SetShaderUniformFloat(G2Core::GfxResource* shaderResource, std::string const& property, float value);
	GEARDX11DLL_API void SetShaderUniformInt(G2Core::GfxResource* shaderResource, std::string const& property, int value);
};