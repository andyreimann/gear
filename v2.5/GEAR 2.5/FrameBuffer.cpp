#include "FrameBuffer.h"
#include "Logger.h"
#include "Texture.h"

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
	mWidth = rhs.mWidth;
	mHeight = rhs.mHeight;
	// 3. Stage: modify src to a well defined state
	rhs.mId = GL_INVALID_VALUE;
	rhs.mDepthBufferId = GL_INVALID_VALUE;
	rhs.mWidth = rhs.mHeight = 0;
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
FrameBuffer::attachTexture(std::shared_ptr<Texture> const& tex, BufferAttachment::Name attachment /*= BufferAttachment::COLOR_0*/, int texTarget, int mipLevel /*= 0 */) const
{
	bind();

	if(tex->mType == GL_TEXTURE_1D)
	{
		GLDEBUG( glFramebufferTexture1D( GL_FRAMEBUFFER, attachment,  texTarget, tex->mId, mipLevel ) );
	}
	else if(tex->mType == GL_TEXTURE_3D)
	{
		int zSlice = 0;
		GLDEBUG( glFramebufferTexture3D( GL_FRAMEBUFFER, attachment,  texTarget, tex->mId, mipLevel, zSlice ) );
	}
	else
	{
		// Default is GL_TEXTURE_2D, GL_TEXTURE_RECTANGLE_ARB, or cube faces
		GLDEBUG( glFramebufferTexture2D( GL_FRAMEBUFFER, attachment,  texTarget, tex->mId, mipLevel ) );
	}
	check();
}

bool
FrameBuffer::check() const
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

BufferAttachment::Name
BufferAttachment::getByOutputFormat(std::string const& outputFormat) 
{
	if(outputFormat == "RGB") 
	{
		return COLOR_0;
	}
	else if(outputFormat == "RGBA") 
	{
		return COLOR_0;
	}
	else if(outputFormat == "DEPTH") 
	{
		return DEPTH;
	}
	else if(outputFormat == "STENCIL") 
	{
		return STENCIL;
	}

	
	if(outputFormat == "RGB") { return COLOR_0; }
	if(outputFormat == "RGB_UB") { return COLOR_0; }
	if(outputFormat == "RGB_US") { return COLOR_0; }
	if(outputFormat == "RGBA") { return COLOR_0; }
	if(outputFormat == "RGBA_UB") { return COLOR_0; }
	if(outputFormat == "RGBA_US") { return COLOR_0; }
	if(outputFormat == "RGBA16_F") { return COLOR_0; }
	if(outputFormat == "RGBA_F") { return COLOR_0; }
	if(outputFormat == "RGBA_B") { return COLOR_0; }
	if(outputFormat == "RGBA_S") { return COLOR_0; }
	if(outputFormat == "RGBA_I") { return COLOR_0; }
	if(outputFormat == "RGBA_UI") { return COLOR_0; }
	if(outputFormat == "ALPHA_UB") { return COLOR_0; }
	if(outputFormat == "ALPHA_US") { return COLOR_0; }
	if(outputFormat == "ALPHA16_F") { return COLOR_0; }
	if(outputFormat == "ALPHA_B") { return COLOR_0; }
	if(outputFormat == "ALPHA_S") { return COLOR_0; }
	if(outputFormat == "ALPHA_I") { return COLOR_0; }
	if(outputFormat == "ALPHA_UI") { return COLOR_0; }
	if(outputFormat == "DEPTH") { return DEPTH; }
	if(outputFormat == "DEPTH16") { return DEPTH; }
	if(outputFormat == "DEPTH24") { return DEPTH; }
	if(outputFormat == "DEPTH32") { return DEPTH; }
	if(outputFormat == "LUMINANCE") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA_UB") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA_US") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA16_F") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA_F") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA_B") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA_S") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA_I") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA_UI") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_UB") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_US") { return COLOR_0; }
	if(outputFormat == "LUMINANCE16_F") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_F") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_B") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_S") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_I") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_UI") { return COLOR_0; }
	if(outputFormat == "INTENSITY") { return COLOR_0; }
	if(outputFormat == "INTENSITY_UB") { return COLOR_0; }
	if(outputFormat == "INTENSITY_US") { return COLOR_0; }
	if(outputFormat == "INTENSITY16_F") { return COLOR_0; }
	if(outputFormat == "INTENSITY_F") { return COLOR_0; }
	if(outputFormat == "INTENSITY_B") { return COLOR_0; }
	if(outputFormat == "INTENSITY_S") { return COLOR_0; }
	if(outputFormat == "INTENSITY_I") { return COLOR_0; }
	if(outputFormat == "INTENSITY_UI") { return COLOR_0; }

	return ATTACHMENT_INVALID;
}

