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
		DXWindow();
		~DXWindow();

		void startFrame();
		void endFrame();

	private:

		void initD3D();

		LPCTSTR WndClassName;		// The title of the window
		HWND hWnd;					// The handle of the window
		HINSTANCE hInstance;		// The application handle
		
		unsigned int mWidth;
		unsigned int mHeight;

		

		// global declarations
		IDXGISwapChain*			mSwapchain;             // the pointer to the swap chain interface
		ID3D11Device*			mDevice;                     // the pointer to our Direct3D device interface
		ID3D11DeviceContext*	mDeviceContext;           // the pointer to our Direct3D device context


};

