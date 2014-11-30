#pragma once
#include "Defines.h"

#include <memory>
#include <string>

#include <G2Core/Defines.h>

namespace G2 
{
	class Texture;

	/// This class can configure a buffer together with textures to catch a rendered image in this textures.
	/// @created:	2014/05/08
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class FrameBuffer 
	{
		friend class RenderTarget;
		public:
			/// This constructs a new FrameBuffer.
			FrameBuffer(unsigned width, unsigned height, G2Core::DataFormat::Internal::Name format);
			/// Move ctor.
			FrameBuffer(FrameBuffer && rhs);
			/// Move ctor.
			FrameBuffer& operator=(FrameBuffer && rhs);
			/// This function attaches a Texture to a given attachment point.
			/// @param tex The Texture to attach. If you want to attach an empty Texture use the TextureManager::loadEmptyTexture function.
			/// @param attachment The attachment point to attach the Texture to. 
			///  Possible values are 
			///		G2Core::FrameBufferAttachmentPoint::COLOR_0,
			///		G2Core::FrameBufferAttachmentPoint::COLOR_1, 
			///		G2Core::FrameBufferAttachmentPoint::COLOR_2, 
			///		G2Core::FrameBufferAttachmentPoint::COLOR_3, 
			///		G2Core::FrameBufferAttachmentPoint::DEPTH, 
			///		G2Core::FrameBufferAttachmentPoint::STENCIL. 
			///	It is possible that your machine will support more than 4 color attachments. If so, use the constants GL_COLOR_ATTACHMENT4_EXT and so on. Use the getMaxColorAttachments function to get the number of color attachments of your machine.
			/// @param texFormat Specifies the target texture format. Must be either the type of the texture itself or one of the following constants: G2Core::TextureFormat::CUBE_MAP_POS_X, G2Core::TextureFormat::CUBE_MAP_NEG_X, G2Core::TextureFormat::CUBE_MAP_POS_Y, G2Core::TextureFormat::CUBE_MAP_NEG_Y, G2Core::TextureFormat::CUBE_MAP_POS_Z, G2Core::TextureFormat::CUBE_MAP_NEG_Z.
			/// @param mipLevel The mip mapping level of the texture to attach to the given attachment point.
			/// @param layer the Z-Slice/layer of the Texture3D or TextureArray to attach.
			/// @note Make sure to call bind() before you attach a texture.
			void attachTexture( std::shared_ptr<Texture> const& tex, G2Core::FrameBufferAttachmentPoint::Name attachment, G2Core::TextureFormat::Name texFormat, int mipLevel = 0, int layer = 0 ) const;
			/// This function binds the FrameBuffer
			void bind() const;
			/// This function unbinds the FrameBuffer
			void unbind() const;

			~FrameBuffer();
		private:
			FrameBuffer() {}
			
			G2Core::GfxResource*		mRenderTargetResource;	// The resource handle for the graphics layer
	};
};