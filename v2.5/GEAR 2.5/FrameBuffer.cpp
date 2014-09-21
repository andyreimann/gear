#include "FrameBuffer.h"
#include "Logger.h"
#include "Texture.h"

#include <G2Core/GfxDevice.h>

using namespace G2;

FrameBuffer::FrameBuffer(unsigned width, unsigned height, G2Core::DataFormat::Name format) 
	: mRenderTargetResource(G2_gfxDevice()->createRenderTarget(width, height, format))
{
}

FrameBuffer::FrameBuffer(FrameBuffer && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer && rhs)
{
	// 1. Stage: delete maybe allocated resources on target type
	// 2. Stage: transfer data from src to target
	mRenderTargetResource = rhs.mRenderTargetResource;
	// 3. Stage: modify src to a well defined state
	rhs.mRenderTargetResource = nullptr;
	return *this;
}

FrameBuffer::~FrameBuffer() 
{
	if(mRenderTargetResource != nullptr)
	{
		G2_gfxDevice()->freeGfxResource(mRenderTargetResource);
	}
}

void
FrameBuffer::bind() const
{
	G2_gfxDevice()->bindRenderTarget(mRenderTargetResource);
}

void
FrameBuffer::unbind() const
{
	G2_gfxDevice()->unbindRenderTarget(mRenderTargetResource);
}

void
FrameBuffer::attachTexture(std::shared_ptr<Texture> const& tex, G2Core::FrameBufferAttachmentPoint::Name attachment, G2Core::TextureFormat::Name texTarget, int mipLevel, int layer) const
{
	G2_gfxDevice()->attachTextureToRenderTarget(mRenderTargetResource, tex->mTexResource, attachment, texTarget, mipLevel, layer);
}

