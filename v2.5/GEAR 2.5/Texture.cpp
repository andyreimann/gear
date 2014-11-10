// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Texture.h"

#include <G2Core/GfxDevice.h>

using namespace G2;


Texture::Texture(G2Core::TextureFormat::Name type, 
				 G2Core::DataFormat::Internal::Name dataFormat, 
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
Texture::numChannelsFromFormat( G2Core::DataFormat::Internal::Name dstFormat ) {
	if (dstFormat == G2Core::DataFormat::Internal::R32G32B32A32_UI ||
		dstFormat == G2Core::DataFormat::Internal::R32G32B32A32_I ||
		dstFormat == G2Core::DataFormat::Internal::R32G32B32A32_F
		/*dstFormat == GL_COMPRESSED_RGBA || 
		dstFormat == GL_COMPRESSED_SRGB_ALPHA || 
		dstFormat == GL_COMPRESSED_RGBA_BPTC_UNORM || 
		dstFormat == GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM*/ ) 
			return 4;
	else if (dstFormat == G2Core::DataFormat::Internal::R32G32B32_UI ||
		dstFormat == G2Core::DataFormat::Internal::R32G32B32_I ||
		dstFormat == G2Core::DataFormat::Internal::R32G32B32_F
		/*dstFormat == GL_COMPRESSED_RGB || 
		dstFormat == GL_COMPRESSED_SRGB || 
		dstFormat == GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT || 
		dstFormat == GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT*/) 
		return 3;
	else if (dstFormat == G2Core::DataFormat::Internal::R8_UI ||
		dstFormat == G2Core::DataFormat::Internal::R8_I ||
		dstFormat == G2Core::DataFormat::Internal::R16_UI ||
		dstFormat == G2Core::DataFormat::Internal::R16_I ||
		dstFormat == G2Core::DataFormat::Internal::R16_F ||
		dstFormat == G2Core::DataFormat::Internal::R32_UI ||
		dstFormat == G2Core::DataFormat::Internal::R32_I ||
		dstFormat == G2Core::DataFormat::Internal::R32_F ||
		dstFormat == G2Core::DataFormat::Internal::D16_F ||
		dstFormat == G2Core::DataFormat::Internal::D24_F ||
		dstFormat == G2Core::DataFormat::Internal::D32_F
		/*dstFormat == GL_COMPRESSED_RED || 
		dstFormat == GL_COMPRESSED_RED_RGTC1 || 
		dstFormat == GL_COMPRESSED_SIGNED_RED_RGTC1*/ ) {
			return 1;
	}
	else if (dstFormat == G2Core::DataFormat::Internal::R8G8_UI ||
		dstFormat == G2Core::DataFormat::Internal::R8G8_I ||
		dstFormat == G2Core::DataFormat::Internal::R16G16_UI ||
		dstFormat == G2Core::DataFormat::Internal::R16G16_I ||
		dstFormat == G2Core::DataFormat::Internal::R16G16_F ||
		dstFormat == G2Core::DataFormat::Internal::R32G32_UI ||
		dstFormat == G2Core::DataFormat::Internal::R32G32_I ||
		dstFormat == G2Core::DataFormat::Internal::R32G32_F
		/*dstFormat == GL_COMPRESSED_RG || 
		dstFormat == GL_COMPRESSED_RG_RGTC2 || 
		dstFormat == GL_COMPRESSED_SIGNED_RG_RGTC2*/ ) {
			return 2;
	}
	assert(0);
	return 0;
}

unsigned int 
Texture::convertFormat( G2Core::DataFormat::Internal::Name baseFormat, bool compress ) 
{

	return baseFormat; // disabled

	//if( !compress ) // no compression requested or compression not supported
	//	return baseFormat;

	//switch( baseFormat ) {
	//	case G2Core::DataFormat::ALPHA_F:
	//		return GL_COMPRESSED_ALPHA;
	//		break;
	//	case G2Core::DataFormat::LUMINANCE:
	//		return GL_COMPRESSED_LUMINANCE;
	//		break;
	//	case G2Core::DataFormat::LUMINANCE_ALPHA_F:
	//		return GL_COMPRESSED_LUMINANCE_ALPHA;
	//		break;
	//	case G2Core::DataFormat::INTENSITY:
	//		return GL_COMPRESSED_INTENSITY;
	//		break;
	//	case G2Core::DataFormat::RGB:
	//		return GL_COMPRESSED_RGB;
	//		break;
	//	case G2Core::DataFormat::RED:
	//		return GL_COMPRESSED_RED;
	//		break;
	//	case G2Core::DataFormat::RG:
	//		return GL_COMPRESSED_RG;
	//		break;
	//	case G2Core::DataFormat::RGBA:
	//		return GL_COMPRESSED_RGBA;
	//		break;
	//	default:
	//		return baseFormat;
	//}
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
Texture::baseFormatToCompressedFormat(G2Core::DataFormat::Internal::Name baseFormat, bool compress)
{
	return baseFormat;

	//if( !compress /*|| !mTexCompressionSupported*/ ) // no compression requested or compression not supported
	//	return baseFormat;
	//switch( baseFormat ) 
	//{
	//	case G2Core::DataFormat::ALPHA_F:
	//		return GL_COMPRESSED_ALPHA;
	//		break;
	//	case G2Core::DataFormat::LUMINANCE:
	//		return GL_COMPRESSED_LUMINANCE;
	//		break;
	//	case G2Core::DataFormat::LUMINANCE_ALPHA_F:
	//		return GL_COMPRESSED_LUMINANCE_ALPHA;
	//		break;
	//	case G2Core::DataFormat::INTENSITY:
	//		return GL_COMPRESSED_INTENSITY;
	//		break;
	//	case G2Core::DataFormat::RGB:
	//		return GL_COMPRESSED_RGB;
	//		break;
	//	case G2Core::DataFormat::RGBA:
	//		return GL_COMPRESSED_RGBA;
	//		break;
	//	default:
	//		return baseFormat;
	//		break;
	//}
}

G2Core::DataFormat::Base::Name
Texture::getBaseFormatByString(std::string const& format)
{
	if (format == "RED") { return G2Core::DataFormat::Base::RED; }
	if (format == "RG") { return G2Core::DataFormat::Base::RG; }
	if (format == "RGB") { return G2Core::DataFormat::Base::RGB; }
	if (format == "RGBA") { return G2Core::DataFormat::Base::RGBA; }
	if (format == "DEPTH") { return G2Core::DataFormat::Base::DEPTH; }

	if (format == "R8_UI") { return G2Core::DataFormat::Base::RED; }
	if (format == "R8_I") { return G2Core::DataFormat::Base::RED; }
	if (format == "R16_UI") { return G2Core::DataFormat::Base::RED; }
	if (format == "R16_I") { return G2Core::DataFormat::Base::RED; }
	if (format == "R16_F") { return G2Core::DataFormat::Base::RED; }
	if (format == "R32_UI") { return G2Core::DataFormat::Base::RED; }
	if (format == "R32_I") { return G2Core::DataFormat::Base::RED; }
	if (format == "R32_F") { return G2Core::DataFormat::Base::RED; }
	if (format == "R8G8_UI") { return G2Core::DataFormat::Base::RG; }
	if (format == "R8G8_I") { return G2Core::DataFormat::Base::RG; }
	if (format == "R16G16_UI") { return G2Core::DataFormat::Base::RG; }
	if (format == "R16G16_I") { return G2Core::DataFormat::Base::RG; }
	if (format == "R16G16_F") { return G2Core::DataFormat::Base::RG; }
	if (format == "R32G32_UI") { return G2Core::DataFormat::Base::RG; }
	if (format == "R32G32_I") { return G2Core::DataFormat::Base::RG; }
	if (format == "R32G32_F") { return G2Core::DataFormat::Base::RG; }
	if (format == "R32G32B32_UI") { return G2Core::DataFormat::Base::RGB; }
	if (format == "R32G32B32_I") { return G2Core::DataFormat::Base::RGB; }
	if (format == "R32G32B32_F") { return G2Core::DataFormat::Base::RGB; }
	if (format == "R32G32B32A32_UI") { return G2Core::DataFormat::Base::RGBA; }
	if (format == "R32G32B32A32_I") { return G2Core::DataFormat::Base::RGBA; }
	if (format == "R32G32B32A32_F") { return G2Core::DataFormat::Base::RGBA; }
	if (format == "D16_F") { return G2Core::DataFormat::Base::DEPTH; }
	if (format == "D24_F") { return G2Core::DataFormat::Base::DEPTH; }
	if (format == "D32_F") { return G2Core::DataFormat::Base::DEPTH; }
	return G2Core::DataFormat::Base::UNKNOWN;
}

G2Core::DataFormat::Internal::Name
Texture::getInternalFormatByString(std::string const& format)
{
	if (format == "RED") { return G2Core::DataFormat::Internal::R32_F; }
	if (format == "RG") { return G2Core::DataFormat::Internal::R32G32_F; }
	if (format == "RGB") { return G2Core::DataFormat::Internal::R32G32B32_F; }
	if (format == "RGBA") { return G2Core::DataFormat::Internal::R32G32B32A32_F; }
	if (format == "DEPTH") { return G2Core::DataFormat::Internal::D24_F; }

	if (format == "R8_UI") { return G2Core::DataFormat::Internal::R8_UI; }
	if (format == "R8_I") { return G2Core::DataFormat::Internal::R8_I; }
	if (format == "R16_UI") { return G2Core::DataFormat::Internal::R16_UI; }
	if (format == "R16_I") { return G2Core::DataFormat::Internal::R16_I; }
	if (format == "R16_F") { return G2Core::DataFormat::Internal::R16_F; }
	if (format == "R32_UI") { return G2Core::DataFormat::Internal::R32_UI; }
	if (format == "R32_I") { return G2Core::DataFormat::Internal::R32_I; }
	if (format == "R32_F") { return G2Core::DataFormat::Internal::R32_F; }
	if (format == "R8G8_UI") { return G2Core::DataFormat::Internal::R8G8_UI; }
	if (format == "R8G8_I") { return G2Core::DataFormat::Internal::R8G8_I; }
	if (format == "R16G16_UI") { return G2Core::DataFormat::Internal::R16G16_UI; }
	if (format == "R16G16_I") { return G2Core::DataFormat::Internal::R16G16_I; }
	if (format == "R16G16_F") { return G2Core::DataFormat::Internal::R16G16_F; }
	if (format == "R32G32_UI") { return G2Core::DataFormat::Internal::R32G32_UI; }
	if (format == "R32G32_I") { return G2Core::DataFormat::Internal::R32G32_I; }
	if (format == "R32G32_F") { return G2Core::DataFormat::Internal::R32G32_F; }
	if (format == "R32G32B32_UI") { return G2Core::DataFormat::Internal::R32G32B32_UI; }
	if (format == "R32G32B32_I") { return G2Core::DataFormat::Internal::R32G32B32_I; }
	if (format == "R32G32B32_F") { return G2Core::DataFormat::Internal::R32G32B32_F; }
	if (format == "R32G32B32A32_UI") { return G2Core::DataFormat::Internal::R32G32B32A32_UI; }
	if (format == "R32G32B32A32_I") { return G2Core::DataFormat::Internal::R32G32B32A32_I; }
	if (format == "R32G32B32A32_F") { return G2Core::DataFormat::Internal::R32G32B32A32_F; }
	if (format == "D16_F") { return G2Core::DataFormat::Internal::D16_F; }
	if (format == "D24_F") { return G2Core::DataFormat::Internal::D24_F; }
	if (format == "D32_F") { return G2Core::DataFormat::Internal::D32_F; }
	return G2Core::DataFormat::Internal::UNKNOWN;
}

G2Core::DataFormat::Base::Name
Texture::getBaseFormatByInternalFormat(G2Core::DataFormat::Internal::Name format)
{
	if (format == G2Core::DataFormat::Internal::R8_UI) { return G2Core::DataFormat::Base::RED; }
	if (format == G2Core::DataFormat::Internal::R8_I) { return G2Core::DataFormat::Base::RED; }
	if (format == G2Core::DataFormat::Internal::R16_UI) { return G2Core::DataFormat::Base::RED; }
	if (format == G2Core::DataFormat::Internal::R16_I) { return G2Core::DataFormat::Base::RED; }
	if (format == G2Core::DataFormat::Internal::R16_F) { return G2Core::DataFormat::Base::RED; }
	if (format == G2Core::DataFormat::Internal::R32_UI) { return G2Core::DataFormat::Base::RED; }
	if (format == G2Core::DataFormat::Internal::R32_I) { return G2Core::DataFormat::Base::RED; }
	if (format == G2Core::DataFormat::Internal::R32_F) { return G2Core::DataFormat::Base::RED; }
	if (format == G2Core::DataFormat::Internal::R8G8_UI) { return G2Core::DataFormat::Base::RG; }
	if (format == G2Core::DataFormat::Internal::R8G8_I) { return G2Core::DataFormat::Base::RG; }
	if (format == G2Core::DataFormat::Internal::R16G16_UI) { return G2Core::DataFormat::Base::RG; }
	if (format == G2Core::DataFormat::Internal::R16G16_I) { return G2Core::DataFormat::Base::RG; }
	if (format == G2Core::DataFormat::Internal::R16G16_F) { return G2Core::DataFormat::Base::RG; }
	if (format == G2Core::DataFormat::Internal::R32G32_UI) { return G2Core::DataFormat::Base::RG; }
	if (format == G2Core::DataFormat::Internal::R32G32_I) { return G2Core::DataFormat::Base::RG; }
	if (format == G2Core::DataFormat::Internal::R32G32_F) { return G2Core::DataFormat::Base::RG; }
	if (format == G2Core::DataFormat::Internal::R32G32B32_UI) { return G2Core::DataFormat::Base::RGB; }
	if (format == G2Core::DataFormat::Internal::R32G32B32_I) { return G2Core::DataFormat::Base::RGB; }
	if (format == G2Core::DataFormat::Internal::R32G32B32_F) { return G2Core::DataFormat::Base::RGB; }
	if (format == G2Core::DataFormat::Internal::R32G32B32A32_UI) { return G2Core::DataFormat::Base::RGBA; }
	if (format == G2Core::DataFormat::Internal::R32G32B32A32_I) { return G2Core::DataFormat::Base::RGBA; }
	if (format == G2Core::DataFormat::Internal::R32G32B32A32_F) { return G2Core::DataFormat::Base::RGBA; }
	if (format == G2Core::DataFormat::Internal::D16_F) { return G2Core::DataFormat::Base::DEPTH; }
	if (format == G2Core::DataFormat::Internal::D24_F) { return G2Core::DataFormat::Base::DEPTH; }
	if (format == G2Core::DataFormat::Internal::D32_F) { return G2Core::DataFormat::Base::DEPTH; }
	return G2Core::DataFormat::Base::UNKNOWN;
}

bool
Texture::isCompressedFormat(G2Core::DataFormat::Internal::Name baseFormat)
{
	return false;
	//return baseFormat == GL_COMPRESSED_RED ||
	//	baseFormat == GL_COMPRESSED_RG ||
	//	baseFormat == GL_COMPRESSED_RGB ||
	//	baseFormat == GL_COMPRESSED_RGBA ||
	//	baseFormat == GL_COMPRESSED_SRGB ||
	//	baseFormat == GL_COMPRESSED_SRGB_ALPHA ||
	//	baseFormat == GL_COMPRESSED_RED_RGTC1 ||
	//	baseFormat == GL_COMPRESSED_SIGNED_RED_RGTC1 ||
	//	baseFormat == GL_COMPRESSED_RG_RGTC2 ||
	//	baseFormat == GL_COMPRESSED_SIGNED_RG_RGTC2 ||
	//	baseFormat == GL_COMPRESSED_RGBA_BPTC_UNORM ||
	//	baseFormat == GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM ||
	//	baseFormat == GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT ||
	//	baseFormat == GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
}