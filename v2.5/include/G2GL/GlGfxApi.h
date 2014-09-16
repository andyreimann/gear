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

	// ShaderAPI
	GEAROPENGLDLL_API G2Core::GfxResource* CompileShader(std::string const& shadingLanguage, std::string const& vertexCode, std::string const& geometryCode, std::string const& fragmentCode);
	GEAROPENGLDLL_API void BindShader(G2Core::GfxResource const* shaderResource);
	GEAROPENGLDLL_API void SetShaderUniformMat4(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat4 const& value);
	GEAROPENGLDLL_API void SetShaderUniformMat3(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value);
	GEAROPENGLDLL_API void SetShaderUniformVec4(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value);
	GEAROPENGLDLL_API void SetShaderUniformVec3(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value);
	GEAROPENGLDLL_API void SetShaderUniformVec2(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value);
	GEAROPENGLDLL_API void SetShaderUniformFloat(G2Core::GfxResource* shaderResource, std::string const& property, float value);
	GEAROPENGLDLL_API void SetShaderUniformInt(G2Core::GfxResource* shaderResource, std::string const& property, int value);
	
};