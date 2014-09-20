#pragma once
#include <windows.h>

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;


struct D3DGlobals
{
	HWND					hwnd;			// The window handle
	IDXGISwapChain*			swapChain;		// the pointer to the swap chain interface
	ID3D11Device*			device;		// the pointer to our Direct3D device interface
	ID3D11DeviceContext*	deviceContext; // the pointer to our Direct3D device context
	ID3D11RenderTargetView*	backbuffer;    // The back buffer
	ID3D11DepthStencilView* backbufferDepthStencilView;
	ID3D11Texture2D*		backbufferDepthStencilBuffer;
};