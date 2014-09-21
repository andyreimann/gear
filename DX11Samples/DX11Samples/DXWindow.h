#pragma once// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>		// core
#include <d3dx11.h>		// extensions
#include <d3dx10.h>		// dx 11 is ext of dx 10 -> useful macros usable

#include <Cg/cgD3D11.h>

#include <string>

class DXWindow
{
	public:
		DXWindow(void);
		~DXWindow(void);
		
		ID3D11Device* getDevice() { return dev; }
		ID3D11DeviceContext* getDeviceContext() { return devcon; }
		CGcontext getCgContext();
		CGprofile getVSProfile();
		CGprofile getFSProfile();
		unsigned int getWidth() { return mWidth; }
		unsigned int getHeight() { return mHeight; }
		
		virtual void processEvents(int frame);
		virtual void swapBuffer();
		virtual void startFrame();

	private:
	
		void initD3DApp();

		LPCTSTR WndClassName;		// The title of the window
		HWND hWnd;					// The handle of the window
		HINSTANCE hInstance;		// The application handle
		
		unsigned int mWidth;
		unsigned int mHeight;

		IDXGISwapChain *swapchain;	// the pointer to the swap chain interface
		ID3D11Device *dev;                     // the pointer to our Direct3D device interface
		ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
		ID3D11RenderTargetView *backbuffer;    // The back buffer

		void createDepthAndStencilBuffer();

		// DEPTH
		ID3D11DepthStencilView* depthStencilView;
		ID3D11Texture2D* depthStencilBuffer;

};

