#include "DX11GfxApi.h"
#include "DX11Globals.h"
#include "DX11GfxMappings.h"

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

// The rasterizer
D3D11_RASTERIZER_DESC gRasterizerStateDesc;
ID3D11RasterizerState* gRasterizerState;

D3D11_RASTERIZER_DESC& gGetRasterizerStateDescr()
{
	return gRasterizerStateDesc;
}

void gUpdateRasterizer()
{
	//gRasterizerStateDesc.CullMode = D3D11_CULL_NONE;
	//gRasterizerStateDesc.FrontCounterClockwise = false;
	HRESULT hr = gDevice->CreateRasterizerState( &gRasterizerStateDesc, &gRasterizerState );
	//
	gDeviceContext->RSSetState(gRasterizerState);
}

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
	_initMappings();
	D3DGlobals* globals = (D3DGlobals*)data;
	gHwnd = globals->hwnd;
	gSwapChain = globals->swapChain;
	gDevice = globals->device;
	gDeviceContext = globals->deviceContext;
	gBackbuffer = globals->backbuffer;
	gBackbufferDepthStencilView = globals->backbufferDepthStencilView;
	gBackbufferDepthStencilBuffer = globals->backbufferDepthStencilBuffer;

	gRasterizerStateDesc.FillMode = D3D11_FILL_WIREFRAME;
	gRasterizerStateDesc.CullMode = D3D11_CULL_NONE;
	gRasterizerStateDesc.FrontCounterClockwise = false;

	gUpdateRasterizer();

	gDeviceContext->RSSetState(gRasterizerState);
	// init CG Runtime
	_initCgRuntime(gDevice);

	return true;
}

void test()
{
	

		/*ID3D11RasterizerState* ras;
		gDeviceContextPtr()->RSGetState(&ras);
		ras->GetDesc();
		ras->*/
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
	//viewport.MinDepth = 0.f;
	//viewport.MaxDepth = 1.f;

	gDeviceContext->RSSetViewports(1, &viewport);
}

void ClearBuffers(G2Core::BufferFlags flags, G2Core::GfxResource* buffer)
{
	if(buffer == nullptr)
	{
		int dxFlags = 0;
		dxFlags |= (flags & G2Core::Buffer::DEPTH) == G2Core::Buffer::DEPTH ? D3D11_CLEAR_DEPTH : 0;
		dxFlags |= (flags & G2Core::Buffer::STENCIL) == G2Core::Buffer::STENCIL ? D3D11_CLEAR_STENCIL : 0;
		// LH Coordinate System
		//gDeviceContext->ClearDepthStencilView(gBackbufferDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		// RH Coordinate System (=OpenGL)
		gDeviceContext->ClearDepthStencilView(gBackbufferDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	else
	{
		assert(false);
	}
}

void UpdateRenderStates(G2Core::FaceCulling::Name cullFaceState, G2Core::PolygonDrawMode::Name polygonDrawMode, float polygonOffsetFactor, float polygonOffsetUnits, G2Core::BlendFactor::Name blendFuncSourceFactor, G2Core::BlendFactor::Name blendFuncDestinationFactor)
{
	// not implemented
}

void FreeGfxResource(G2Core::GfxResource* resource)
{
	delete resource;
}

void GetPerspectiveProjection(glm::mat4& target, int width, int height, float zNear, float zFar, float fovY)
{
	// http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/perspective-projections-in-lh-and-rh-systems-r3598
	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovRH(&projection, fovY * 3.14f / 180.f, (float)width / (float)height, zNear, zFar);
	//projection[10] *= -1;
	//projection[14] *= -1;
	for (int i = 0; i < 16; ++i)
	{
		float* t = glm::value_ptr(target);
		t[i] = projection[i];
	}
}

void AdjustCameraSpaceMatrix(glm::mat4& camSpaceMatrix)
{
	float* data = glm::value_ptr(camSpaceMatrix);
	//data[2] *= -1.f;
	//data[6] *= -1.f;
	//data[10] *= -1.f;
	//data[14] *= -1.f;
	data[8] *= -1.f;
	data[9] *= -1.f;
	data[10] *= -1.f;
	data[11] *= -1.f;

}