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
			/// @note The Attachment request will be ignored if there is already a TextureArray attached to the given attachment point.
			void attachTexture( std::shared_ptr<Texture> const& tex, BufferAttachment::Name attachment, int texTarget, int mipLevel = 0 ) const;
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