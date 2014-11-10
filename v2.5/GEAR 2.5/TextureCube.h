#pragma once
#include "Texture.h"

#include <glm/glm.hpp>

namespace G2 
{
	/// This class defines...
	/// @created:	2014/05/09
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class TextureCube : public Texture
	{
		public:
			/** This constructs a new Texture2D from the given data.
			 */
			TextureCube(G2Core::FilterMode::Name minFilter, 
						G2Core::FilterMode::Name magFilter, 
						unsigned int width,
						unsigned int height,
						G2Core::DataFormat::Internal::Name format,
						bool compress,
						void* data = nullptr);
			/// Move ctor.
			TextureCube(TextureCube && rhs);
			/// Move ctor.
			TextureCube& operator=(TextureCube && rhs);
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
			 */
			virtual unsigned getDepth() { return 1; }
		private:
			TextureCube() {}

			unsigned		mWidth;			// The height of the Texture.
			unsigned		mHeight;		// The width of the Texture.
			unsigned		mChannels;		// The number of channels of the Texture
			int				mBytes;			// The memory usage of the texture in bytes
			glm::mat4		mTextureMatrix;	// The Texture Matrix to use for the Texture
			bool			mCompressed;	// Indicates whether the Texture is compressed or not
	};
};