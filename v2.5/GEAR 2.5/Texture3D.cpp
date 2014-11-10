// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Texture3D.h"

#include <G2Core/GfxDevice.h>

using namespace G2;

Texture3D::Texture3D(G2Core::FilterMode::Name minFilter, 
					 G2Core::FilterMode::Name magFilter, 
					 unsigned int width,
					 unsigned int height,
					 unsigned int depth,
					 G2Core::DataFormat::Internal::Name format,
					 bool compress,
					 G2Core::DataType::Name dataType,
					 void* data)
	: Texture(G2Core::TextureFormat::TEXTURE_3D, format, minFilter, magFilter),
	mWidth(width),
	mHeight(height),
	mDepth(depth),
	mChannels(numChannelsFromFormat(format)),
	mCompressed(compress),
	mBytes(0)
{
	init();

	mTexResource = G2_gfxDevice()->createTexture3D(
		mWidth, mHeight, mDepth,
		getBaseFormatByInternalFormat(format), format,
		mMinFilter,mMagFilter,
		G2Core::WrapMode::CLAMP_TO_EDGE,G2Core::WrapMode::CLAMP_TO_EDGE,G2Core::WrapMode::CLAMP_TO_EDGE,
		dataType,
		data
	);
}

Texture3D::Texture3D(Texture3D && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

Texture3D& Texture3D::operator=(Texture3D && rhs) 
{
	mWidth = rhs.mWidth;
	mHeight = rhs.mHeight;
	mDepth = rhs.mDepth;
	mChannels = rhs.mChannels;
	mBytes = rhs.mBytes;
	mTextureMatrix = std::move(rhs.mTextureMatrix);
	mCompressed = rhs.mCompressed;
	
	return static_cast<Texture3D&>(Texture::operator=(std::move(rhs)));
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