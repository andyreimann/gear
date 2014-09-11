#pragma once
#include "VirtualGfxDevice.h"

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
#include <windows.h>

namespace G2 
{
	/** This class defines...
	 * @created:	2014/09/11
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class DX11GfxDevice : public VirtualGfxDevice
	{
		public:
			/// This constructs a new DX11GfxDevice.
			DX11GfxDevice();
			virtual ~DX11GfxDevice();

			virtual void clearColor(D3DXCOLOR const& color) override;
			virtual void draw(void* vbo, int drawMode, int startVertex, int numVertices, unsigned int stride, unsigned int offset) override;

			void init(HWND hWnd);
		protected:
			
			IDXGISwapChain*			mSwapChain;		// the pointer to the swap chain interface
			ID3D11Device*			mDevice;		// the pointer to our Direct3D device interface
			ID3D11DeviceContext*	mDeviceContext; // the pointer to our Direct3D device context
			ID3D11RenderTargetView*	mBackbuffer;    // The back buffer
	};
};