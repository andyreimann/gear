#include "MultipleRenderTarget.h"
#include "Texture2D.h"

using namespace G2;

MultipleRenderTarget::MultipleRenderTarget(unsigned int width, unsigned int height)
	: mWidth(width),
	mHeight(height),
	mFrameBuffer(width,height)
{
}

void
MultipleRenderTarget::allocateRenderTexture(BufferAttachment::Name attachmentPoint) 
{
	// allocate new texture
	mRenderTextures[attachmentPoint] = std::shared_ptr<Texture>(
		new Texture2D(
			NEAREST, 
			NEAREST, 
			mWidth, 
			mHeight, 
			RGBA, 
			false,
			nullptr
		)
	);
	mFrameBuffer.attachTexture(mRenderTextures[attachmentPoint], attachmentPoint, GL_TEXTURE_2D);
}