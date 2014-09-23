#include "DXWindow.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include <D3Dcompiler.h>		// 

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
	hr = D3D11CreateDeviceAndSwapChain(	  NULL,
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
	{
		std::cout << "Opened D3D HARDWARE Context!\n";
	}
	else
	{
		std::cout << "Could not open D3D HARDWARE Context!\n";
		exit(-1);
	}

	
	// SET RENDER TARGET
	{
		// get the address of the back buffer
		ID3D11Texture2D *pBackBuffer;
		mSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		hr = mDevice->CreateRenderTargetView(pBackBuffer, NULL, &mBackbuffer);
		
		if( SUCCEEDED( hr ) )
		{
			std::cout << "Created back buffer!\n";
		}
		else
		{
			std::cout << "Could not create back buffer!\n";
			exit(-1);
		}
		pBackBuffer->Release();

		// set the render target as the back buffer
		mDeviceContext->OMSetRenderTargets(1, &mBackbuffer, NULL);
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
	// LOAD SHADER
	{
		// load and compile the two shader
		ID3D10Blob *VS, *PS;
		//D3DX11CompileFromFile("shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
		//D3DX11CompileFromFile("shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);


		std::string sourcePtr = read("shaders.shader");
		D3DCompile(sourcePtr.c_str(), sourcePtr.size(), 0, 0, 0, "VShader", "vs_4_0", 0, 0, &VS, 0);
		D3DCompile(sourcePtr.c_str(), sourcePtr.size(), 0, 0, 0, "PShader", "ps_4_0", 0, 0, &PS, 0);

		ID3D11ShaderReflection* reflection;
		D3DReflect(VS->GetBufferPointer(), VS->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);
		reflection->GetConstantBufferByName("myUniform");
		D3D11_SHADER_INPUT_BIND_DESC uniformDesc;
		reflection->GetResourceBindingDescByName("myUniform", &uniformDesc);
		
		

		
		// encapsulate both shaders into shader objects
		mDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &mVShader);
		mDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &mPShader);
	
		// activate the shader objects
		mDeviceContext->VSSetShader(mVShader, 0, 0);
		mDeviceContext->PSSetShader(mPShader, 0, 0);

		// create the input layout object
		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		hr = mDevice->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &mLayout);
		
		if( SUCCEEDED( hr ) )
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
			{0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
			{0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
			{-0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)}
		};

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

		hr = mDevice->CreateBuffer(&bd, NULL, &mTriangleVertexBuffer);       // create the buffer
		
		if( SUCCEEDED( hr ) )
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
	// clear the back buffer to a deep blue
	mDeviceContext->ClearRenderTargetView(mBackbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
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

	// DRAWING
	{
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
	// close and release all existing COM objects
	mVShader->Release();
	mPShader->Release();
	mSwapchain->Release();
	mBackbuffer->Release();

	mDevice->Release();
	mDeviceContext->Release();
}