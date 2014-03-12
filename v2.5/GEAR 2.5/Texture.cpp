// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Texture.h"

using namespace G2;


Texture::Texture(int type) 
	: mId(GL_INVALID_VALUE),
	mType(type)
{}


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
		dstFormat == LUMINANCE_UB || 
		dstFormat == LUMINANCE_US || 
		dstFormat == LUMINANCE16_F || 
		dstFormat == LUMINANCE_F || 
		dstFormat == LUMINANCE_B || 
		dstFormat == LUMINANCE_S || 
		dstFormat == LUMINANCE_I || 
		dstFormat == LUMINANCE_UI ||
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