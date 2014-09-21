// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Texture.h"
#include "ResourceBuilder.h"

#include <string>
#include <glm/glm.hpp>

namespace G2 
{
	/** This class defines a Texture in 2-Dimensional texture space.
	 * @created:	2014/02/17
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class Texture2D : public Texture
	{
		friend class RenderTarget;
		public:

			struct Builder : public ResourceBuilder<Builder,Texture2D>
			{

				std::shared_ptr<Texture2D> buildResource(
					G2Core::FilterMode::Name minFilter, 
					G2Core::FilterMode::Name magFilter, 
					bool compress, 
					G2Core::WrapMode::Name wrapS, 
					G2Core::WrapMode::Name wrapT,
					G2Core::DataFormat::Name internalFormat);
				~Builder();

				unsigned id; // The IL image id
			};

			/** This constructs a new Texture2D from the given data.
			 * @param minFilter The min filter to use.
			 * @param magFilter The mag filter to use.
			 * @param width The width  to use.
			 * @param height The height to use.
			 * @param format The format of the incoming data.
			 * @param internalFormat The internal format to interpret it as. Set this value to -1 if you want to choose the same internal format as given in 'format'
			 * @param compress The compress flag to use.
			 * @param data The pointer to the initial data or nullptr.
			 * @note If you don't want to use the internal compression method, just pass in a compressed internal format.
			 * If the supplied internal format is already a compressed format, it does not matter what value you set the compressed parameter to!
			 */
			Texture2D(G2Core::FilterMode::Name minFilter, 
					  G2Core::FilterMode::Name magFilter, 
					  unsigned int width,
					  unsigned int height,
					  G2Core::DataFormat::Name format,
					  G2Core::DataFormat::Name internalFormat,
					  G2Core::WrapMode::Name wrapS,
					  G2Core::WrapMode::Name wrapT,
					  bool compress,
					  unsigned char * data = nullptr);
			/// Move ctor.
			Texture2D(Texture2D && rhs);
			/// Move ctor.
			Texture2D& operator=(Texture2D && rhs);
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
			/** This function will return the Channels. 
			* @return The Channels.
			*/
			unsigned const& getChannels() const { return mChannels; }
			
			static void		init();
		private:
			Texture2D() {}
			
			static bool		gInitialized;

			unsigned		mWidth;			// The height of the Texture.
			unsigned		mHeight;		// The width of the Texture.
			unsigned		mChannels;		// The number of channels of the Texture
			int				mBytes;			// The memory usage of the texture in bytes
			glm::mat4		mTextureMatrix;	// The Texture Matrix to use for the Texture
			bool			mCompressed;	// Indicates whether the Texture is compressed or not

	};
};