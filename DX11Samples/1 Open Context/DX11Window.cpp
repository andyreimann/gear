#include "DX11Window.h"
#include <iostream>
#include <fstream>
#include <sstream>

// the WindowProc function prototype for message handling
LRESULT CALLBACK WindowProc(HWND hWnd,
						 UINT message,
						 WPARAM wParam,
						 LPARAM lParam);

DXWindow::DXWindow(void)
	: WndClassName("DX11 Window"),
	hWnd(nullptr),
	hInstance(GetModuleHandle(NULL)),
	mWidth(800), 
	mHeight(600)
{
	HRESULT hr;
	
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
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	RECT wr = {0, 0, mWidth, mHeight};    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
						  "WindowClass1",    // name of the window class
						  "Our First Windowed Program",   // title of the window
						  WS_OVERLAPPEDWINDOW,    // window style
						  300,    // x-position of the window
						  300,    // y-position of the window
						  wr.right - wr.left,    // width of the window
						  wr.bottom - wr.top,    // height of the window
						  NULL,    // we have no parent window, NULL
						  NULL,    // we aren't using menus, NULL
						  hInstance,    // application handle
						  NULL);    // used with multiple windows, NULL

	// start D3D
	initD3D();

	// display the window on the screen
	ShowWindow(hWnd, SW_SHOW);
}

void
DXWindow::initD3D() 
{
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

	// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
								  D3D_DRIVER_TYPE_HARDWARE,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  D3D11_SDK_VERSION,
								  &swapChainDesc,
								  &mSwapchain,
								  &mDevice,
								  NULL,
								  &mDeviceContext);

	if( SUCCEEDED( hr ) )
		std::cout << "Opened D3D HARDWARE Context!";
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
DXWindow::startFrame() 
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
DXWindow::endFrame() 
{
}

DXWindow::~DXWindow() 
{
	// close and release all existing COM objects
	mSwapchain->Release();
	mDevice->Release();
	mDeviceContext->Release();
}