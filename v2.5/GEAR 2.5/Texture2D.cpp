// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Texture2D.h"
#include "Logger.h"

#include <G2Core/GfxDevice.h>

using namespace G2;

Texture2D::Texture2D(G2Core::FilterMode::Name minFilter,
	G2Core::FilterMode::Name magFilter,
	unsigned int width,
	unsigned int height,
	G2Core::DataFormat::Base::Name format,
	G2Core::DataFormat::Internal::Name internalFormat,
	G2Core::WrapMode::Name wrapS,
	G2Core::WrapMode::Name wrapT,
	bool compress,
	G2Core::DataType::Name dataType,
	void* data)
	: Texture(G2Core::TextureFormat::TEXTURE_2D, internalFormat, minFilter, magFilter),
	mWidth(width),
	mHeight(height),
	mChannels(/*internalFormat == G2Core::DataFormat::Internal::UNKNOWN ? numChannelsFromFormat(format) :*/ numChannelsFromFormat(internalFormat)),
	mCompressed(compress),
	mBytes(0)
{
	mTexResource = G2_gfxDevice()->createTexture2D(
		mWidth, mHeight,
		format, internalFormat/* == G2Core::DataFormat::Internal::UNKNOWN ? format : internalFormat*/,
		mMinFilter, mMagFilter,
		wrapS, wrapT,
		dataType,
		data
		);
}

Texture2D::Texture2D(G2Core::GfxResource* gfxResource, 
	std::string const& sourceFile,
	G2Core::FilterMode::Name minFilter,
	G2Core::FilterMode::Name magFilter,
	unsigned int width,
	unsigned int height,
	G2Core::DataFormat::Base::Name format,
	G2Core::DataFormat::Internal::Name internalFormat,
	G2Core::WrapMode::Name wrapS,
	G2Core::WrapMode::Name wrapT,
	bool compress)
	: Texture(G2Core::TextureFormat::TEXTURE_2D, internalFormat, minFilter, magFilter),
	mSourceFile(sourceFile),
	mWidth(width),
	mHeight(height),
	mChannels(/*internalFormat == G2Core::DataFormat::Internal::UNKNOWN ? numChannelsFromFormat(format) :*/ numChannelsFromFormat(internalFormat)),
	mCompressed(compress),
	mBytes(0)
{
	mTexResource = gfxResource;
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

std::shared_ptr<Texture2D>
Texture2D::Builder::buildResource(
	G2Core::FilterMode::Name minFilter, 
	G2Core::FilterMode::Name magFilter, 
	bool compress,
	G2Core::WrapMode::Name wrapS, 
	G2Core::WrapMode::Name wrapT,
	G2Core::DataFormat::Internal::Name internalFormat)
{
	G2Core::TextureMetaData metaData;
	auto* texResource = G2_gfxDevice()->createTexture2DFromFile(fileName, internalFormat, minFilter, magFilter, wrapS, wrapT, &metaData);

	if (metaData.status != G2Core::TextureMetaData::OK)
	{
		// error during loading
		logger << "[Texture2D::Builder] Import of texture from file '" << fileName << "' returned with error " << metaData.status << ": " << metaData.error << endl;
		return std::shared_ptr<Texture2D>();
	}

	// build texture
	std::shared_ptr<Texture2D> tex(new Texture2D(texResource, fileName, minFilter, magFilter, metaData.width, metaData.height, metaData.baseFormat, internalFormat, wrapS, wrapT, compress));
	return tex;
}