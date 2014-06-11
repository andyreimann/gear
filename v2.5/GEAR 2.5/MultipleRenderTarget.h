#pragma once
#include "FrameBuffer.h"
#include "Texture.h"

#include <unordered_map>
#include <memory>

namespace G2 
{
	/// This class handles a render target, which can be used to render to multiple Texture2D objects
	/// @created:	2014/06/05
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class MultipleRenderTarget 
	{
			typedef std::unordered_map<BufferAttachment::Name,std::shared_ptr<Texture>> RenderTextureMap;
		public:
			/// This constructs a new MultipleRenderTarget.
			MultipleRenderTarget(unsigned int width, unsigned int height);
			/** This function will allocate a new Texture and attach it to the given
			 * attachment point.
			 * @param attachmentPoint The attachment point to attach it to.
			 */
			void allocateRenderTexture(BufferAttachment::Name attachment = BufferAttachment::COLOR_0);
			
			void bind() const { mFrameBuffer.bind(); }
			void unbind() const { mFrameBuffer.unbind(); }

			std::shared_ptr<Texture> getRenderTexture(BufferAttachment::Name attachment) { return mRenderTextures[attachment]; }
		private:
			unsigned int		mWidth;
			unsigned int		mHeight;
			FrameBuffer			mFrameBuffer;		// The framebuffer, the render textures are attached to
			RenderTextureMap	mRenderTextures;	// The attached render textures

	};
};