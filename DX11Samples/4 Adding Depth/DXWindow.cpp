#include "DXWindow.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <glm/ext.hpp>

CGprofile CG_VERTEX_SHADER_PROFILES[] = {
	CG_PROFILE_VS_5_0,
	CG_PROFILE_VS_4_0,
	CG_PROFILE_VS_3_0,
	CG_PROFILE_VS_2_0,
	CG_PROFILE_VS_1_1
};

CGprofile CG_GEOMETRY_SHADER_PROFILES[] = {
	CG_PROFILE_GS_5_0,
	CG_PROFILE_GS_4_0
};

CGprofile CG_PIXEL_SHADER_PROFILES[] = {
	CG_PROFILE_PS_5_0,
	CG_PROFILE_PS_4_0,
	CG_PROFILE_PS_3_0,
	CG_PROFILE_PS_2_0,
	CG_PROFILE_PS_1_1
};

static void checkForCgError(const char * situation, CGcontext myCgContext, bool _exit = true)
{
	CGerror error;
	const char *string = cgGetLastErrorString(&error);

	if (error != CG_NO_ERROR)
	{
		if (error == CG_COMPILER_ERROR)
		{
			fprintf(stderr,
				"Situation: %s\n"
				"Error: %s\n\n"
				"Cg compiler output...\n%s",
				situation, string,
				cgGetLastListing(myCgContext));
		}
		else
		{
			fprintf(stderr,
				"Situation: %s\n"
				"Error: %s", situation, string);
		}

		if (_exit)
			exit(1);
	}
}

void _cgErrorHandler(CGcontext context, CGerror error, void* appData)
{
	if (error != CG_NO_ERROR)
	{
		const char* strPtr = cgGetErrorString(error);
		std::cout << "[CgRuntime] Cg Error: \n" << (strPtr != nullptr ? strPtr : "");

		if (error == CG_COMPILER_ERROR)
		{
			strPtr = cgGetLastListing(context);
			std::cout << (strPtr != nullptr ? strPtr : "");
		}
		std::cout << std::endl;
	}
}

std::string
read(std::string const& file)
{
	std::ifstream t(file);
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}

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
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	RECT wr = { 0, 0, mWidth, mHeight };    // set the size, but not the position
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

	// create a device, device context and swap chain using the information in the scd struct
	hr = D3D11CreateDeviceAndSwapChain(NULL,
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

	if (SUCCEEDED(hr))
	{
		std::cout << "Opened D3D HARDWARE Context!\n";
	}
	else
	{
		std::cout << "Could not open D3D HARDWARE Context!\n";
		exit(-1);
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

		///////////////**************new**************////////////////////
		//Describe our Depth/Stencil Buffer
		D3D11_TEXTURE2D_DESC depthStencilDesc;

		depthStencilDesc.Width = mWidth;
		depthStencilDesc.Height = mHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		//Create the Depth/Stencil View
		mDevice->CreateTexture2D(&depthStencilDesc, NULL, &mDepthStencilBuffer);
		mDevice->CreateDepthStencilView(mDepthStencilBuffer, NULL, &mDepthStencilView);

		//Set our Render Target
		mDeviceContext->OMSetRenderTargets(1, &mBackbuffer, mDepthStencilView);
		///////////////**************new**************////////////////////
	}

	// SET VIEWPORT
	{
		// Set the viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = mWidth;
		viewport.Height = mHeight;

		mDeviceContext->RSSetViewports(1, &viewport);
	}

	initPipeline();
	initGraphics();
}

void DXWindow::initPipeline()
{
	HRESULT hr;

	// INIT CG
	{
		std::cout << "Initialize Cg-Runtime-Direct3D 11" << std::endl;
		//qDebug("[Cg] Initialize Cg");
		// register the error handler
		cgSetErrorHandler(&_cgErrorHandler, NULL);
		// create a new Cg Context
		mCgContext = cgCreateContext();


		HRESULT hr = cgD3D11SetDevice(mCgContext, mDevice);

		if (hr != S_OK)
			return;

		// Register the default state assignment for OpenGL
		cgD3D11RegisterStates(mCgContext);
		// This will allow the Cg runtime to manage texture binding
		cgD3D11SetManageTextureParameters(mCgContext, CG_TRUE);
	}


	// LOAD SHADER
	{


		std::string sourcePtr = read("vshader.cg");
		int num = sizeof(CG_VERTEX_SHADER_PROFILES) / sizeof(CG_VERTEX_SHADER_PROFILES[0]);
		for (int i = 0; i < num; ++i)
		{
			auto optimal = cgD3D11GetOptimalOptions(CG_VERTEX_SHADER_PROFILES[i]);
			mVertexShaderId = cgCreateProgram(mCgContext, CG_SOURCE, sourcePtr.c_str(), CG_VERTEX_SHADER_PROFILES[i], "main", optimal);
			hr = cgD3D11LoadProgram(mVertexShaderId, NULL);

			if (SUCCEEDED(hr))
				break;
		}

		if (SUCCEEDED(hr))
		{
			std::cout << "Loaded vertex shader!\n";
		}
		else
		{
			std::cout << "Could not load vertex shader!\n";
			exit(-1);
		}

		sourcePtr = read("fshader.cg");
		num = sizeof(CG_PIXEL_SHADER_PROFILES) / sizeof(CG_PIXEL_SHADER_PROFILES[0]);
		for (int i = 0; i < num; ++i)
		{
			auto optimal = cgD3D11GetOptimalOptions(CG_PIXEL_SHADER_PROFILES[i]);
			mFragmentShaderId = cgCreateProgram(mCgContext, CG_SOURCE, sourcePtr.c_str(), CG_PIXEL_SHADER_PROFILES[i], "main", optimal);

			hr = cgD3D11LoadProgram(mFragmentShaderId, NULL);

			if (SUCCEEDED(hr))
				break;
		}

		if (SUCCEEDED(hr))
		{
			std::cout << "Loaded pixel shader!\n";
		}
		else
		{
			std::cout << "Could not load pixel shader!\n";
			exit(-1);
		}
		// get a D3D shader resource from CG shader resource
		ID3D10Blob* VS = cgD3D11GetCompiledProgram(mVertexShaderId);

		// activate the shader objects
		cgD3D11BindProgram(mVertexShaderId);
		cgD3D11BindProgram(mFragmentShaderId);

		// create the input layout object
		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		hr = mDevice->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &mLayout);

		if (SUCCEEDED(hr))
		{
			std::cout << "Created vertex layout for shader!\n";
		}
		else
		{
			std::cout << "Could not create vertex layout for shader!\n";
			exit(-1);
		}

		mDeviceContext->IASetInputLayout(mLayout);
	}
}

void
DXWindow::initGraphics()
{
	HRESULT hr;
	// LOAD VERTEX BUFFER
	{
		VERTEX OurVertices[] =
		{
			{ 0.0f, 0.5f, 0.5f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
			{ 0.45f, -0.5, 0.5f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
			{ -0.45f, -0.5f, 0.5f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) }
		};

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

		hr = mDevice->CreateBuffer(&bd, NULL, &mTriangleVertexBuffer);       // create the buffer

		if (SUCCEEDED(hr))
		{
			std::cout << "Created vertex buffer!\n";
		}
		else
		{
			std::cout << "Could not create vertex buffer!\n";
			exit(-1);
		}

		// Filling data into the buffer
		D3D11_MAPPED_SUBRESOURCE ms;
		mDeviceContext->Map(mTriangleVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
		memcpy(ms.pData, OurVertices, sizeof(OurVertices));										// copy the data
		mDeviceContext->Unmap(mTriangleVertexBuffer, NULL);                                     // unmap the buffer
	}
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
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
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void
DXWindow::startFrame()
{
	// clear the back buffer to a deep blue
	mDeviceContext->ClearRenderTargetView(mBackbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	///////////////**************new**************////////////////////
	//Refresh the Depth/Stencil view
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	///////////////**************new**************////////////////////

	// this struct holds Windows event messages
	MSG msg;
	// wait for the next message in the queue, store the result in 'msg'
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// translate keystroke messages into the right format
		TranslateMessage(&msg);

		// send the message to the WindowProc function
		DispatchMessage(&msg);
	}

	// DRAWING
	{

		// setting a uniform variable 

		glm::mat4 projection = glm::perspective(70.f, mWidth / (float)mHeight, 0.1f, 100.f);
		glm::mat4 camera = glm::translate(glm::vec3(0.f, 0.f, 5.f));

		D3DXMATRIX projectionDx;
		D3DXMatrixPerspectiveFovRH(&projectionDx, 70.f * 3.14 / 180.f, (float)mWidth / (float)mHeight, 0.1f, 100.f);
		for (int i = 0; i < 16; ++i)
		{
			float* t = glm::value_ptr(projection);
			t[i] = projectionDx[i];
		}

		CGparameter location = cgGetNamedParameter(mVertexShaderId, "mvp");
		cgSetMatrixParameterfc(location, glm::value_ptr(/*projection * camera*/glm::mat4()));
		// call bind again to update data of the shader
		cgD3D11BindProgram(mVertexShaderId);

		// select which vertex buffer to display
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		mDeviceContext->IASetVertexBuffers(0, 1, &mTriangleVertexBuffer, &stride, &offset);

		// select which primtive type we are using
		mDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw the vertex buffer to the back buffer
		mDeviceContext->Draw(3, 0);
	}
}

void
DXWindow::endFrame()
{
	// switch the back buffer and the front buffer
	mSwapchain->Present(0, 0);
}

DXWindow::~DXWindow()
{
	mSwapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
	mDepthStencilView->Release();
	mDepthStencilBuffer->Release();
	// close and release all existing COM objects
	mSwapchain->Release();
	mBackbuffer->Release();

	mDevice->Release();
	mDeviceContext->Release();
}