#include "DX11Window.h"

#include <G2/Defines.h>
#include <G2/Logger.h>
#include <G2Core/EventDistributer.h>

#include <G2Core/ECSManager.h>
#include <G2Core/EventDistributer.h>
#include <G2Core/GfxDevice.h>
#include <G2DX11/DX11Globals.h>

#include <iostream>
#include <unordered_map>

// mouse_t; defines the mouse data struct

std::unordered_map<int, int> gMouseButtonMapping;


struct tagMouse
{
	int x, y,
		pinstate,
		pin;
	BYTE    buttonDown[4];
	DIMOUSESTATE state;
} mouse_t;

// keyboard_t; defines the keyboard data struct
struct tagKeyboard
{
	byte state[256];
} keyboard_t;

static std::string ASSET_PATH = "../../Assets/";

// the WindowProc function prototype for message handling
LRESULT CALLBACK WindowProc(HWND hWnd,
						 UINT message,
						 WPARAM wParam,
						 LPARAM lParam);

DX11Window::DX11Window(std::string const& title, unsigned int width, unsigned int height, bool hideMouse)
	: G2::AbstractWindow(title,width,height,hideMouse),
	hWnd(nullptr),
	hInstance(GetModuleHandle(NULL)),
	mMousePosition(0, 0)
{

	gMouseButtonMapping[0] = G2::MOUSE_LEFT;
	gMouseButtonMapping[2] = G2::MOUSE_MIDDLE;
	gMouseButtonMapping[1] = G2::MOUSE_RIGHT;


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
						  550,			// x-position of the window
						  50,			// y-position of the window
						  wr.right - wr.left,    // width of the window
						  wr.bottom - wr.top,    // height of the window
						  NULL,			// we have no parent window, NULL
						  NULL,			// we aren't using menus, NULL
						  hInstance,    // application handle
						  NULL);		// used with multiple windows, NULL

	initD3D();
	initDInput();

	// display the window on the screen
	ShowWindow(hWnd, SW_SHOW);
}



void
DX11Window::initD3D()
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
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching by Alt+Enter

	int deviceCreationFlags = D3D11_CREATE_DEVICE_DEBUG;

	// create a device, device context and swap chain using the information in the scd struct
	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		deviceCreationFlags,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mSwapchain,
		&mDevice,
		NULL,
		&mDeviceContext); 

	if (SUCCEEDED(hr))
	{
		std::cout << "Opened D3D HARDWARE Context!\n";
	}
	else
	{
		std::cout << "Could not open D3D HARDWARE Context!\n";
		exit(-1);
	}

	ID3D11Debug *d3dDebug = nullptr;
	if (SUCCEEDED(mDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3dDebug)))
	{
		ID3D11InfoQueue *d3dInfoQueue = nullptr;
		if (SUCCEEDED(d3dDebug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue)))
		{ 
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			//d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif 
		}
	}
	 


	D3D_FEATURE_LEVEL featureLevel = mDevice->GetFeatureLevel();


	std::unordered_map<D3D_FEATURE_LEVEL, std::string> featureLevelMapping;
	featureLevelMapping[D3D_FEATURE_LEVEL_9_1] = "D3D_FEATURE_LEVEL_9_1";
	featureLevelMapping[D3D_FEATURE_LEVEL_9_2] = "D3D_FEATURE_LEVEL_9_2";
	featureLevelMapping[D3D_FEATURE_LEVEL_9_3] = "D3D_FEATURE_LEVEL_9_3";
	featureLevelMapping[D3D_FEATURE_LEVEL_10_0] = "D3D_FEATURE_LEVEL_10_0";
	featureLevelMapping[D3D_FEATURE_LEVEL_10_1] = "D3D_FEATURE_LEVEL_10_1";
	featureLevelMapping[D3D_FEATURE_LEVEL_11_0] = "D3D_FEATURE_LEVEL_11_0";
	//featureLevelMapping[D3D_FEATURE_LEVEL_11_1]	= "D3D_FEATURE_LEVEL_11_1";
	std::cout << "Rendering with feature level " << featureLevelMapping[featureLevel] << "!\n";


	// SET RENDER TARGET
	{
		// get the address of the back buffer
		ID3D11Texture2D *pBackBuffer;
		mSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		hr = mDevice->CreateRenderTargetView(pBackBuffer, NULL, &mBackbuffer);

		if (SUCCEEDED(hr))
		{
			std::cout << "Created back buffer!\n";
		}
		else
		{
			std::cout << "Could not create back buffer!\n";
			exit(-1);
		}
		pBackBuffer->Release();



		// set up a depth/stencil buffer
		D3D11_TEXTURE2D_DESC depthStencilTexDesc;

		// TODO Where to get the data from?
		depthStencilTexDesc.Width = mWidth;
		depthStencilTexDesc.Height = mHeight;
		depthStencilTexDesc.MipLevels = 1;
		depthStencilTexDesc.ArraySize = 1;
		depthStencilTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilTexDesc.SampleDesc.Count = 1;
		depthStencilTexDesc.SampleDesc.Quality = 0;
		depthStencilTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilTexDesc.CPUAccessFlags = 0;
		depthStencilTexDesc.MiscFlags = 0;


		//Create the actual view
		hr = mDevice->CreateTexture2D(&depthStencilTexDesc, NULL, &mBackbufferDepthStencilBuffer);
		hr = mDevice->CreateDepthStencilView(mBackbufferDepthStencilBuffer, NULL, &mBackbufferDepthStencilView);


		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthStencilDesc.StencilEnable = false;
		depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		hr = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
		if (SUCCEEDED(hr)) 
		{
			std::cout << "Created Depth-Stencil state!\n";
		}
		else
		{
			std::cout << "Could not create Depth-Stencil state!\n";
			exit(-1);
		}
		mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

		//D3D11_RASTERIZER_DESC gRasterizerStateDesc;
		//// clear out the struct for use
		//ZeroMemory(&gRasterizerStateDesc, sizeof(D3D11_RASTERIZER_DESC));
		//gRasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
		//gRasterizerStateDesc.CullMode = D3D11_CULL_BACK;
		//gRasterizerStateDesc.FrontCounterClockwise = false;
		//ID3D11RasterizerState* gRasterizerState; 
		//mDevice->CreateRasterizerState(&gRasterizerStateDesc, &gRasterizerState);
		//mDeviceContext->RSSetState(gRasterizerState);

		// set the render target as the back buffer
		mDeviceContext->OMSetRenderTargets(1, &mBackbuffer, mBackbufferDepthStencilView);
	}  

	// SET VIEWPORT AND GLOBALS
	{
		D3DGlobals globals;
		globals.hwnd = hWnd;
		globals.device = mDevice;
		globals.deviceContext = mDeviceContext;
		globals.backbuffer = mBackbuffer;
		globals.backbufferDepthStencilView = mBackbufferDepthStencilView;
		globals.backbufferDepthStencilBuffer = mBackbufferDepthStencilBuffer;

		G2_gfxDevice()->init(&globals);

		G2_gfxDevice()->setViewport(G2::rect(0.f, 0.f, mWidth, mHeight)); 
	}
}

bool DX11Window::initDInput()
{
	HRESULT
		hr = DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&mDirectInput,
		NULL);
	hr = mDirectInput->CreateDevice(GUID_SysKeyboard,
		&mKeyboard,
		NULL);

	hr = mDirectInput->CreateDevice(GUID_SysMouse,
		&mMouse,
		NULL);

	hr = mKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = mKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = mMouse->SetDataFormat(&c_dfDIMouse);
	hr = mMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	return true;
}

DX11Window::~DX11Window() 
{

	mKeyboard->Release();
	mMouse->Release();
	mSwapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
	// close and release all existing COM objects
	mSwapchain->Release(); 
	mBackbufferDepthStencilView->Release();
	mBackbufferDepthStencilBuffer->Release();
	mBackbuffer->Release(); 

	mDevice->Release();
	mDeviceContext->Release();
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

	mKeyboard->Acquire();
	mMouse->Acquire();

	mMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouse_t.state);

	mKeyboard->GetDeviceState(sizeof(keyboard_t.state), (LPVOID)&keyboard_t.state);
	 
	//if (keyboardState[DIK_ESCAPE] & 0x80)
	//	PostMessage(hWnd, WM_DESTROY, 0, 0);
	//if (keyboardState[DIK_LEFT] & 0x80)
	//{
	//	PostMessage(hWnd, WM_DESTROY, 0, 0);
	//}

	// has a single-click occured?
	for (int i = 0; i < 4; ++i)
	{
		if (mouse_t.state.rgbButtons[i] && !mouse_t.buttonDown[i]) {
			mouse_t.buttonDown[i] = 1;

			// left mouse down
			G2::EventDistributer::onMouseDown(gMouseButtonMapping[i], glm::detail::tvec2<int>(0, 0));
		}
		// reset 'pin' state
		if (!mouse_t.state.rgbButtons[i] && mouse_t.buttonDown[i] == 1)
		{ 
			mouse_t.buttonDown[i] = 0;
			G2::EventDistributer::onMouseUp(gMouseButtonMapping[i], glm::detail::tvec2<int>(0, 0));
		}
	}

	if (mouse_t.state.lX != 0 || mouse_t.state.lY != 0)
	{
		mMousePosition.x += mouse_t.state.lX;
		mMousePosition.y += mouse_t.state.lY;
		G2::EventDistributer::onMouseMove(mMousePosition);
	}

	if (mouse_t.state.lZ != 0)
	{
		G2::EventDistributer::onMouseWheel((int)(mouse_t.state.lZ / 100.f));
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
	mSwapchain->Present(0, 0);
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