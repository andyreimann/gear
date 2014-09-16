#pragma once
#include "Defines.h"
#include <G2Core/GfxDevice.h>

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
#include <windows.h>


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
};