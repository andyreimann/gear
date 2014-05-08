#include "FrameBuffer.h"
#include "Logger.h"

using namespace G2;

FrameBuffer::FrameBuffer(unsigned width, unsigned height) 
	: mDepthBufferId(GL_INVALID_VALUE),
	mWidth(width),
	mHeight(height)
{
	GLDEBUG( glGenFramebuffers( 1, &mId ) );
	bind();
	GLDEBUG( glGenRenderbuffers( 1, &mDepthBufferId ) );
	// Bind depth buffer
	GLDEBUG( glBindRenderbuffer(GL_RENDERBUFFER, mDepthBufferId) );
	// Allocate memory for renderBuffer
	GLDEBUG( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight ) );
	// unbind depth buffer
	GLDEBUG( glBindRenderbuffer(GL_RENDERBUFFER, mDepthBufferId) );
	// attach depth buffer to frame buffer
	GLDEBUG( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBufferId) );
	// done attaching
	unbind();
}

FrameBuffer::FrameBuffer(FrameBuffer && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer && rhs)
{
	// 1. Stage: delete maybe allocated resources on target type
	GLDEBUG( glDeleteFramebuffers( 1, &mId ) );
	if(mDepthBufferId != GL_INVALID_VALUE)
	{
		GLDEBUG( glDeleteRenderbuffers( 1, &mDepthBufferId ) );
	}
	// 2. Stage: transfer data from src to target
	mId = std::move(rhs.mId);
	mDepthBufferId = std::move(rhs.mDepthBufferId);
	// 3. Stage: modify src to a well defined state
	rhs.mId = GL_INVALID_VALUE;
	rhs.mDepthBufferId = GL_INVALID_VALUE;
	return *this;
}

FrameBuffer::~FrameBuffer() 
{
	GLDEBUG( glDeleteFramebuffers( 1, &mId ) );
	if(mDepthBufferId != GL_INVALID_VALUE)
	{
		GLDEBUG( glDeleteRenderbuffers( 1, &mDepthBufferId ) );
	}
}

void
FrameBuffer::bind() const
{
	GLDEBUG( glBindFramebuffer(GL_FRAMEBUFFER, mId) );
}

void
FrameBuffer::unbind() const
{
	GLDEBUG( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
}

void
FrameBuffer::attachTexture(Texture const& tex, BufferAttachment::Name attachment /*= BufferAttachment::COLOR_0*/, int mipLevel /*= 0 */) 
{
	bind();

	if(tex.mType == GL_TEXTURE_1D)
	{
		GLDEBUG( glFramebufferTexture1D( GL_FRAMEBUFFER, attachment,  tex.mType, tex.mId, mipLevel ) );
	}
	else if(tex.mType == GL_TEXTURE_3D)
	{
		int zSlice = 0;
		GLDEBUG( glFramebufferTexture3D( GL_FRAMEBUFFER, attachment,  tex.mType, tex.mId, mipLevel, zSlice ) );
	}
	else
	{
		// Default is GL_TEXTURE_2D, GL_TEXTURE_RECTANGLE_ARB, or cube faces
		GLDEBUG( glFramebufferTexture2D( GL_FRAMEBUFFER, attachment,  tex.mType, tex.mId, mipLevel ) );
	}
	check();
}

bool
FrameBuffer::check() 
{
	GLDEBUG( unsigned status = glCheckFramebufferStatus(GL_FRAMEBUFFER) );
	if( GL_FRAMEBUFFER_COMPLETE != status) {
		switch(status) {
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			  logger << "[FBO] state check failed: Not all framebuffer attachment points are framebuffer attachment complete. \n"
				<< "This means that at least one attachment point with a renderbuffer or texture\n"
				<< "attached has its attached object no longer in existence or has an attached \n"
				<< "image with a width or height of zero, or the color attachment point has a \n"
				<< "non-color-renderable image attached, or the"
				<< "depth attachment point has a non-depth-renderable image attached, \n"
				<< "or the stencil attachment point has a non-stencil-renderable image \n"
				<< "attached Color-renderable formats include GL_RGBA4, GL_RGB5_A1, and GL_RGB565. GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT\n";
			  break;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			  logger << "[FBO] state check failed: Not all attached images have the same width and height. GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\n";
			  break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			  logger << "[FBO] state check failed: No images are attached to the framebuffer. GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT\n";
			  break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
			  logger << "[FBO] state check failed: The combination of internal formats of the attached images violates an \n"
						<< "implementation-dependent set of restrictions. GL_FRAMEBUFFER_UNSUPPORTED_EXT\n";
			  break;
			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			  logger << "[FBO] state check failed: GL_FRAMEBUFFER_INCOMPLETE_FORMATS\n";
			  break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			  logger << "[FBO] state check failed: An attachment point referenced by glDrawBuffers() doesn’t have an attachment.GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n";
			  break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			  logger << "[FBO] state check failed: The attachment point referenced by glReadBuffers() doesn’t have an attachment. GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n";
			  break;
			default:
			  logger << "[FBO] This particular FBO configuration is not supported by the implementation.\n";
		}
		return false;
	}
	return true;
}