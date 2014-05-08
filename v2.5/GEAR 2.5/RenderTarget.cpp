#include "RenderTarget.h"
#include "Defines.h"

using namespace G2;

RenderTarget::RenderTarget(Sampler::Name targetSampler, unsigned width, unsigned height) 
	: mRenderTextureSampler(targetSampler), 
	mRenderTexture(NEAREST, NEAREST, width, height, RGBA, false),
	mFrameBuffer(width,height)
{
	mFrameBuffer.attachTexture(mRenderTexture);
}

RenderTarget::RenderTarget(RenderTarget && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

RenderTarget& RenderTarget::operator=(RenderTarget && rhs)
{
	// 1. Stage: delete maybe allocated resources on target type
	// nothing here
	// 2. Stage: transfer data from src to target
	mRenderTextureSampler = rhs.mRenderTextureSampler;
	mRenderTexture = std::move(rhs.mRenderTexture);
	mFrameBuffer = std::move(rhs.mFrameBuffer);
	// 3. Stage: modify src to a well defined state
	rhs.mRenderTextureSampler = Sampler::SAMPLER_INVALID;
	return *this;
}

void
RenderTarget::bind() const
{
	mFrameBuffer.bind();
	GLDEBUG( glClearColor(0.3f,0.0f,0.0f,1.0f) );
	GLDEBUG( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
}

void
RenderTarget::unbind() const
{
	mFrameBuffer.unbind();
}