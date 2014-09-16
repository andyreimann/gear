#include "DX11GfxApi.h"

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
ID3D11Texture2D* gBackbufferDepthStencilBuffer;

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
	HWND hwnd = *(HWND*)data;
		// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hwnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching
	
	
	D3D_FEATURE_LEVEL featureLevels     = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL *pFeatureLevel    = NULL;

	D3D_DRIVER_TYPE driverTypes[] = 
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	unsigned int num = sizeof( driverTypes ) / sizeof( driverTypes[0] );

	
	for( UINT driverTypeIndex = 0; driverTypeIndex < num; driverTypeIndex++ )
	{
		HRESULT hr = D3D11CreateDeviceAndSwapChain( NULL,							// Adapter
													driverTypes[driverTypeIndex],   // Driver Type
													NULL,							// Software
													D3D11_CREATE_DEVICE_DEBUG,		// Flags
													&featureLevels,					// Feature Levels
													1,								// num Feature Levels
													D3D11_SDK_VERSION,				// SDK Version
													&scd,
													&gSwapChain,
													&gDevice,
													pFeatureLevel,
													&gDeviceContext);

		if( SUCCEEDED( hr ) )
			break;
	}

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	gDevice->CreateRenderTargetView(pBackBuffer, NULL, &gBackbuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	gDeviceContext->OMSetRenderTargets(1, &gBackbuffer, NULL);

	// set up a depth/stencil buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	// TODO Where to get the data from?
	depthStencilDesc.Width     = 1024;
	depthStencilDesc.Height    = 768;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	//Create the actual view
	gDevice->CreateTexture2D(&depthStencilDesc, NULL, &gBackbufferDepthStencilBuffer);
	gDevice->CreateDepthStencilView(gBackbufferDepthStencilBuffer, NULL, &gBackbufferDepthStencilView);

	// bind it to the render target (=backbuffer)
	gDeviceContext->OMSetRenderTargets( 1, &gBackbuffer, gBackbufferDepthStencilView );

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
	return false;
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
