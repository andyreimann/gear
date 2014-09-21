#include "GlGfxApi.h"
#include "GlGfxData.h"
#include "GlGfxMappings.h"

#include <G2/Logger.h>

bool _check()
{
	GLCHECK( unsigned status = glCheckFramebufferStatus(GL_FRAMEBUFFER) );
	if( GL_FRAMEBUFFER_COMPLETE != status) {
		switch(status) {
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			  G2::logger << "[FBO] state check failed: Not all framebuffer attachment points are framebuffer attachment complete. \n"
				<< "This means that at least one attachment point with a renderbuffer or texture\n"
				<< "attached has its attached object no longer in existence or has an attached \n"
				<< "image with a width or height of zero, or the color attachment point has a \n"
				<< "non-color-renderable image attached, or the"
				<< "depth attachment point has a non-depth-renderable image attached, \n"
				<< "or the stencil attachment point has a non-stencil-renderable image \n"
				<< "attached Color-renderable formats include GL_RGBA4, GL_RGB5_A1, and GL_RGB565. GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT\n";
			  break;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			  G2::logger << "[FBO] state check failed: Not all attached images have the same width and height. GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\n";
			  break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			  G2::logger << "[FBO] state check failed: No images are attached to the framebuffer. GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT\n";
			  break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
			  G2::logger << "[FBO] state check failed: The combination of internal formats of the attached images violates an \n"
						<< "implementation-dependent set of restrictions. GL_FRAMEBUFFER_UNSUPPORTED_EXT\n";
			  break;
			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			  G2::logger << "[FBO] state check failed: GL_FRAMEBUFFER_INCOMPLETE_FORMATS\n";
			  break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			  G2::logger << "[FBO] state check failed: An attachment point referenced by glDrawBuffers() doesn’t have an attachment.GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n";
			  break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			  G2::logger << "[FBO] state check failed: The attachment point referenced by glReadBuffers() doesn’t have an attachment. GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n";
			  break;
			default:
			  G2::logger << "[FBO] This particular FBO configuration is not supported by the implementation.\n";
		}
		return false;
	}
	return true;
}

G2Core::GfxResource* CreateRenderTarget(unsigned int width, unsigned int height, G2Core::DataFormat::Name format)
{
	unsigned int fboId, renderBufferId;
	GLCHECK( glGenFramebuffers( 1, &fboId ) );

	GLCHECK( glBindFramebuffer(GL_FRAMEBUFFER, fboId) );

	GLCHECK( glGenRenderbuffers( 1, &renderBufferId ) );
	// Bind depth buffer
	GLCHECK( glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId) );
	// Allocate memory for renderBuffer
	GLCHECK( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height ) );
	// unbind depth buffer
	GLCHECK( glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId) );
	// attach depth buffer to frame buffer
	GLCHECK( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferId) );
	// done attaching
	GLCHECK( glBindFramebuffer(GL_FRAMEBUFFER, 0) );

	G2GL::RenderTargetResource* renderTarget = new G2GL::RenderTargetResource(width, height, format, fboId, renderBufferId);
	return renderTarget;
}

void AttachTextureToRenderTarget(G2Core::GfxResource* renderTarget, G2Core::GfxResource* texture, G2Core::FrameBufferAttachmentPoint::Name attachment, G2Core::TextureFormat::Name texTarget, int mipmapLevel, int layer)
{
	G2GL::TextureResource* tex = static_cast<G2GL::TextureResource*>(texture);
	if(tex->texType == GL_TEXTURE_1D)
	{
		GLCHECK( glFramebufferTexture1D( GL_FRAMEBUFFER, toGlFrameBufferAttachmentPoint(attachment),  toGlTextureFormat(texTarget), tex->texId, mipmapLevel ) );
	}
	else if(tex->texType == GL_TEXTURE_3D)
	{
		GLCHECK( glFramebufferTexture3D( GL_FRAMEBUFFER, toGlFrameBufferAttachmentPoint(attachment),  toGlTextureFormat(texTarget), tex->texId, mipmapLevel, layer ) );
	}
	else if(tex->texType == GL_TEXTURE_2D_ARRAY)
	{
		GLCHECK( glFramebufferTextureLayer( GL_FRAMEBUFFER, toGlFrameBufferAttachmentPoint(attachment),  tex->texId, mipmapLevel, layer ) );
	}
	else
	{
		// Default is GL_TEXTURE_2D, GL_TEXTURE_RECTANGLE_ARB, or cube faces
		GLCHECK( glFramebufferTexture2D( GL_FRAMEBUFFER, toGlFrameBufferAttachmentPoint(attachment),  toGlTextureFormat(texTarget), tex->texId, mipmapLevel ) );
	}
	_check();
}

void BindRenderTarget(G2Core::GfxResource* renderTarget)
{
	G2GL::RenderTargetResource* renderTargetPtr = static_cast<G2GL::RenderTargetResource*>(renderTarget);
	GLCHECK( glBindFramebuffer(GL_FRAMEBUFFER, renderTargetPtr->fboId) );
}

void UnbindRenderTarget(G2Core::GfxResource* renderTarget)
{
	GLCHECK( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
}
