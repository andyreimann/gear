// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Texture2D.h"

#include <G2Core/GfxDevice.h>

#include <IL/il.h>

using namespace G2;

Texture2D::Texture2D(G2Core::FilterMode::Name minFilter, 
					 G2Core::FilterMode::Name magFilter, 
					 unsigned int width,
					 unsigned int height,
					 G2Core::DataFormat::Name format,
					 G2Core::DataFormat::Name internalFormat,
					 G2Core::WrapMode::Name wrapS,
					 G2Core::WrapMode::Name wrapT,
					 bool compress,
					 unsigned char * data)
	: Texture(G2Core::TextureFormat::TEXTURE_2D, internalFormat, minFilter, magFilter),
	mWidth(width),
	mHeight(height),
	mChannels(internalFormat == G2Core::DataFormat::UNKNOWN ? numChannelsFromFormat(format) : numChannelsFromFormat(internalFormat)),
	mCompressed(compress),
	mBytes(0)
{
	init();

	mTexResource = G2_gfxDevice()->createTexture2D(
		mWidth, mHeight,
		format,internalFormat == G2Core::DataFormat::UNKNOWN ? format : internalFormat,
		mMinFilter,mMagFilter,
		wrapS,wrapT,
		data
	);
}

Texture2D::Texture2D(Texture2D && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

Texture2D& Texture2D::operator=(Texture2D && rhs) 
{
	mWidth = rhs.mWidth;
	mHeight = rhs.mHeight;
	mChannels = rhs.mChannels;
	mBytes = rhs.mBytes;
	mTextureMatrix = std::move(rhs.mTextureMatrix);
	mCompressed = rhs.mCompressed;
	
	return static_cast<Texture2D&>(Texture::operator=(std::move(rhs)));
}

bool Texture2D::gInitialized = false;

void
Texture2D::init() 
{
	// lazy initialization
	if(!gInitialized) 
	{
		ilInit();
		gInitialized = true;
	}
}

std::shared_ptr<Texture2D>
Texture2D::Builder::buildResource(
	G2Core::FilterMode::Name minFilter, 
	G2Core::FilterMode::Name magFilter, 
	bool compress,
	G2Core::WrapMode::Name wrapS, 
	G2Core::WrapMode::Name wrapT,
	G2Core::DataFormat::Name internalFormat) 
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

	G2Core::DataFormat::Name parsedFormat = G2Core::DataFormat::UNKNOWN;
	switch(format)
	{
		case IL_RGB:			parsedFormat = G2Core::DataFormat::RGB; break;
		case IL_RGBA:			parsedFormat = G2Core::DataFormat::RGBA; break;
		case IL_LUMINANCE:			parsedFormat = G2Core::DataFormat::LUMINANCE; break;
		case IL_BGR:			parsedFormat = G2Core::DataFormat::BGR; break;
		case IL_BGRA:			parsedFormat = G2Core::DataFormat::BGRA; break;
	}
	assert(parsedFormat != G2Core::DataFormat::UNKNOWN);

	// build texture
	std::shared_ptr<Texture2D> tex(new Texture2D(minFilter, magFilter, width, height, parsedFormat, internalFormat, wrapS, wrapT, compress, data));
	ilBindImage(0);
	return tex;
}


Texture2D::Builder::~Builder() 
{
	ilDeleteImages(1,&id);
}