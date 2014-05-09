#include "TextureCube.h"

using namespace G2;

TextureCube::TextureCube(unsigned int minFilter, 
						 unsigned int magFilter, 
						 unsigned int width,
						 unsigned int height,
						 unsigned int format,
						 bool compress,
						 unsigned char * data)
	: Texture(GL_TEXTURE_CUBE_MAP),
	mMinFilter(minFilter),
	mMagFilter(magFilter),
	mUseMipMaps(mipMapsRequested(minFilter,magFilter)),
	mWidth(width),
	mHeight(height),
	mChannels(numChannelsFromFormat(format)),
	mCompressed(compress),
	mBytes(0)
{
	const unsigned CubeMapDefines[6] = {
		G2::CUBE_MAP_POS_X,
		G2::CUBE_MAP_NEG_X,
		G2::CUBE_MAP_POS_Y,
		G2::CUBE_MAP_NEG_Y,
		G2::CUBE_MAP_POS_Z,
		G2::CUBE_MAP_NEG_Z
	};
	init();
	checkFilter(mMinFilter, mMagFilter);
	GLDEBUG( glGenTextures(1, &mId) );
	GLDEBUG( glBindTexture(mType, mId) );
	GLDEBUG( glTexParameteri(mType, GL_TEXTURE_MIN_FILTER, mMinFilter) );
	GLDEBUG( glTexParameteri(mType, GL_TEXTURE_MAG_FILTER, mMagFilter) );
	GLDEBUG( glTexParameteri(mType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
	GLDEBUG( glTexParameteri(mType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
	for(int i = 0; i < 6; ++i)
	{
		GLDEBUG( glTexImage2D(CubeMapDefines[i], 0, baseFormatToCompressedFormat(format, mCompressed), mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, data) );
	}
	GLint compFlag;
	GLDEBUG( glGetTexLevelParameteriv(mType, 0, GL_TEXTURE_COMPRESSED, &compFlag) );
	GLDEBUG( glBindTexture(mType, 0) );
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

bool TextureCube::gInitialized = false;

void
TextureCube::init() 
{
	// lazy initialization
	if(!gInitialized) 
	{
		glEnable(GL_TEXTURE_CUBE_MAP);
		gInitialized = true;
	}
}