// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Texture.h"

#include <glm/glm.hpp>

namespace G2 
{
	/** This class defines...
	 * @created:	2014/02/12
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class Texture3D : public Texture
	{
		public:
			/** This constructs a new Texture3D.
			 */
			Texture3D(unsigned int minFilter, 
					  unsigned int magFilter, 
					  unsigned int width,
					  unsigned int height,
					  unsigned int depth,
					  unsigned int format,
					  bool compress,
					  unsigned char * data);
			/** normal destructor
			 */
			virtual ~Texture3D();
		private:

			static bool		gInitialized;
			static void		init();

			unsigned int	mMinFilter;		// The type of min filter, the texture uses
			unsigned int	mMagFilter;		// The type of mag filter, the texture uses			
			const unsigned	mWidth;			// The height of the Texture.
			const unsigned	mHeight;		// The width of the Texture.
			const unsigned	mDepth;			// The depth of the Texture.
			const unsigned	mChannels;		// The number of channels of the Texture
			int				mBytes;			// The memory usage of the texture in bytes
			glm::mat4		mTextureMatrix;	// The Texture Matrix to use for the Texture
			bool			mCompressed;	// Indicates whether the Texture is compressed or not
			bool			mUseMipMaps;	// If true mipmaps are applied to the texture
	};
};