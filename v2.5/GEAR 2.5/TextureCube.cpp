#include "TextureCube.h"

#include <G2Core/GfxDevice.h>

using namespace G2;

TextureCube::TextureCube(G2Core::FilterMode::Name minFilter, 
						 G2Core::FilterMode::Name magFilter, 
						 unsigned int width,
						 unsigned int height,
						 G2Core::DataFormat::Internal::Name format,
						 bool compress,
						 void* data)
	: Texture(G2Core::TextureFormat::TEXTURE_CUBE_MAP, format,minFilter,magFilter),
	mWidth(width),
	mHeight(height),
	mChannels(numChannelsFromFormat(format)),
	mCompressed(compress),
	mBytes(0)
{
	mTexResource = G2_gfxDevice()->createTextureCube(
		mWidth, mHeight,
		getBaseFormatByInternalFormat(format),format,
		mMinFilter,mMagFilter,
		G2Core::WrapMode::CLAMP_TO_EDGE,G2Core::WrapMode::CLAMP_TO_EDGE
	);
}


G2::TextureCube::TextureCube(TextureCube && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

TextureCube& TextureCube::operator=(TextureCube && rhs) 
{
	mMinFilter = rhs.mMinFilter;
	mMagFilter = rhs.mMagFilter;
	mWidth = rhs.mWidth;
	mHeight = rhs.mHeight;
	mChannels = rhs.mChannels;
	mBytes = rhs.mBytes;
	mTextureMatrix = std::move(rhs.mTextureMatrix);
	mCompressed = rhs.mCompressed;
	mUseMipMaps = rhs.mUseMipMaps;
	
	return static_cast<TextureCube&>(Texture::operator=(std::move(rhs)));
}