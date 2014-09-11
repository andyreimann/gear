#pragma once
#include "Defines.h"
#include <G2Core/GfxDevice.h>

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
#include <windows.h>

namespace G2 
{
	/** This class implements the GfxDefive interface using DirectX 11.
	 * @created:	2014/09/11
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class DX11GfxDevice : public GfxDevice
	{
		public:
			/** This constructs a new DX11GfxDevice.
			 * @param hWnd The window handle.
			 */
			GEARDX11DLL_API DX11GfxDevice(HWND hWnd);
			
			/** Clears the color of the back buffer to the given color
			 * @param color The color to clear the back buffer with.
			 */
			GEARDX11DLL_API virtual void clearColor(glm::vec4 const& color);
			/** Draws the given VertexBuffer
			 * @param vbo A pointer to the VertexBuffer to render.
			 * @param drawMode The draw mode to use
			 * @param startVertex The start vertex to draw.
			 * @param numVertices The number of vertices to draw
			 * @param stride The stride to use (byte distance between the vertices).
			 * @param offset The offset to use in bytes.
			 */
			GEARDX11DLL_API virtual void draw(void* vbo, int drawMode, int startVertex, int numVertices, unsigned int strideBytes, unsigned int offsetBytes);
			

			/// normal destructor
			GEARDX11DLL_API ~DX11GfxDevice();
		protected:
			
			HWND					mHwnd;			// The window handle
			IDXGISwapChain*			mSwapChain;		// the pointer to the swap chain interface
			ID3D11Device*			mDevice;		// the pointer to our Direct3D device interface
			ID3D11DeviceContext*	mDeviceContext; // the pointer to our Direct3D device context
			ID3D11RenderTargetView*	mBackbuffer;    // The back buffer
	};
};