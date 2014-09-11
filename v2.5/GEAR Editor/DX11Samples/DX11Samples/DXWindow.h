#pragma once// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>		// core
#include <d3dx11.h>		// extensions
#include <d3dx10.h>		// dx 11 is ext of dx 10 -> useful macros usable

struct VERTEX
{
	  FLOAT X, Y, Z;      // position
	  D3DXCOLOR Color;    // color
};

class DXWindow
{
	public:
		DXWindow(void);
		~DXWindow(void);

		virtual void processEvents(int frame);

		virtual void swapBuffer();

		virtual void setHideMouseMode(bool mode) {};

	private:
	
		LPCTSTR WndClassName;		// The title of the window
		HWND hWnd;					// The handle of the window
		HINSTANCE hInstance;		// The application handle
		
		unsigned int mWidth;
		unsigned int mHeight;

		IDXGISwapChain *swapchain;	// the pointer to the swap chain interface
		ID3D11Device *dev;                     // the pointer to our Direct3D device interface
		ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
		ID3D11RenderTargetView *backbuffer;    // The back buffer


		ID3D10Blob *VS, *PS;
		ID3D11VertexShader *pVS;    // the vertex shader
		ID3D11PixelShader *pPS;     // the pixel shader

		ID3D11Buffer *pVBuffer;		// A vertex buffer
		ID3D11InputLayout *pLayout;	// A vertex buffer layout
};

