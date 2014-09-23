#include "DXWindow.h"
#include <iostream>
#include <fstream>
#include <sstream>

CGcontext gCgContext = nullptr;
CGprofile gCgVertexShaderProfile;
CGprofile gCgGeometryShaderProfile;
CGprofile gCgFragmentShaderProfile;

const char ** profileOpts = NULL;

void _cgErrorHandler(CGcontext context, CGerror error, void* appData) 
{
	if(error != CG_NO_ERROR)
	{
		const char* strPtr = cgGetErrorString(error);
		std::cout << "[CgRuntime] Cg Error: \n" << (strPtr != nullptr ? strPtr : "");
		
		if(error == CG_COMPILER_ERROR)
		{
			strPtr = cgGetLastListing(context);
			std::cout << (strPtr != nullptr ? strPtr : "");
		}
		std::cout << std::endl;
	}
}

void _initCgRuntime(ID3D11Device *dev)
{
	if(gCgContext == nullptr) {
		std::cout << "[CgRuntime] Initialize Cg-Runtime-Direct3D 11" << std::endl;
		//qDebug("[Cg] Initialize Cg");
		// register the error handler
		cgSetErrorHandler( &_cgErrorHandler, NULL);
		// create a new Cg Context
		gCgContext = cgCreateContext();

		
		HRESULT hr = cgD3D11SetDevice( gCgContext, dev);
	
		if( hr != S_OK )
			return;

		// Register the default state assignment for OpenGL
		//cgD3D11RegisterStates(gCgContext);
		// This will allow the Cg runtime to manage texture binding
		//cgD3D11SetManageTextureParameters(gCgContext, CG_TRUE);
		
		gCgVertexShaderProfile = cgD3D11GetLatestVertexProfile();
		if(gCgVertexShaderProfile == CG_PROFILE_UNKNOWN)
		{
			// ERROR
			std::cout << "[CgRuntime] Error: Could not get valid Vertex-Profile." << std::endl;
			return;
		}
		
		gCgGeometryShaderProfile = cgD3D11GetLatestGeometryProfile();
		if(gCgGeometryShaderProfile == CG_PROFILE_UNKNOWN)
		{
			// WARNING
			std::cout << "[CgRuntime] Warning: Could not get valid Geometry-Profile." << std::endl;
		}
		
		gCgFragmentShaderProfile = cgD3D11GetLatestPixelProfile();
		if(gCgFragmentShaderProfile == CG_PROFILE_UNKNOWN)
		{
			// ERROR
			std::cout << "[CgRuntime] Error: Could not get valid Fragment-Profile." << std::endl;
			return;
		}

		profileOpts = cgD3D11GetOptimalOptions(gCgVertexShaderProfile);
	}
}

D3D_DRIVER_TYPE             g_driverType        = D3D_DRIVER_TYPE_NULL;

// the WindowProc function prototype for message handling
LRESULT CALLBACK WindowProc(HWND hWnd,
						 UINT message,
						 WPARAM wParam,
						 LPARAM lParam);

DXWindow::DXWindow(void)
	: WndClassName(L"DX11 Window"),
	hWnd(nullptr),
	hInstance(GetModuleHandle(NULL)),
	mWidth(800),
	mHeight(600)
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
	wc.lpszClassName = L"WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	RECT wr = {0, 0, mWidth, mHeight};    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
						  L"WindowClass1",    // name of the window class
						  WndClassName,   // title of the window
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

	initD3DApp();
}

void DXWindow::initD3DApp()
{
	HRESULT hr;

	//Describe our SwapChain Buffer
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = mWidth;
	bufferDesc.Height = mHeight;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	// clear out the struct for use
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hWnd; 
	swapChainDesc.Windowed = TRUE; 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_DRIVER_TYPE driverTypes[] = 
	{
		D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_HARDWARE,
	};
	UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		g_driverType = driverTypes[driverTypeIndex];
		HRESULT hr = D3D11CreateDeviceAndSwapChain( NULL,                  // Adapter
											g_driverType,          // Driver Type
											NULL,                  // Software
											D3D11_CREATE_DEVICE_DEBUG,     // Flags
											NULL,        // Feature Levels
											NULL,                     // Num Feature Levels
											D3D11_SDK_VERSION,     // SDK Version
											  &swapChainDesc,
											  &swapchain,
											  &dev,
											  NULL,
											  &devcon);

		if( SUCCEEDED( hr ) )
			break;
	}

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

	// use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

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
	dev->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	dev->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	// bind it to the render target (=backbuffer)
	devcon->OMSetRenderTargets( 1, &backbuffer, depthStencilView );

	_initCgRuntime(dev);
}

void
DXWindow::startFrame() 
{
	// clear the back buffer to a deep blue
	float col[4] = {0.0f, 0.2f, 0.4f, 1.0f};
	devcon->ClearRenderTargetView(backbuffer, col);
	devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void
DXWindow::swapBuffer() 
{
	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);
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


DXWindow::~DXWindow(void)
{
	// Direct3D is actually incapable of closing when in fullscreen mode
	swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

	depthStencilView->Release();
	depthStencilBuffer->Release();


	// close and release all existing COM objects
	//pVS->Release();
	//pPS->Release();
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}

void
DXWindow::processEvents(int frame) 
{
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
}

void
DXWindow::createDepthAndStencilBuffer() 
{

	
}

CGcontext
DXWindow::getCgContext() 
{
	return gCgContext;
}

CGprofile
DXWindow::getVSProfile() 
{
	return gCgVertexShaderProfile;
}

CGprofile
DXWindow::getFSProfile() 
{
	return gCgFragmentShaderProfile;
}