#include "MultipleRenderTarget.h"
#include "Texture2D.h"

using namespace G2;

MultipleRenderTarget::MultipleRenderTarget(unsigned int width, unsigned int height, G2Core::DataFormat::Internal::Name format)
	: mWidth(width),
	mHeight(height),
	mDataFormat(format),
	mFrameBuffer(width,height,format)
{
}

void
MultipleRenderTarget::allocateRenderTexture(G2Core::FrameBufferAttachmentPoint::Name attachmentPoint) 
{
	// allocate new texture
	mRenderTextures[attachmentPoint] = std::shared_ptr<Texture>(
		new Texture2D(
			G2Core::FilterMode::NEAREST, 
			G2Core::FilterMode::NEAREST, 
			mWidth, 
			mHeight, 
			Texture::getBaseFormatByInternalFormat(mDataFormat),
			mDataFormat,
			G2Core::WrapMode::REPEAT,
			G2Core::WrapMode::REPEAT, 
			false,
			G2Core::DataType::UNSIGNED_BYTE,
			nullptr
		)
	);
	mFrameBuffer.bind();
	mFrameBuffer.attachTexture(mRenderTextures[attachmentPoint], attachmentPoint, G2Core::TextureFormat::TEXTURE_2D);
	mFrameBuffer.unbind();
}