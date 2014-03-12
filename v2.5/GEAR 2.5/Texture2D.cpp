// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Texture2D.h"

#include <IL/il.h>

using namespace G2;

Texture2D::Texture2D(unsigned int minFilter, 
					 unsigned int magFilter, 
					 unsigned int width,
					 unsigned int height,
					 unsigned int format,
					 bool compress,
					 unsigned char * data)
	: Texture(GL_TEXTURE_2D),
	mMinFilter(minFilter),
	mMagFilter(magFilter),
	mUseMipMaps(mipMapsRequested(minFilter,magFilter)),
	mWidth(width),
	mHeight(height),
	mChannels(numChannelsFromFormat(format)),
	mCompressed(compress),
	mBytes(0)
{
	init();
	checkFilter(mMinFilter, mMagFilter);
	
	GLDEBUG( glGenTextures(1, &mId) );
	GLDEBUG( glBindTexture(GL_TEXTURE_2D, mId) );
	GLDEBUG( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mMinFilter) );
	GLDEBUG( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mMagFilter) );
	GLDEBUG( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
	GLDEBUG( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
	GLDEBUG( glTexImage2D(GL_TEXTURE_2D, 0, baseFormatToCompressedFormat(format, mCompressed), mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data) );
	GLint compFlag;
	GLDEBUG( glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &compFlag) );
	GLDEBUG( glBindTexture(GL_TEXTURE_2D, 0) );
}

bool Texture2D::gInitialized = false;

void
Texture2D::init() 
{
	// lazy initialization
	if(!gInitialized) 
	{
		ilInit();
		glEnable(GL_TEXTURE_2D);
		gInitialized = true;
	}
}

std::shared_ptr<Texture2D>
Texture2D::Builder::buildResource(unsigned minFilter, unsigned magFilter, bool compress) 
{
	ilBindImage(id);		// bind ID as current Texture

	unsigned char * data = ilGetData();
	if( data == NULL ) 
	{
		return std::shared_ptr<Texture2D>();
	}
	unsigned width		= ilGetInteger( IL_IMAGE_WIDTH );
	unsigned height		= ilGetInteger( IL_IMAGE_HEIGHT );	
	unsigned format		= ilGetInteger( IL_IMAGE_FORMAT );
	// build texture
	std::shared_ptr<Texture2D> tex(new Texture2D(minFilter, magFilter, width, height, format, compress, data));
	ilBindImage(0);
	return tex;
}


Texture2D::Builder::~Builder() 
{
	ilDeleteImages(1,&id);
}