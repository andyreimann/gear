// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Texture3D.h"

using namespace G2;

Texture3D::Texture3D(unsigned int minFilter, 
					 unsigned int magFilter, 
					 unsigned int width,
					 unsigned int height,
					 unsigned int depth,
					 unsigned int format,
					 bool compress,
					 unsigned char * data)
	: Texture(GL_TEXTURE_3D),
	mMinFilter(minFilter),
	mMagFilter(magFilter),
	mUseMipMaps(mipMapsRequested(minFilter,magFilter)),
	mWidth(width),
	mHeight(height),
	mDepth(depth),
	mChannels(numChannelsFromFormat(format)),
	mCompressed(compress),
	mBytes(0)
{
	init();
	checkFilter(mMinFilter, mMagFilter);
	
	GLDEBUG( glGenTextures(1, &mId) );
	GLDEBUG( glBindTexture(GL_TEXTURE_3D, mId) );
	GLDEBUG( glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, mMinFilter) );
	GLDEBUG( glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, mMagFilter) );
	GLDEBUG( glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
	GLDEBUG( glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
	GLDEBUG( glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE) );
	GLDEBUG( glTexImage3D(GL_TEXTURE_3D, 0, format, mWidth, mHeight, mDepth, 0, GL_RGB, GL_UNSIGNED_BYTE, data) );
	GLDEBUG( glBindTexture(GL_TEXTURE_3D, 0) );
}

Texture3D::~Texture3D() 
{
}

bool Texture3D::gInitialized = false;

void
Texture3D::init() 
{
	// lazy initialization
	if(!gInitialized) 
	{
		glEnable(GL_TEXTURE_3D);
		gInitialized = true;
	}
}