// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"

namespace G2 
{
	/** This class defines...
	 * @created:	2014/02/12
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class Texture 
	{
		public:
			/** This constructs a new Texture.
			 */
			Texture(int type);
			
			/** This function binds the Texture into a specific Texture slot.
			 * @param textureSlot The TextureSlot to bind the Texture to. 
			 * @warning If you bind a Texture into a specific slot, be sure to unbind it from the same slot.
			 * Valid Slots are GEAR2::TEX_SLOT1, GEAR2::TEX_SLOT2, GEAR2::TEX_SLOT3, GEAR2::TEX_SLOT4, GEAR2::TEX_SLOT5, GEAR2::TEX_SLOT6, GEAR2::TEX_SLOT7, GEAR2::TEX_SLOT8
			 */
			void bind( unsigned textureSlot ) const;
			/** This function unbinds the Texture from a specific Texture slot.
			 * @param textureSlot The TextureSlot to bind the Texture to.
			 * @warning If you bind a Texture into a specific slot, be sure to unbind it from the same slot.
			 * Valid Slots are GEAR2::TEX_SLOT1, GEAR2::TEX_SLOT2, GEAR2::TEX_SLOT3, GEAR2::TEX_SLOT4, GEAR2::TEX_SLOT5, GEAR2::TEX_SLOT6, GEAR2::TEX_SLOT7, GEAR2::TEX_SLOT8
			 */
			void unbind( unsigned textureSlot ) const;

			/** normal destructor
			 */
			virtual ~Texture() = 0;
		protected:

			static GLuint numChannelsFromFormat( GLuint dstFormat );
			static GLuint convertFormat( GLuint baseFormat, bool compress );
			static bool mipMapsRequested(GLuint minFilter, GLuint magFilter);
			static void checkFilter(GLuint minFilter, GLuint magFilter);
			/** This function computes the compressed internal format from the corresponding internal base format if requested.
			 * @param baseFormat The requested base format.
			 * @param compress A flag indicating whether compression should be done or not.
			 * @return The corresponding compressed format if availavble and requested, the base format if compression is not requested or the corresponding compressed internal format is not available.
			 */
			static unsigned baseFormatToCompressedFormat( unsigned baseFormat, bool compress );
			

			unsigned int	mId;			// The OpenGL texture ID
			int				mType;			// The OpenGL texture ID
	};
};