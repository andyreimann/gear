// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"

#include <G2Core/Defines.h>

namespace G2 
{
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
			Texture(G2Core::TextureFormat::Name type, 
				    G2Core::DataFormat::Name dataFormat,
					G2Core::FilterMode::Name minFilter, 
					G2Core::FilterMode::Name magFilter);
			/// Move ctor.
			Texture(Texture && rhs);
			/// Move ctor.
			Texture& operator=(Texture && rhs);
			
			/** This function binds the Texture into a specific Texture slot.
			 * @param textureSlot The TextureSlot to bind the Texture to. 
			 * @warning If you bind a Texture into a specific slot, be sure to unbind it from the same slot.
			 */
			virtual void bind( G2Core::TexSlot::Name textureSlot ) const;
			/** This function unbinds the Texture from a specific Texture slot.
			 * @param textureSlot The TextureSlot to bind the Texture to.
			 * @warning If you bind a Texture into a specific slot, be sure to unbind it from the same slot.
			 */
			virtual void unbind( G2Core::TexSlot::Name textureSlot ) const;
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
			/** Returns the type of the Texture
			 * @return The type of the Texture
			 */
			G2Core::TextureFormat::Name getType() { return mType; }
			/** Returns the format of the internal data contained in the texture.
			 * @return The format of the internal data contained in the texture.
			 */
			G2Core::DataFormat::Name getDataFormat() { return mDataFormat; }

			/** normal destructor
			 */
			virtual ~Texture() = 0;

			static G2Core::DataFormat::Name getFormatByString(std::string const& format);
		protected:
			Texture() {}

			static unsigned int numChannelsFromFormat(G2Core::DataFormat::Name dstFormat );
			static unsigned int convertFormat( G2Core::DataFormat::Name baseFormat, bool compress );
			static bool isCompressedFormat(G2Core::DataFormat::Name baseFormat);
			static bool mipMapsRequested(G2Core::FilterMode::Name minFilter, G2Core::FilterMode::Name magFilter);
			static void checkFilter(G2Core::FilterMode::Name minFilter, G2Core::FilterMode::Name magFilter);
			/** This function computes the compressed internal format from the corresponding internal base format if requested.
			 * @param baseFormat The requested base format.
			 * @param compress A flag indicating whether compression should be done or not.
			 * @return The corresponding compressed format if available and requested, the base format if compression is not requested or the corresponding compressed internal format is not available.
			 */
			static unsigned baseFormatToCompressedFormat( G2Core::DataFormat::Name baseFormat, bool compress );
			

			G2Core::GfxResource* mTexResource;

			G2Core::TextureFormat::Name	mType;			// The texture type
			G2Core::DataFormat::Name	mDataFormat;	// The format of the internal data contained in the texture
			
			G2Core::FilterMode::Name mMinFilter;		// The type of min filter, the texture uses
			G2Core::FilterMode::Name mMagFilter;		// The type of mag filter, the texture uses			
			bool					 mUseMipMaps;		// If true mip maps are applied to the texture
	};
};