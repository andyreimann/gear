// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"

namespace G2 
{
	namespace WrapMode
	{
		enum Name {
			CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
			CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
			MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
			MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
			REPEAT = GL_REPEAT,
			NUM_WRAP_MODES,
			WRAP_MODE_INVALID = GL_INVALID_VALUE,
		};
		/** This function will parse the given string to the appropriate
		 * WrapMode enum value.
		 * @param name The name to parse.
		 * @return The parsed WrapMode enum value.
		 */
		WrapMode::Name getWrapMode(std::string const& name);
	};
	/** This class defines...
	 * @created:	2014/02/12
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class Texture 
	{
		friend class FrameBuffer;
		friend class RenderTarget;
		public:
			/** This constructs a new Texture.
			 */
			Texture(int type);
			/// Move ctor.
			Texture(Texture && rhs);
			/// Move ctor.
			Texture& operator=(Texture && rhs);
			
			/** This function binds the Texture into a specific Texture slot.
			 * @param textureSlot The TextureSlot to bind the Texture to. 
			 * @warning If you bind a Texture into a specific slot, be sure to unbind it from the same slot.
			 * Valid Slots are GEAR2::TEX_SLOT1, GEAR2::TEX_SLOT2, GEAR2::TEX_SLOT3, GEAR2::TEX_SLOT4, GEAR2::TEX_SLOT5, GEAR2::TEX_SLOT6, GEAR2::TEX_SLOT7, GEAR2::TEX_SLOT8
			 */
			virtual void bind( unsigned textureSlot ) const;
			/** This function unbinds the Texture from a specific Texture slot.
			 * @param textureSlot The TextureSlot to bind the Texture to.
			 * @warning If you bind a Texture into a specific slot, be sure to unbind it from the same slot.
			 * Valid Slots are GEAR2::TEX_SLOT1, GEAR2::TEX_SLOT2, GEAR2::TEX_SLOT3, GEAR2::TEX_SLOT4, GEAR2::TEX_SLOT5, GEAR2::TEX_SLOT6, GEAR2::TEX_SLOT7, GEAR2::TEX_SLOT8
			 */
			virtual void unbind( unsigned textureSlot ) const;
			/** Returns the width of the Texture
			 * @return The width of the Texture
			 */
			virtual unsigned getWidth() = 0;
			/** Returns the height of the Texture
			 * @return The height of the Texture
			 */
			virtual unsigned getHeight() = 0;
			/** Returns the depth of the Texture
			 * @return The depth of the Texture
			 */
			virtual unsigned getDepth() = 0;

			/** normal destructor
			 */
			virtual ~Texture() = 0;

			static unsigned getFormatByString(std::string const& format);
		protected:
			Texture() {}

			static GLuint numChannelsFromFormat( GLuint dstFormat );
			static GLuint convertFormat( GLuint baseFormat, bool compress );
			static GLuint isCompressedFormat(GLuint baseFormat);
			static bool mipMapsRequested(GLuint minFilter, GLuint magFilter);
			static void checkFilter(GLuint minFilter, GLuint magFilter);
			/** This function computes the compressed internal format from the corresponding internal base format if requested.
			 * @param baseFormat The requested base format.
			 * @param compress A flag indicating whether compression should be done or not.
			 * @return The corresponding compressed format if availavble and requested, the base format if compression is not requested or the corresponding compressed internal format is not available.
			 */
			static unsigned baseFormatToCompressedFormat( unsigned baseFormat, bool compress );
			

			unsigned int	mId;			// The OpenGL texture ID
			int				mType;			// The OpenGL texture type
			
			WrapMode::Name  mWrapModeS;		// The textures wrap mode for s coordinates
			WrapMode::Name  mWrapModeT;		// The textures wrap mode for s coordinates
			WrapMode::Name  mWrapModeR;		// The textures wrap mode for s coordinates
	};
};