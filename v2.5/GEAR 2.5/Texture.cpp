// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Texture.h"

using namespace G2;


Texture::Texture(int type) 
	: mId(GL_INVALID_VALUE),
	mType(type)
{}

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
	mId = rhs.mId;
	mType = rhs.mType;
	mWrapModeS = rhs.mWrapModeS;
	mWrapModeR = rhs.mWrapModeR;
	mWrapModeT = rhs.mWrapModeT;
	// 3. Stage: modify src to a well defined state
	rhs.mId = GL_INVALID_VALUE;

	return *this;
}

Texture::~Texture() 
{
	GLDEBUG( glDeleteTextures( 1, &mId ) );
}

void
Texture::bind(unsigned textureSlot) const 
{
	GLDEBUG( glActiveTexture( textureSlot ) );
	GLDEBUG( glBindTexture( mType, mId ) );
}

void
Texture::unbind(unsigned textureSlot) const 
{
	GLDEBUG( glActiveTexture( textureSlot ) );
	GLDEBUG( glBindTexture( mType, 0 ) );
}

GLuint 
Texture::numChannelsFromFormat( GLuint dstFormat ) {
	if(	dstFormat == RGBA_UB || 
		dstFormat == RGBA_US || 
		dstFormat == RGBA16_F || 
		dstFormat == RGBA_F || 
		dstFormat == RGBA_B || 
		dstFormat == RGBA_S || 
		dstFormat == RGBA_I || 
		dstFormat == RGBA_UI || 
		dstFormat == RGBA ) 
			return 4;
	else if( dstFormat == RGB || 
		dstFormat == RGB_UB || 
		dstFormat == RGB_US) 
		return 3;
	else if( dstFormat == ALPHA_UB || 
		dstFormat == ALPHA_US || 
		dstFormat == ALPHA16_F || 
		dstFormat == ALPHA_F || 
		dstFormat == ALPHA_B || 
		dstFormat == ALPHA_S || 
		dstFormat == ALPHA_I || 
		dstFormat == ALPHA_UI || 
		dstFormat == LUMINANCE || 
		dstFormat == LUMINANCE_UB || 
		dstFormat == LUMINANCE_US || 
		dstFormat == LUMINANCE16_F || 
		dstFormat == LUMINANCE_F || 
		dstFormat == LUMINANCE_B || 
		dstFormat == LUMINANCE_S || 
		dstFormat == LUMINANCE_I || 
		dstFormat == LUMINANCE_UI ||
		dstFormat == DEPTH || 
		dstFormat == DEPTH16 || 
		dstFormat == DEPTH24 || 
		dstFormat == DEPTH32 || 
		dstFormat == INTENSITY_UB || 
		dstFormat == INTENSITY_US || 
		dstFormat == INTENSITY16_F || 
		dstFormat == INTENSITY_F || 
		dstFormat == INTENSITY_B || 
		dstFormat == INTENSITY_S || 
		dstFormat == INTENSITY_I || 
		dstFormat == INTENSITY_UI ) {
			return 1;
	}
	else if( dstFormat == LUMINANCE_ALPHA_UB || 
		dstFormat == LUMINANCE_ALPHA_US || 
		dstFormat == LUMINANCE_ALPHA16_F || 
		dstFormat == LUMINANCE_ALPHA_F || 
		dstFormat == LUMINANCE_ALPHA_B || 
		dstFormat == LUMINANCE_ALPHA_S || 
		dstFormat == LUMINANCE_ALPHA_I ) {
			return 2;
	}
	assert(0);
	return 0;
}

GLuint 
Texture::convertFormat( GLuint baseFormat, bool compress ) {
	
	if( !compress ) // no compression requested or compression not supported
		return baseFormat;

	switch( baseFormat ) {
		case GL_ALPHA:
			return GL_COMPRESSED_ALPHA;
			break;
		case GL_LUMINANCE:
			return GL_COMPRESSED_LUMINANCE;
			break;
		case GL_LUMINANCE_ALPHA:
			return GL_COMPRESSED_LUMINANCE_ALPHA;
			break;
		case GL_INTENSITY:
			return GL_COMPRESSED_INTENSITY;
			break;
		case GL_RGB:
			return GL_COMPRESSED_RGB;
			break;
		case GL_RGBA:
			return GL_COMPRESSED_RGBA;
			break;
		default:
			return baseFormat;
	}
}

bool Texture::mipMapsRequested(GLuint minFilter, GLuint magFilter) 
{

	// check the minFilter if it has mipmapping
	if( minFilter == NEAREST_MIPMAP_NEAREST ||
		minFilter == LINEAR_MIPMAP_NEAREST  || 
		minFilter == NEAREST_MIPMAP_LINEAR  || 
		minFilter == LINEAR_MIPMAP_LINEAR ) 
	{
		return true;
	}
	return false;
}

void Texture::checkFilter(GLuint minFilter, GLuint magFilter) 
{
	assert( minFilter == NEAREST || 
		    minFilter == LINEAR ||
			minFilter == NEAREST_MIPMAP_NEAREST ||
			minFilter == LINEAR_MIPMAP_NEAREST  || 
			minFilter == NEAREST_MIPMAP_LINEAR  || 
			minFilter == LINEAR_MIPMAP_LINEAR );

	assert( magFilter == NEAREST || magFilter == LINEAR );
}



unsigned 
Texture::baseFormatToCompressedFormat( unsigned baseFormat, bool compress ) 
{
	if( !compress /*|| !mTexCompressionSupported*/ ) // no compression requested or compression not supported
		return baseFormat;
	switch( baseFormat ) 
	{
		case GL_ALPHA:
			return GL_COMPRESSED_ALPHA;
			break;
		case GL_LUMINANCE:
			return GL_COMPRESSED_LUMINANCE;
			break;
		case GL_LUMINANCE_ALPHA:
			return GL_COMPRESSED_LUMINANCE_ALPHA;
			break;
		case GL_INTENSITY:
			return GL_COMPRESSED_INTENSITY;
			break;
		case GL_RGB:
			return GL_COMPRESSED_RGB;
			break;
		case GL_RGBA:
			return GL_COMPRESSED_RGBA;
			break;
		default:
			return baseFormat;
			break;
	}
}

unsigned
Texture::getFormatByString(std::string const& format) 
{
	if(format == "RGB") { return RGB; }
	if(format == "RGB_UB") { return RGB_UB; }
	if(format == "RGB_US") { return RGB_US; }
	if(format == "RGBA") { return RGBA; }
	if(format == "RGBA_UB") { return RGBA_UB; }
	if(format == "RGBA_US") { return RGBA_US; }
	if(format == "RGBA16_F") { return RGBA16_F; }
	if(format == "RGBA_F") { return RGBA_F; }
	if(format == "RGBA_B") { return RGBA_B; }
	if(format == "RGBA_S") { return RGBA_S; }
	if(format == "RGBA_I") { return RGBA_I; }
	if(format == "RGBA_UI") { return RGBA_UI; }
	if(format == "ALPHA_UB") { return ALPHA_UB; }
	if(format == "ALPHA_US") { return ALPHA_US; }
	if(format == "ALPHA16_F") { return ALPHA16_F; }
	if(format == "ALPHA_B") { return ALPHA_B; }
	if(format == "ALPHA_S") { return ALPHA_S; }
	if(format == "ALPHA_I") { return ALPHA_I; }
	if(format == "ALPHA_UI") { return ALPHA_UI; }
	if(format == "LUMINANCE") { return LUMINANCE; }
	if(format == "DEPTH") { return DEPTH; }
	if(format == "DEPTH16") { return DEPTH16; }
	if(format == "DEPTH24") { return DEPTH24; }
	if(format == "DEPTH32") { return DEPTH32; }
	if(format == "LUMINANCE_ALPHA_UB") { return LUMINANCE_ALPHA_UB; }
	if(format == "LUMINANCE_ALPHA_US") { return LUMINANCE_ALPHA_US; }
	if(format == "LUMINANCE_ALPHA16_F") { return LUMINANCE_ALPHA16_F; }
	if(format == "LUMINANCE_ALPHA_F") { return LUMINANCE_ALPHA_F; }
	if(format == "LUMINANCE_ALPHA_B") { return LUMINANCE_ALPHA_B; }
	if(format == "LUMINANCE_ALPHA_S") { return LUMINANCE_ALPHA_S; }
	if(format == "LUMINANCE_ALPHA_I") { return LUMINANCE_ALPHA_I; }
	if(format == "LUMINANCE_ALPHA_UI") { return LUMINANCE_ALPHA_UI; }
	if(format == "LUMINANCE_UB") { return LUMINANCE_UB; }
	if(format == "LUMINANCE_US") { return LUMINANCE_US; }
	if(format == "LUMINANCE16_F") { return LUMINANCE16_F; }
	if(format == "LUMINANCE_F") { return LUMINANCE_F; }
	if(format == "LUMINANCE_B") { return LUMINANCE_B; }
	if(format == "LUMINANCE_S") { return LUMINANCE_S; }
	if(format == "LUMINANCE_I") { return LUMINANCE_I; }
	if(format == "LUMINANCE_UI") { return LUMINANCE_UI; }
	if(format == "INTENSITY") { return INTENSITY; }
	if(format == "INTENSITY_UB") { return INTENSITY_UB; }
	if(format == "INTENSITY_US") { return INTENSITY_US; }
	if(format == "INTENSITY16_F") { return INTENSITY16_F; }
	if(format == "INTENSITY_F") { return INTENSITY_F; }
	if(format == "INTENSITY_B") { return INTENSITY_B; }
	if(format == "INTENSITY_S") { return INTENSITY_S; }
	if(format == "INTENSITY_I") { return INTENSITY_I; }
	if(format == "INTENSITY_UI") { return INTENSITY_UI; }
	return GL_INVALID_VALUE;
}

WrapMode::Name
WrapMode::getWrapMode(std::string const& name) 
{
	if(name == "CLAMP_TO_EDGE") 
	{
		return CLAMP_TO_EDGE;
	}
	if(name == "CLAMP_TO_BORDER") 
	{
		return CLAMP_TO_BORDER;
	}
	else if(name == "MIRRORED_REPEAT") 
	{
		return MIRRORED_REPEAT;
	}
	else if(name == "MIRROR_CLAMP_TO_EDGE") 
	{
		return MIRROR_CLAMP_TO_EDGE;
	}
	else if(name == "REPEAT") 
	{
		return REPEAT;
	}
	return WRAP_MODE_INVALID;
}