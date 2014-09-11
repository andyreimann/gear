#include "DX11GfxDevice.h"


#include <windowsx.h>
#include <d3d11.h>		// core
#include <d3dx11.h>		// extensions
#include <d3dx10.h>		// dx 11 is ext of dx 10 -> useful macros usable

using namespace G2;

DX11GfxDevice::DX11GfxDevice() 
{
}

DX11GfxDevice::~DX11GfxDevice() 
{
	// Direct3D is actually incapable of closing when in full screen mode
	mSwapChain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

	// close and release all existing COM objects
	mSwapChain->Release();
	mBackbuffer->Release();
	mDevice->Release();
	mDeviceContext->Release();
}

void
G2::DX11GfxDevice::init(HWND hWnd) 
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	//scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
								  D3D_DRIVER_TYPE_HARDWARE,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  D3D11_SDK_VERSION,
								  &scd,
								  &mSwapChain,
								  &mDevice,
								  NULL,
								  &mDeviceContext);

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	mDevice->CreateRenderTargetView(pBackBuffer, NULL, &mBackbuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	mDeviceContext->OMSetRenderTargets(1, &mBackbuffer, NULL);
}

void
G2::DX11GfxDevice::clearColor(D3DXCOLOR const& color) 
{
	mDeviceContext->ClearRenderTargetView(mBackbuffer, color);
}

void
G2::DX11GfxDevice::draw(void* vbo, int drawMode, int startVertex, int numVertices, unsigned int stride, unsigned int offset) 
{
	ID3D11Buffer* vertexBuffer = static_cast<ID3D11Buffer*>(vbo);
	
	mDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->Draw(3, 0);    // draw 3 vertices, starting from vertex 0
}