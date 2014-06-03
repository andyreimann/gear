// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Texture.h"
#include "ResourceBuilder.h"

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace G2 
{
	/** This class defines a Texture in 2-Dimensional texture space.
	 * @created:	2014/02/17
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class TextureArray : public Texture
	{
		friend class RenderTarget;
		public:

			/** This constructs a new Texture2D from the given data.
			 * @param minFilter The min filter to use.
			 * @param magFilter The mag filter to use.
			 * @param width The width  to use.
			 * @param height The height to use.
			 * @param height The height to use.
			 * @param size The actual number of elements in the TextureArray for this texture (so that it actually becomes a texture array)
			 * @param compress The compress flag to use.
			 * @param data The pointer to the initial data or nullptr.
			 */
			TextureArray(unsigned int minFilter, 
					  unsigned int magFilter, 
					  unsigned int width,
					  unsigned int height,
					  unsigned int size,
					  unsigned int format,
					  bool compress,
					  unsigned char * data = nullptr);
			/// Move ctor.
			TextureArray(TextureArray && rhs);
			/// Move ctor.
			TextureArray& operator=(TextureArray && rhs);
			/** Returns the width of the Texture
			 * @return The width of the Texture
			 */
			virtual unsigned getWidth() { return mWidth; }
			/** Returns the height of the Texture
			 * @return The height of the Texture
			 */
			virtual unsigned getHeight() { return mHeight; }
			/** Returns the depth of the Texture
			 * @return The depth of the Texture
			 * @note The TextureArray object will return the number of textures managed.
			 */
			virtual unsigned getDepth() { return mDepth; }
			
			static void		init();
		private:
			TextureArray() {}
			

			static bool		gInitialized;
			unsigned int	mMinFilter;		// The type of min filter, the texture uses
			unsigned int	mMagFilter;		// The type of mag filter, the texture uses			
			unsigned		mWidth;			// The height of the Texture.
			unsigned		mHeight;		// The width of the Texture.
			unsigned		mDepth;		// The width of the Texture.
			unsigned		mChannels;		// The number of channels of the Texture
			int				mBytes;			// The memory usage of the texture in bytes
			glm::mat4		mTextureMatrix;	// The Texture Matrix to use for the Texture
			bool			mCompressed;	// Indicates whether the Texture is compressed or not
			bool			mUseMipMaps;	// If true mip maps are applied to the texture
	};
};