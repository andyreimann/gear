#include "TextureArray.h"
#include "Texture2D.h"
// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5

using namespace G2;

TextureArray::TextureArray(
					 unsigned int minFilter, 
					 unsigned int magFilter, 
					 unsigned int width,
					 unsigned int height,
					 unsigned int size,
					 unsigned int format,
					 bool compress,
					 unsigned char * data)
	: Texture(GL_TEXTURE_2D_ARRAY),
	mMinFilter(minFilter),
	mMagFilter(magFilter),
	mUseMipMaps(mipMapsRequested(minFilter,magFilter)),
	mWidth(width),
	mHeight(height),
	mDepth(size),
	mChannels(numChannelsFromFormat(format)),
	mCompressed(compress),
	mBytes(0)
{
	assert(mDepth > 0);
	init();
	checkFilter(mMinFilter, mMagFilter);
	GLDEBUG( glGenTextures(1, &mId) );
	GLDEBUG( glBindTexture(mType, mId) );
	GLDEBUG( glTexParameteri(mType, GL_TEXTURE_MIN_FILTER, mMinFilter) );
	GLDEBUG( glTexParameteri(mType, GL_TEXTURE_MAG_FILTER, mMagFilter) );
	GLDEBUG( glTexParameteri(mType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
	GLDEBUG( glTexParameteri(mType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
	GLDEBUG( glTexImage3D(mType, 0, baseFormatToCompressedFormat(format, mCompressed), mWidth, mHeight, mDepth, 0, format, GL_UNSIGNED_BYTE, data) );
	GLint compFlag;
	GLDEBUG( glGetTexLevelParameteriv(mType, 0, GL_TEXTURE_COMPRESSED, &compFlag) );
	GLDEBUG( glBindTexture(mType, 0) );
}

TextureArray::TextureArray(TextureArray && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

TextureArray& TextureArray::operator=(TextureArray && rhs) 
{
	mMinFilter = rhs.mMinFilter;
	mMagFilter = rhs.mMagFilter;
	mWidth = rhs.mWidth;
	mHeight = rhs.mHeight;
	mDepth = rhs.mDepth;
	mChannels = rhs.mChannels;
	mBytes = rhs.mBytes;
	mTextureMatrix = std::move(rhs.mTextureMatrix);
	mCompressed = rhs.mCompressed;
	mUseMipMaps = rhs.mUseMipMaps;
	
	return static_cast<TextureArray&>(Texture::operator=(std::move(rhs)));
}

bool TextureArray::gInitialized = false;

void
TextureArray::init() 
{
	// lazy initialization
	if(!gInitialized) 
	{
		glEnable(GL_TEXTURE_2D);
		gInitialized = true;
	}
}