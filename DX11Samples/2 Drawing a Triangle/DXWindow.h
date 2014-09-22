#pragma once// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>		// core
#include <d3dx11.h>		// extensions
#include <d3dx10.h>		// dx 11 is ext of dx 10 -> useful macros usable

#include <Cg/cgD3D11.h>

#include <string>

struct VERTEX
{
	  FLOAT X, Y, Z;      // position
	  D3DXCOLOR Color;    // color
};

class DXWindow
{
	public:
		DXWindow();
		~DXWindow();

		void startFrame();
		void endFrame();

	private:

		void initD3D();
		void initPipeline();
		void initGraphics();

		LPCTSTR WndClassName;		// The title of the window
		HWND hWnd;					// The handle of the window
		HINSTANCE hInstance;		// The application handle
		
		unsigned int mWidth;
		unsigned int mHeight;

		

		// global declarations
		IDXGISwapChain*			mSwapchain;             // the pointer to the swap chain interface
		ID3D11Device*			mDevice;                // the pointer to our Direct3D device interface
		ID3D11DeviceContext*	mDeviceContext;         // the pointer to our Direct3D device context
		ID3D11RenderTargetView* mBackbuffer;			// the pointer to out backbuffer

		// render pipeline
		ID3D11VertexShader* mVShader;					// the vertex shader
		ID3D11PixelShader*	mPShader;					// the pixel shader
		ID3D11InputLayout *	mLayout;						// The Vertex-Layout of the shader
		// graphics
		ID3D11Buffer*		mTriangleVertexBuffer;		// A Vertex buffer holding data for a colored triangle
};

