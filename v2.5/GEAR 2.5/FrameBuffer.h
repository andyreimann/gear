#pragma once
#include "Defines.h"

#include <memory>
#include <string>

namespace G2 
{
	class Texture;

	namespace BufferAttachment
	{
		enum Name
		{
			COLOR_0 = GL_COLOR_ATTACHMENT0,
			COLOR_1 = GL_COLOR_ATTACHMENT1,
			COLOR_2 = GL_COLOR_ATTACHMENT2,
			COLOR_3 = GL_COLOR_ATTACHMENT3,
			COLOR_4 = GL_COLOR_ATTACHMENT4,
			COLOR_5 = GL_COLOR_ATTACHMENT5,
			COLOR_6 = GL_COLOR_ATTACHMENT6,
			COLOR_7 = GL_COLOR_ATTACHMENT7,
			COLOR_8 = GL_COLOR_ATTACHMENT8,
			COLOR_9 = GL_COLOR_ATTACHMENT9,
			COLOR_10 = GL_COLOR_ATTACHMENT10,
			COLOR_11 = GL_COLOR_ATTACHMENT11,
			COLOR_12 = GL_COLOR_ATTACHMENT12,
			COLOR_13 = GL_COLOR_ATTACHMENT13,
			COLOR_14 = GL_COLOR_ATTACHMENT14,
			COLOR_15 = GL_COLOR_ATTACHMENT15,
			DEPTH   = GL_DEPTH_ATTACHMENT,
			STENCIL = GL_STENCIL_ATTACHMENT,
			ATTACHMENT_INVALID = GL_INVALID_VALUE
		};

		BufferAttachment::Name getByOutputFormat(std::string const& outputFormat);
	};
	/// This class defines...
	/// @created:	2014/05/08
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class FrameBuffer 
	{
		friend class RenderTarget;
		public:
			/// This constructs a new FrameBuffer.
			FrameBuffer(unsigned width, unsigned height);
			/// Move ctor.
			FrameBuffer(FrameBuffer && rhs);
			/// Move ctor.
			FrameBuffer& operator=(FrameBuffer && rhs);
			/// This function attaches a Texture to a given attachment point.
			/// @param tex The Texture to attach. If you want to attach an empty Texture use the TextureManager::loadEmptyTexture function.
			/// @param attachment The attachment point to attach the Texture to. 
			///  Possible values are BufferAttachment::COLOR_0, BufferAttachment::COLOR_1, BufferAttachment::COLOR_2, BufferAttachment::COLOR_3, BufferAttachment::DEPTH, BufferAttachment::STENCIL. It is possible that your machine will support more than 4 color attachments. If so, use the constants GL_COLOR_ATTACHMENT4_EXT and so on. Use the getMaxColorAttachments function to get the number of color attachments of your machine.
			/// @param texTarget Specifies the texture target. Must be either the type of the texture itself or one of the following constants: G2::CUBE_MAP_POS_X, G2::CUBE_MAP_NEG_X, G2::CUBE_MAP_POS_Y, G2::CUBE_MAP_NEG_Y, G2::CUBE_MAP_POS_Z, G2::CUBE_MAP_NEG_Z.
			/// @param mipLevel The mip mapping level of the texture to attach to the given attachment point.
			/// @param layer the Z-Slice/layer of the Texture3D or TextureArray to attach.
			void attachTexture( std::shared_ptr<Texture> const& tex, BufferAttachment::Name attachment, int texTarget, int mipLevel = 0, int layer = 0 ) const;
			/// This function binds the FrameBuffer
			void bind() const;
			/// This function unbinds the FrameBuffer
			void unbind() const;

			~FrameBuffer();
		private:
			FrameBuffer() {}

			bool check() const;
			
			unsigned mId;				// The id of the FrameBuffer
			unsigned mWidth;			// The width of the FrameBuffer
			unsigned mHeight;			// The height of the FrameBuffer
			unsigned mDepthBufferId;	// The id of the attached DepthBuffer
	};
};