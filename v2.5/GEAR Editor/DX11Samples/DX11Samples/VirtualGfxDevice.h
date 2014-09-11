#pragma once
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>		// core
#include <d3dx11.h>		// extensions
#include <d3dx10.h>		// dx 11 is ext of dx 10 -> useful macros usable

namespace G2 
{

	/** This class defines 
	 * @created:	2014/09/11
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class VirtualGfxDevice /*: public NonCopyable */
	{
		public:
			/// This constructs a new VirtualGfxDevice.
			VirtualGfxDevice() {}

			virtual void clearColor(D3DXCOLOR const& color) = 0;
			virtual void draw(void* vbo, int drawMode, int startVertex, int numVertices, unsigned int stride, unsigned int offset) = 0;
			
			virtual ~VirtualGfxDevice() = 0 {}


		protected:

		private:
	};
};