#include "DX11GfxApi.h"
#include "DX11Globals.h"

#include <glm/ext.hpp>

#include <windowsx.h> 
#include <d3d11.h>		// core
#include <d3dx11.h>		// extensions
#include <d3dx10.h>		// dx 11 is ext of dx 10 -> useful macros usable

HWND					gHwnd;			// The window handle
IDXGISwapChain*			gSwapChain;		// the pointer to the swap chain interface
ID3D11Device*			gDevice;		// the pointer to our Direct3D device interface
ID3D11DeviceContext*	gDeviceContext; // the pointer to our Direct3D device context

// The mail render target
ID3D11RenderTargetView*	gBackbuffer;    // The back buffer
ID3D11DepthStencilView* gBackbufferDepthStencilView;
ID3D11Texture2D*		gBackbufferDepthStencilBuffer;

ID3D11Device* gDevicePtr()
{
	return gDevice;
}

ID3D11DeviceContext* gDeviceContextPtr()
{
	return gDeviceContext;
}

void _initCgRuntime();

G2Core::GfxResource* error()
{
	G2Core::GfxResource* res = new G2Core::GfxResource;
	res->valid = false;
	return res;
}

G2DX11::Type type(G2Core::GfxResource const* resource)
{
	if(resource->valid)
	{
		return static_cast<G2DX11::DX11Resource const*>(resource)->type;
	}
	return G2DX11::INVALID_RESOURCE;
}

void release(G2Core::GfxResource* resource)
{
	delete resource;
}

bool
Init(void* data) 
{
	D3DGlobals* globals = (D3DGlobals*)data;
	gHwnd = globals->hwnd;
	gSwapChain = globals->swapChain;
	gDevice = globals->device;
	gDeviceContext = globals->deviceContext;
	gBackbuffer = globals->backbuffer;
	gBackbufferDepthStencilView = globals->backbufferDepthStencilView;
	gBackbufferDepthStencilBuffer = globals->backbufferDepthStencilBuffer;

	// init CG Runtime
	_initCgRuntime(gDevice);

	return true;
}

void Shutdown()
{
	// Direct3D is actually incapable of closing when in full screen mode
	gSwapChain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

	// close and release all existing COM objects
	gSwapChain->Release();
	gBackbuffer->Release();
	gDevice->Release();
	gDeviceContext->Release();
}

int GfxVersion()
{
	return 1;
}

bool SupportsShadingLanguage(std::string const& shadingLanguage)
{
	return shadingLanguage == "CG";
}

void ClearColor(glm::vec4 const& color)
{
	gDeviceContext->ClearRenderTargetView(gBackbuffer, glm::value_ptr(color) );
}

void SetViewport(G2::rect const& vp) 
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = vp.x;
	viewport.TopLeftY = vp.y;
	viewport.Width = vp.z;
	viewport.Height = vp.w;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	gDeviceContext->RSSetViewports(1, &viewport);
}

void ClearBuffers(G2Core::BufferFlags flags, G2Core::GfxResource* buffer)
{
	if(buffer == nullptr)
	{
		int dxFlags = 0;
		dxFlags |= (flags & G2Core::Buffer::DEPTH) == G2Core::Buffer::DEPTH ? D3D11_CLEAR_DEPTH : 0;
		dxFlags |= (flags & G2Core::Buffer::STENCIL) == G2Core::Buffer::STENCIL ? D3D11_CLEAR_STENCIL : 0;

		gDeviceContext->ClearDepthStencilView(gBackbufferDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	else
	{
		assert(false);
	}
}

void FreeGfxResource(G2Core::GfxResource* resource)
{
	delete resource;
}

//void
//DX11GfxDevice::draw(void* vbo, int drawMode, int startVertex, int numVertices, unsigned int strideBytes, unsigned int offsetBytes) 
//{
//	ID3D11Buffer* vertexBuffer = static_cast<ID3D11Buffer*>(vbo);
//	
//	mDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &strideBytes, &offsetBytes);
//	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	assert(false); // FIX DRAWMODE!
//	mDeviceContext->Draw(numVertices, startVertex);    // draw 3 vertices, starting from vertex 0
//}
