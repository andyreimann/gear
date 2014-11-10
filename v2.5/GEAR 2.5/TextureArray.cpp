// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "TextureArray.h"
#include "Texture2D.h"

#include <G2Core/GfxDevice.h>

using namespace G2;

TextureArray::TextureArray(
					 G2Core::FilterMode::Name minFilter, 
					 G2Core::FilterMode::Name magFilter, 
					 unsigned int width,
					 unsigned int height,
					 unsigned int size,
					 G2Core::DataFormat::Internal::Name format,
					 bool compress,
					 G2Core::DataType::Name dataType,
					 void* data)
	: Texture(G2Core::TextureFormat::TEXTURE_2D_ARRAY,format,minFilter,magFilter),
	mWidth(width),
	mHeight(height),
	mDepth(size),
	mChannels(numChannelsFromFormat(format)),
	mCompressed(compress),
	mBytes(0)
{
	assert(mDepth > 0);

	mTexResource = G2_gfxDevice()->createTexture2DArray(
		mWidth, mHeight, size,
		getBaseFormatByInternalFormat(format),format,
		mMinFilter,mMagFilter,
		G2Core::WrapMode::CLAMP_TO_EDGE,G2Core::WrapMode::CLAMP_TO_EDGE,
		dataType,
		data
	);
}

TextureArray::TextureArray(TextureArray && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

TextureArray& TextureArray::operator=(TextureArray && rhs) 
{
	mWidth = rhs.mWidth;
	mHeight = rhs.mHeight;
	mDepth = rhs.mDepth;
	mChannels = rhs.mChannels;
	mBytes = rhs.mBytes;
	mCompressed = rhs.mCompressed;
	
	return static_cast<TextureArray&>(Texture::operator=(std::move(rhs)));
}