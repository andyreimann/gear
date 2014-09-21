// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Texture.h"

#include <G2Core/GfxDevice.h>

using namespace G2;


Texture::Texture(G2Core::TextureFormat::Name type, 
				 G2Core::DataFormat::Name dataFormat, 
				 G2Core::FilterMode::Name minFilter, 
				 G2Core::FilterMode::Name magFilter) 
	: mType(type),
	mDataFormat(dataFormat),
	mMinFilter(minFilter),
	mMagFilter(magFilter),
	mUseMipMaps(mipMapsRequested(minFilter,magFilter)),
	mTexResource(nullptr)
{
	checkFilter(mMinFilter, mMagFilter);
}

Texture::Texture(Texture && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

Texture& Texture::operator=(Texture && rhs)
{
	// 1. Stage: delete maybe allocated resources on target type
	// nothing here
	// 2. Stage: transfer data from src to target
	mType = rhs.mType;
	mDataFormat = rhs.mDataFormat;
	mMinFilter = rhs.mMinFilter;
	mMagFilter = rhs.mMagFilter;
	mUseMipMaps = rhs.mUseMipMaps;
	mTexResource = rhs.mTexResource;
	// 3. Stage: modify src to a well defined state
	rhs.mTexResource = nullptr;

	return *this;
}

Texture::~Texture() 
{
	if(mTexResource != nullptr)
	{
		G2_gfxDevice()->freeGfxResource(mTexResource);
	}
}

void
Texture::bind(G2Core::TexSlot::Name textureSlot) const 
{
	G2_gfxDevice()->bindTexture(mTexResource, textureSlot);
}

void
Texture::unbind(G2Core::TexSlot::Name textureSlot) const 
{
	G2_gfxDevice()->unbindTexture(mTexResource, textureSlot);
}

unsigned int 
Texture::numChannelsFromFormat( G2Core::DataFormat::Name dstFormat ) {
	if(	dstFormat == G2Core::DataFormat::RGBA_UB || 
		dstFormat == G2Core::DataFormat::RGBA_US || 
		dstFormat == G2Core::DataFormat::RGBA16_F || 
		dstFormat == G2Core::DataFormat::RGBA_F || 
		dstFormat == G2Core::DataFormat::RGBA_B || 
		dstFormat == G2Core::DataFormat::RGBA_S || 
		dstFormat == G2Core::DataFormat::RGBA_I || 
		dstFormat == G2Core::DataFormat::RGBA_UI || 
		dstFormat == G2Core::DataFormat::RGBA || 
		dstFormat == G2Core::DataFormat::BGRA || 
		dstFormat == GL_COMPRESSED_RGBA || 
		dstFormat == GL_COMPRESSED_SRGB_ALPHA || 
		dstFormat == GL_COMPRESSED_RGBA_BPTC_UNORM || 
		dstFormat == GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM ) 
			return 4;
	else if( dstFormat == G2Core::DataFormat::RGB || 
		dstFormat == G2Core::DataFormat::BGR || 
		dstFormat == G2Core::DataFormat::RGB_UB || 
		dstFormat == GL_COMPRESSED_RGB || 
		dstFormat == GL_COMPRESSED_SRGB || 
		dstFormat == GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT || 
		dstFormat == GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT) 
		return 3;
	else if( dstFormat == G2Core::DataFormat::ALPHA_UB || 
		dstFormat == G2Core::DataFormat::ALPHA_US || 
		dstFormat == G2Core::DataFormat::ALPHA16_F || 
		dstFormat == G2Core::DataFormat::ALPHA_F || 
		dstFormat == G2Core::DataFormat::ALPHA_B || 
		dstFormat == G2Core::DataFormat::ALPHA_S || 
		dstFormat == G2Core::DataFormat::ALPHA_I || 
		dstFormat == G2Core::DataFormat::ALPHA_UI || 
		dstFormat == G2Core::DataFormat::LUMINANCE || 
		dstFormat == G2Core::DataFormat::LUMINANCE_UB || 
		dstFormat == G2Core::DataFormat::LUMINANCE_US || 
		dstFormat == G2Core::DataFormat::LUMINANCE16_F || 
		dstFormat == G2Core::DataFormat::LUMINANCE_F || 
		dstFormat == G2Core::DataFormat::LUMINANCE_B || 
		dstFormat == G2Core::DataFormat::LUMINANCE_S || 
		dstFormat == G2Core::DataFormat::LUMINANCE_I || 
		dstFormat == G2Core::DataFormat::LUMINANCE_UI ||
		dstFormat == G2Core::DataFormat::DEPTH || 
		dstFormat == G2Core::DataFormat::DEPTH16 || 
		dstFormat == G2Core::DataFormat::DEPTH24 || 
		dstFormat == G2Core::DataFormat::DEPTH32 || 
		dstFormat == G2Core::DataFormat::INTENSITY || 
		dstFormat == G2Core::DataFormat::INTENSITY_UB || 
		dstFormat == G2Core::DataFormat::INTENSITY_US || 
		dstFormat == G2Core::DataFormat::INTENSITY16_F || 
		dstFormat == G2Core::DataFormat::INTENSITY_F || 
		dstFormat == G2Core::DataFormat::INTENSITY_B || 
		dstFormat == G2Core::DataFormat::INTENSITY_S || 
		dstFormat == G2Core::DataFormat::INTENSITY_I || 
		dstFormat == G2Core::DataFormat::INTENSITY_UI || 
		dstFormat == G2Core::DataFormat::RED || 
		dstFormat == GL_COMPRESSED_RED || 
		dstFormat == GL_COMPRESSED_RED_RGTC1 || 
		dstFormat == GL_COMPRESSED_SIGNED_RED_RGTC1 ) {
			return 1;
	}
	else if( dstFormat == G2Core::DataFormat::LUMINANCE_ALPHA_UB || 
		dstFormat == G2Core::DataFormat::LUMINANCE_ALPHA_US || 
		dstFormat == G2Core::DataFormat::LUMINANCE_ALPHA16_F || 
		dstFormat == G2Core::DataFormat::LUMINANCE_ALPHA_F || 
		dstFormat == G2Core::DataFormat::LUMINANCE_ALPHA_B || 
		dstFormat == G2Core::DataFormat::LUMINANCE_ALPHA_S || 
		dstFormat == G2Core::DataFormat::LUMINANCE_ALPHA_I || 
		dstFormat == G2Core::DataFormat::RG || 
		dstFormat == GL_COMPRESSED_RG || 
		dstFormat == GL_COMPRESSED_RG_RGTC2 || 
		dstFormat == GL_COMPRESSED_SIGNED_RG_RGTC2 ) {
			return 2;
	}
	assert(0);
	return 0;
}

unsigned int 
Texture::convertFormat( G2Core::DataFormat::Name baseFormat, bool compress ) {
	
	if( !compress ) // no compression requested or compression not supported
		return baseFormat;

	switch( baseFormat ) {
		case G2Core::DataFormat::ALPHA_F:
			return GL_COMPRESSED_ALPHA;
			break;
		case G2Core::DataFormat::LUMINANCE:
			return GL_COMPRESSED_LUMINANCE;
			break;
		case G2Core::DataFormat::LUMINANCE_ALPHA_F:
			return GL_COMPRESSED_LUMINANCE_ALPHA;
			break;
		case G2Core::DataFormat::INTENSITY:
			return GL_COMPRESSED_INTENSITY;
			break;
		case G2Core::DataFormat::RGB:
			return GL_COMPRESSED_RGB;
			break;
		case G2Core::DataFormat::RED:
			return GL_COMPRESSED_RED;
			break;
		case G2Core::DataFormat::RG:
			return GL_COMPRESSED_RG;
			break;
		case G2Core::DataFormat::RGBA:
			return GL_COMPRESSED_RGBA;
			break;
		default:
			return baseFormat;
	}
}

bool Texture::mipMapsRequested(G2Core::FilterMode::Name minFilter, G2Core::FilterMode::Name magFilter) 
{

	// check the minFilter if it has mipmapping
	if( minFilter == G2Core::FilterMode::NEAREST_MIPMAP_NEAREST ||
		minFilter == G2Core::FilterMode::LINEAR_MIPMAP_NEAREST  || 
		minFilter == G2Core::FilterMode::NEAREST_MIPMAP_LINEAR  || 
		minFilter == G2Core::FilterMode::LINEAR_MIPMAP_LINEAR ) 
	{
		return true;
	}
	return false;
}

void Texture::checkFilter(G2Core::FilterMode::Name minFilter, G2Core::FilterMode::Name magFilter) 
{
	assert( minFilter == G2Core::FilterMode::NEAREST || 
		    minFilter == G2Core::FilterMode::LINEAR ||
			minFilter == G2Core::FilterMode::NEAREST_MIPMAP_NEAREST ||
			minFilter == G2Core::FilterMode::LINEAR_MIPMAP_NEAREST  || 
			minFilter == G2Core::FilterMode::NEAREST_MIPMAP_LINEAR  || 
			minFilter == G2Core::FilterMode::LINEAR_MIPMAP_LINEAR );

	assert( magFilter == G2Core::FilterMode::NEAREST || magFilter == G2Core::FilterMode::LINEAR );
}



unsigned 
Texture::baseFormatToCompressedFormat( G2Core::DataFormat::Name baseFormat, bool compress ) 
{
	return baseFormat;
	if( !compress /*|| !mTexCompressionSupported*/ ) // no compression requested or compression not supported
		return baseFormat;
	switch( baseFormat ) 
	{
		case G2Core::DataFormat::ALPHA_F:
			return GL_COMPRESSED_ALPHA;
			break;
		case G2Core::DataFormat::LUMINANCE:
			return GL_COMPRESSED_LUMINANCE;
			break;
		case G2Core::DataFormat::LUMINANCE_ALPHA_F:
			return GL_COMPRESSED_LUMINANCE_ALPHA;
			break;
		case G2Core::DataFormat::INTENSITY:
			return GL_COMPRESSED_INTENSITY;
			break;
		case G2Core::DataFormat::RGB:
			return GL_COMPRESSED_RGB;
			break;
		case G2Core::DataFormat::RGBA:
			return GL_COMPRESSED_RGBA;
			break;
		default:
			return baseFormat;
			break;
	}
}

G2Core::DataFormat::Name
Texture::getFormatByString(std::string const& format) 
{
	if(format == "RGB") { return G2Core::DataFormat::RGB; }
	if(format == "BGR") { return G2Core::DataFormat::BGR; }
	if(format == "RGB_UB") { return G2Core::DataFormat::RGB_UB; }
	if(format == "RGB_US") { return G2Core::DataFormat::RGB_US; }
	if(format == "RGBA") { return G2Core::DataFormat::RGBA; }
	if(format == "BGRA") { return G2Core::DataFormat::BGRA; }
	if(format == "RGBA_UB") { return G2Core::DataFormat::RGBA_UB; }
	if(format == "RGBA_US") { return G2Core::DataFormat::RGBA_US; }
	if(format == "RGBA16_F") { return G2Core::DataFormat::RGBA16_F; }
	if(format == "RGBA_F") { return G2Core::DataFormat::RGBA_F; }
	if(format == "RGBA_B") { return G2Core::DataFormat::RGBA_B; }
	if(format == "RGBA_S") { return G2Core::DataFormat::RGBA_S; }
	if(format == "RGBA_I") { return G2Core::DataFormat::RGBA_I; }
	if(format == "RGBA_UI") { return G2Core::DataFormat::RGBA_UI; }
	if(format == "ALPHA_UB") { return G2Core::DataFormat::ALPHA_UB; }
	if(format == "ALPHA_US") { return G2Core::DataFormat::ALPHA_US; }
	if(format == "ALPHA16_F") { return G2Core::DataFormat::ALPHA16_F; }
	if(format == "ALPHA_B") { return G2Core::DataFormat::ALPHA_B; }
	if(format == "ALPHA_S") { return G2Core::DataFormat::ALPHA_S; }
	if(format == "ALPHA_I") { return G2Core::DataFormat::ALPHA_I; }
	if(format == "ALPHA_UI") { return G2Core::DataFormat::ALPHA_UI; }
	if(format == "LUMINANCE") { return G2Core::DataFormat::LUMINANCE; }
	if(format == "DEPTH") { return G2Core::DataFormat::DEPTH; }
	if(format == "DEPTH16") { return G2Core::DataFormat::DEPTH16; }
	if(format == "DEPTH24") { return G2Core::DataFormat::DEPTH24; }
	if(format == "DEPTH32") { return G2Core::DataFormat::DEPTH32; }
	if(format == "LUMINANCE_ALPHA_UB") { return G2Core::DataFormat::LUMINANCE_ALPHA_UB; }
	if(format == "LUMINANCE_ALPHA_US") { return G2Core::DataFormat::LUMINANCE_ALPHA_US; }
	if(format == "LUMINANCE_ALPHA16_F") { return G2Core::DataFormat::LUMINANCE_ALPHA16_F; }
	if(format == "LUMINANCE_ALPHA_F") { return G2Core::DataFormat::LUMINANCE_ALPHA_F; }
	if(format == "LUMINANCE_ALPHA_B") { return G2Core::DataFormat::LUMINANCE_ALPHA_B; }
	if(format == "LUMINANCE_ALPHA_S") { return G2Core::DataFormat::LUMINANCE_ALPHA_S; }
	if(format == "LUMINANCE_ALPHA_I") { return G2Core::DataFormat::LUMINANCE_ALPHA_I; }
	if(format == "LUMINANCE_ALPHA_UI") { return G2Core::DataFormat::LUMINANCE_ALPHA_UI; }
	if(format == "LUMINANCE_UB") { return G2Core::DataFormat::LUMINANCE_UB; }
	if(format == "LUMINANCE_US") { return G2Core::DataFormat::LUMINANCE_US; }
	if(format == "LUMINANCE16_F") { return G2Core::DataFormat::LUMINANCE16_F; }
	if(format == "LUMINANCE_F") { return G2Core::DataFormat::LUMINANCE_F; }
	if(format == "LUMINANCE_B") { return G2Core::DataFormat::LUMINANCE_B; }
	if(format == "LUMINANCE_S") { return G2Core::DataFormat::LUMINANCE_S; }
	if(format == "LUMINANCE_I") { return G2Core::DataFormat::LUMINANCE_I; }
	if(format == "LUMINANCE_UI") { return G2Core::DataFormat::LUMINANCE_UI; }
	if(format == "INTENSITY") { return G2Core::DataFormat::INTENSITY; }
	if(format == "INTENSITY_UB") { return G2Core::DataFormat::INTENSITY_UB; }
	if(format == "INTENSITY_US") { return G2Core::DataFormat::INTENSITY_US; }
	if(format == "INTENSITY16_F") { return G2Core::DataFormat::INTENSITY16_F; }
	if(format == "INTENSITY_F") { return G2Core::DataFormat::INTENSITY_F; }
	if(format == "INTENSITY_B") { return G2Core::DataFormat::INTENSITY_B; }
	if(format == "INTENSITY_S") { return G2Core::DataFormat::INTENSITY_S; }
	if(format == "INTENSITY_I") { return G2Core::DataFormat::INTENSITY_I; }
	if(format == "INTENSITY_UI") { return G2Core::DataFormat::INTENSITY_UI; }
	return G2Core::DataFormat::UNKNOWN;
}

bool
Texture::isCompressedFormat(G2Core::DataFormat::Name baseFormat) 
{
	return baseFormat == GL_COMPRESSED_RED ||
		baseFormat == GL_COMPRESSED_RG ||
		baseFormat == GL_COMPRESSED_RGB ||
		baseFormat == GL_COMPRESSED_RGBA ||
		baseFormat == GL_COMPRESSED_SRGB ||
		baseFormat == GL_COMPRESSED_SRGB_ALPHA ||
		baseFormat == GL_COMPRESSED_RED_RGTC1 ||
		baseFormat == GL_COMPRESSED_SIGNED_RED_RGTC1 ||
		baseFormat == GL_COMPRESSED_RG_RGTC2 ||
		baseFormat == GL_COMPRESSED_SIGNED_RG_RGTC2 ||
		baseFormat == GL_COMPRESSED_RGBA_BPTC_UNORM ||
		baseFormat == GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM ||
		baseFormat == GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT ||
		baseFormat == GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
}