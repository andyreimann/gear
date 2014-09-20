#include "DX11Window.h"

#include <G2/Defines.h>
#include <G2/Logger.h>

#include <G2Core/ECSManager.h>
#include <G2Core/EventDistributer.h>
#include <G2Core/GfxDevice.h>
#include <G2DX11/DX11Globals.h>

#include <iostream>
#include <unordered_map>

static std::string ASSET_PATH = "../../Assets/";

IDXGISwapChain*			gSwapChain;		// the pointer to the swap chain interface
ID3D11Device*			gDevice;		// the pointer to our Direct3D device interface
ID3D11DeviceContext*	gDeviceContext; // the pointer to our Direct3D device context

// The mail render target
ID3D11RenderTargetView*	gBackbuffer;    // The back buffer
ID3D11DepthStencilView* gBackbufferDepthStencilView;
ID3D11Texture2D*		gBackbufferDepthStencilBuffer;

// the WindowProc function prototype for message handling
LRESULT CALLBACK WindowProc(HWND hWnd,
						 UINT message,
						 WPARAM wParam,
						 LPARAM lParam);

DX11Window::DX11Window(std::string const& title, unsigned int width, unsigned int height, bool hideMouse)
	: G2::AbstractWindow(title,width,height,hideMouse),
	mMousePosition(0,0),
	hWnd(nullptr),
	hInstance(GetModuleHandle(NULL)),
	mEditorCamera(this)
{


	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	RECT wr = {0, 0, mWidth, mHeight};    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
						  "WindowClass1",    // name of the window class
						  mTitle.c_str(),   // title of the window
						  WS_OVERLAPPEDWINDOW,    // window style
						  850,			// x-position of the window
						  50,			// y-position of the window
						  wr.right - wr.left,    // width of the window
						  wr.bottom - wr.top,    // height of the window
						  NULL,			// we have no parent window, NULL
						  NULL,			// we aren't using menus, NULL
						  hInstance,    // application handle
						  NULL);		// used with multiple windows, NULL

	// display the window on the screen
	ShowWindow(hWnd, SW_SHOW);

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
	depthStencilDesc.Width     = mWidth;
	depthStencilDesc.Height    = mHeight;
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

	D3DGlobals globals;
	globals.hwnd = hWnd;
	globals.device = gDevice;
	globals.deviceContext = gDeviceContext;
	globals.backbuffer = gBackbuffer;
	globals.backbufferDepthStencilView = gBackbufferDepthStencilView;
	globals.backbufferDepthStencilBuffer = gBackbufferDepthStencilBuffer;

	G2_gfxDevice()->init(&globals);

	G2_gfxDevice()->setViewport(G2::rect(0.f,0.f,mWidth, mHeight));

	

	mEditorCamera 
		.pan(0.f,10.f)
		.rotate(25.f, 0.f)
		.moveView(-15.f)
		.getComponent<G2::CameraComponent>()->setAsRenderCamera();

	// new way of loading shader
	std::shared_ptr<G2::Effect> effect = mEffectImporter.import(ASSET_PATH + "Shader/Default_CG.g2fx");
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem,G2::RenderComponent>()
		->setDefaultEffect(effect);
}

DX11Window::~DX11Window() 
{

}

void
DX11Window::processEvents(int frame) 
{
	if(frame == 0)
	{
		G2::EventDistributer::onViewportResize(getWidth(),getHeight());
	}

	
	// this struct holds Windows event messages
	MSG msg;

	// wait for the next message in the queue, store the result in 'msg'
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// translate keystroke messages into the right format
		TranslateMessage(&msg);

		// send the message to the WindowProc function
		DispatchMessage(&msg);
	}
	return;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch(message)
	{
		// this message is read when the window is closed
		case WM_DESTROY:
			{
				// close the application entirely
				PostQuitMessage(0);
				return 0;
			} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc (hWnd, message, wParam, lParam);
}  

void
DX11Window::swapBuffer() 
{
	// switch the back buffer and the front buffer
	gSwapChain->Present(0, 0);
}

void
DX11Window::onKeyUp(G2::KeyCode keyCode) 
{
}

void
DX11Window::onKeyDown(G2::KeyCode keyCode) 
{
}

void
DX11Window::setHideMouseMode(bool mode) 
{
	mHideMouse = mode;
}