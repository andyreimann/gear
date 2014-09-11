#pragma once
#include "Defines.h"
#include <G2Core/GfxDevice.h>

namespace G2 
{
	/** This class implements the GfxDefive interface using OpenGL.
	 * @created:	2014/09/11
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class GlGfxDevice : public GfxDevice
	{
		public:
			/// This constructs a new GlGfxDevice.
			GEAROPENGLDLL_API GlGfxDevice();
			
			/** Clears the color of the back buffer to the given color
			 * @param color The color to clear the back buffer with.
			 */
			GEAROPENGLDLL_API virtual void clearColor(glm::vec4 const& color);
			/** Draws the given VertexBuffer
			 * @param vbo A pointer to the VertexBuffer to render.
			 * @param drawMode The draw mode to use
			 * @param startVertex The start vertex to draw.
			 * @param numVertices The number of vertices to draw
			 * @param stride The stride to use (byte distance between the vertices).
			 * @param offset The offset to use in bytes.
			 */
			GEAROPENGLDLL_API virtual void draw(void* vbo, int drawMode, int startVertex, int numVertices, unsigned int strideBytes, unsigned int offsetBytes);
			

			/// normal destructor
			GEAROPENGLDLL_API ~GlGfxDevice();
		protected:
		private:
	};
};