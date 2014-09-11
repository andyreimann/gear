#pragma once

#include <glm/glm.hpp>

namespace G2 
{
	/** This class declares the interface of a graphics device.
	 * @created:	2014/09/11
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class GfxDevice 
	{
		public:
			/// This constructs a new GfxDevice.
			GfxDevice() {}
			/** Clears the color of the back buffer to the given color
			 * @param color The color to clear the back buffer with.
			 */
			virtual void clearColor(glm::vec4 const& color) = 0;
			/** Draws the given VertexBuffer
			 * @param vbo A pointer to the VertexBuffer to render.
			 * @param drawMode The draw mode to use
			 * @param startVertex The start vertex to draw.
			 * @param numVertices The number of vertices to draw
			 * @param stride The stride to use (byte distance between the vertices).
			 * @param offset The offset to use in bytes.
			 */
			virtual void draw(void* vbo, int drawMode, int startVertex, int numVertices, unsigned int strideBytes, unsigned int offsetBytes) = 0;
			
			virtual ~GfxDevice() = 0 {};
		protected:
		private:
	};
};