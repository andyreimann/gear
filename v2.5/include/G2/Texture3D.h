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
		friend class RenderTarget;
		public:
			/** This constructs a new Texture3D.
			 */
			Texture3D(G2Core::FilterMode::Name minFilter, 
					  G2Core::FilterMode::Name magFilter, 
					  unsigned int width,
					  unsigned int height,
					  unsigned int depth,
					  G2Core::DataFormat::Internal::Name format,
					  bool compress,
					  G2Core::DataType::Name dataType,
					  void* data);
			/// Move ctor.
			Texture3D(Texture3D && rhs);
			/// Move ctor.
			Texture3D& operator=(Texture3D && rhs);
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
			virtual unsigned getDepth() { return mDepth; }
			/** normal destructor
			 */
			virtual ~Texture3D();
		private:

			static bool		gInitialized;
			static void		init();
	
			unsigned		mWidth;			// The height of the Texture.
			unsigned		mHeight;		// The width of the Texture.
			unsigned		mDepth;			// The depth of the Texture.
			unsigned		mChannels;		// The number of channels of the Texture
			int				mBytes;			// The memory usage of the texture in bytes
			glm::mat4		mTextureMatrix;	// The Texture Matrix to use for the Texture
			bool			mCompressed;	// Indicates whether the Texture is compressed or not
	};
};