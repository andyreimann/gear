#include "GlGfxApi.h"
#include "GlGfxData.h"
#include "GlGfxMappings.h"

#include <G2/Logger.h>

#include <IL/il.h>

std::unordered_map<std::string, ILuint> gTextureCache;

bool _mipMapsRequested(G2Core::FilterMode::Name minFilter, G2Core::FilterMode::Name magFilter) 
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

void _checkFilterValidity(G2Core::FilterMode::Name minFilter, G2Core::FilterMode::Name magFilter) 
{
	assert( minFilter == G2Core::FilterMode::NEAREST || 
		    minFilter == G2Core::FilterMode::LINEAR ||
			minFilter == G2Core::FilterMode::NEAREST_MIPMAP_NEAREST ||
			minFilter == G2Core::FilterMode::LINEAR_MIPMAP_NEAREST  || 
			minFilter == G2Core::FilterMode::NEAREST_MIPMAP_LINEAR  || 
			minFilter == G2Core::FilterMode::LINEAR_MIPMAP_LINEAR );

	assert( magFilter == G2Core::FilterMode::NEAREST || magFilter == G2Core::FilterMode::LINEAR );
}



bool _isCompressedFormat(G2Core::DataFormat::Base::Name baseFormat)
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

G2Core::GfxResource* CreateTexture2D(
	unsigned int width, 
	unsigned int height, 
	G2Core::DataFormat::Base::Name format,
	G2Core::DataFormat::Internal::Name internalFormat,
	G2Core::FilterMode::Name minFilter, 
	G2Core::FilterMode::Name magFilter, 
	G2Core::WrapMode::Name wrapS, 
	G2Core::WrapMode::Name wrapT,
	G2Core::DataType::Name dataType,
	void* data)
{
	_checkFilterValidity(minFilter,magFilter);

	//if(isCompressedFormat(internalFormat))
	//{
	//	mCompressed = true; // if user gives an already compressed format, adjust compressed flag
	//}

	unsigned int texId;
	unsigned int glFormat = toGlBaseDataFormat(format);
	unsigned int glInternalFormat = toGlInternalDataFormat(internalFormat);
	unsigned int glWrapS = toGlWrapMode(wrapS);
	unsigned int glWrapT = toGlWrapMode(wrapT);
	unsigned int glMinFilter = toGlFilterMode(minFilter);
	unsigned int glMagFilter = toGlFilterMode(magFilter);
	unsigned int glDataType = toGlDataType(dataType);
	GLCHECK( glGenTextures(1, &texId) );
	GLCHECK( glBindTexture(GL_TEXTURE_2D, texId) );
	GLCHECK( glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		glInternalFormat, 
		width, 
		height, 
		0, 
		glFormat, 
		glDataType,
		data) );

	if(_mipMapsRequested(minFilter,magFilter))
	{
		GLCHECK( glGenerateMipmap(GL_TEXTURE_2D) ); // generates mipmaps now!
	}

	GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrapS) );
	GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrapT) );
	GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMinFilter) );
	GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMagFilter) );
	
	//GLint compFlag;
	//GLCHECK( glGetTexLevelParameteriv(mType, 0, GL_TEXTURE_COMPRESSED, &compFlag) );
	GLCHECK( glBindTexture(GL_TEXTURE_2D, 0) );

	return new G2GL::Texture2DResource(
		texId,
		glInternalFormat,
		glMinFilter,
		glMagFilter,
		glWrapS,
		glWrapT
	);
}

G2Core::GfxResource* CreateTexture2DArray(
	unsigned int width, 
	unsigned int height, 
	unsigned int size, 
	G2Core::DataFormat::Base::Name format,
	G2Core::DataFormat::Internal::Name internalFormat,
	G2Core::FilterMode::Name minFilter, 
	G2Core::FilterMode::Name magFilter, 
	G2Core::WrapMode::Name wrapS, 
	G2Core::WrapMode::Name wrapT,
	G2Core::DataType::Name dataType,
	void* data)
{
	_checkFilterValidity(minFilter,magFilter);

	//if(isCompressedFormat(internalFormat))
	//{
	//	mCompressed = true; // if user gives an already compressed format, adjust compressed flag
	//}

	unsigned int texId;
	unsigned int glFormat = toGlBaseDataFormat(format);
	unsigned int glInternalFormat = toGlInternalDataFormat(internalFormat);
	unsigned int glWrapS = toGlWrapMode(wrapS);
	unsigned int glWrapT = toGlWrapMode(wrapT);
	unsigned int glMinFilter = toGlFilterMode(minFilter);
	unsigned int glMagFilter = toGlFilterMode(magFilter);
	unsigned int glDataType = toGlDataType(dataType);
	GLCHECK( glGenTextures(1, &texId) );
	GLCHECK( glBindTexture(GL_TEXTURE_2D_ARRAY, texId) );
	GLCHECK(glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, glInternalFormat, width, height, size, 0, glFormat, glDataType, data));
	
	if(_mipMapsRequested(minFilter,magFilter))
	{
		GLCHECK( glGenerateMipmap(GL_TEXTURE_2D_ARRAY) ); // generates mipmaps now!
	}

	GLCHECK( glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, glWrapS) );
	GLCHECK( glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, glWrapT) );
	GLCHECK( glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, glMinFilter) );
	GLCHECK( glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, glMagFilter) );
	
	//GLint compFlag;
	//GLCHECK( glGetTexLevelParameteriv(mType, 0, GL_TEXTURE_COMPRESSED, &compFlag) );
	GLCHECK( glBindTexture(GL_TEXTURE_2D_ARRAY, 0) );

	return new G2GL::Texture2DArrayResource(
		texId,
		glInternalFormat,
		glMinFilter,
		glMagFilter,
		glWrapS,
		glWrapT
	);
}

G2Core::GfxResource* CreateTextureCube(
	unsigned int width, 
	unsigned int height, 
	G2Core::DataFormat::Base::Name format,
	G2Core::DataFormat::Internal::Name internalFormat, 
	G2Core::FilterMode::Name minFilter, 
	G2Core::FilterMode::Name magFilter, 
	G2Core::WrapMode::Name wrapS, 
	G2Core::WrapMode::Name wrapT)
{
	_checkFilterValidity(minFilter,magFilter);

	static const unsigned int CubeMapDefines[6] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	unsigned int texId;
	unsigned int glFormat = toGlBaseDataFormat(format);
	unsigned int glInternalFormat = toGlInternalDataFormat(internalFormat);
	unsigned int glWrapS = toGlWrapMode(wrapS);
	unsigned int glWrapT = toGlWrapMode(wrapT);
	unsigned int glMinFilter = toGlFilterMode(minFilter);
	unsigned int glMagFilter = toGlFilterMode(magFilter);
	GLCHECK( glGenTextures(1, &texId) );
	GLCHECK( glBindTexture(GL_TEXTURE_CUBE_MAP, texId) );

	GLCHECK( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, glWrapS) );
	GLCHECK( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, glWrapT) );
	GLCHECK( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, glMinFilter) );
	GLCHECK( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, glMagFilter) );
	
	for(int i = 0; i < 6; ++i)
	{
		GLCHECK( glTexImage2D(CubeMapDefines[i], 0, glInternalFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, nullptr) );
	}

	GLCHECK( glBindTexture(GL_TEXTURE_CUBE_MAP, 0) );

	return new G2GL::TextureCubeResource(
		texId,
		glInternalFormat,
		glMinFilter,
		glMagFilter,
		glWrapS,
		glWrapT
	);
}

G2Core::GfxResource* CreateTexture3D(
	unsigned int width, 
	unsigned int height, 
	unsigned int depth, 
	G2Core::DataFormat::Base::Name format,
	G2Core::DataFormat::Internal::Name internalFormat,
	G2Core::FilterMode::Name minFilter, 
	G2Core::FilterMode::Name magFilter, 
	G2Core::WrapMode::Name wrapS, 
	G2Core::WrapMode::Name wrapT,
	G2Core::WrapMode::Name wrapR,
	G2Core::DataType::Name dataType,
	void* data)
{
	_checkFilterValidity(minFilter,magFilter);

	//if(isCompressedFormat(internalFormat))
	//{
	//	mCompressed = true; // if user gives an already compressed format, adjust compressed flag
	//}

	unsigned int texId;
	unsigned int glFormat = toGlBaseDataFormat(format);
	unsigned int glInternalFormat = toGlInternalDataFormat(internalFormat);
	unsigned int glWrapS = toGlWrapMode(wrapS);
	unsigned int glWrapT = toGlWrapMode(wrapT);
	unsigned int glWrapR = toGlWrapMode(wrapR);
	unsigned int glMinFilter = toGlFilterMode(minFilter);
	unsigned int glMagFilter = toGlFilterMode(magFilter);
	unsigned int glDataType = toGlDataType(dataType);
	GLCHECK( glGenTextures(1, &texId) );
	GLCHECK( glBindTexture(GL_TEXTURE_3D, texId) );
	GLCHECK(glTexImage3D(GL_TEXTURE_3D, 0, glInternalFormat, width, height, depth, 0, glFormat, glDataType, data));
	
	if(_mipMapsRequested(minFilter,magFilter))
	{
		GLCHECK( glGenerateMipmap(GL_TEXTURE_3D) ); // generates mipmaps now!
	}

	GLCHECK( glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, glWrapS) );
	GLCHECK( glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, glWrapT) );
	GLCHECK( glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, glWrapR) );
	GLCHECK( glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, glMinFilter) );
	GLCHECK( glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, glMagFilter) );
	
	//GLint compFlag;
	//GLCHECK( glGetTexLevelParameteriv(mType, 0, GL_TEXTURE_COMPRESSED, &compFlag) );
	GLCHECK( glBindTexture(GL_TEXTURE_3D, 0) );

	return new G2GL::Texture3DResource(
		texId,
		glInternalFormat,
		glMinFilter,
		glMagFilter,
		glWrapS,
		glWrapT,
		glWrapR
	);
}

G2Core::GfxResource* CreateTexture2DFromFile(
	std::string const& fileName,
	G2Core::DataFormat::Internal::Name internalFormat,
	G2Core::FilterMode::Name minFilter,
	G2Core::FilterMode::Name magFilter,
	G2Core::WrapMode::Name wrapS,
	G2Core::WrapMode::Name wrapT,
	G2Core::TextureMetaData* metaData)
{
	bool cacheHit = false;
	ILuint imageID;				// index for DevIL texture
	if (gTextureCache.find(fileName) != gTextureCache.end())
	{
		imageID = gTextureCache[fileName];
		cacheHit = true;
	}
	else
	{
		ilGenImages(1, &imageID);	// generate IL-ID for texture
		ilBindImage(imageID);		// bind ID as current Texture
		ILboolean ret = ilLoadImage(fileName.c_str());
		if (!ret)
		{
			ilDeleteImages(1, &imageID);
			if (metaData != nullptr)
			{
				metaData->status = G2Core::TextureMetaData::INVALID_ARG;
				metaData->error = "Could not load texture";
			}
			return error();
		}
	}
	ilBindImage(imageID);		// bind ID as current Texture

	unsigned char * data = ilGetData();
	if (data == NULL)
	{
		ilDeleteImages(1, &imageID);
		if (metaData != nullptr)
		{
			metaData->status = G2Core::TextureMetaData::INVALID_ARG;
			metaData->error = "Could not read texture data";
		}
		return error();
	}
	unsigned width = ilGetInteger(IL_IMAGE_WIDTH);
	unsigned height = ilGetInteger(IL_IMAGE_HEIGHT);
	if (metaData != nullptr)
	{
		metaData->width = width;
		metaData->height = height;
	}
	unsigned format = ilGetInteger(IL_IMAGE_FORMAT);

	G2Core::DataFormat::Base::Name parsedFormat = G2Core::DataFormat::Base::UNKNOWN;
	switch (format)
	{
		case IL_RGB:			parsedFormat = G2Core::DataFormat::Base::RGB; break;
		case IL_RGBA:			parsedFormat = G2Core::DataFormat::Base::RGBA; break;
		case IL_LUMINANCE:			parsedFormat = G2Core::DataFormat::Base::LUMINANCE; break;
		default:
			if (metaData != nullptr)
			{
				metaData->status = G2Core::TextureMetaData::FORMAT_NOT_SUPPORTED;
				metaData->error = "Could not obtain texture source format";
			}
			return error();
	}
	assert(parsedFormat != G2Core::DataFormat::Base::UNKNOWN);


	G2Core::GfxResource* tex = CreateTexture2D(
		width, height,
		parsedFormat, internalFormat,
		minFilter, magFilter, wrapS, wrapT, G2Core::DataType::UNSIGNED_BYTE, data);

	//ilDeleteImages(1, &imageID);
	if (!cacheHit)
	{
		// create cache entry
		gTextureCache[fileName] = imageID;
	}
	return tex;
}

const G2Core::TextureFormat::Name CubeMapDefines[6] = {
	G2Core::TextureFormat::CUBE_MAP_POS_X,
	G2Core::TextureFormat::CUBE_MAP_NEG_X,
	G2Core::TextureFormat::CUBE_MAP_POS_Y,
	G2Core::TextureFormat::CUBE_MAP_NEG_Y,
	G2Core::TextureFormat::CUBE_MAP_POS_Z,
	G2Core::TextureFormat::CUBE_MAP_NEG_Z
};

void BindTexture(G2Core::GfxResource* texResource, G2Core::TexSlot::Name texSlot)
{
	G2GL::TextureResource* texPtr = static_cast<G2GL::TextureResource*>(texResource);
	GLCHECK( glActiveTexture( GL_TEXTURE0 + (int)texSlot ) );
	//GLCHECK( glActiveTexture( (int)texSlot ) );
	GLCHECK( glBindTexture( texPtr->texType, texPtr->texId ) );
}

void UnbindTexture(G2Core::GfxResource* texResource, G2Core::TexSlot::Name texSlot)
{
	G2GL::TextureResource* texPtr = static_cast<G2GL::TextureResource*>(texResource);
	GLCHECK( glActiveTexture( GL_TEXTURE0 + (int)texSlot ) );
	//GLCHECK( glActiveTexture( (int)texSlot ) );
	GLCHECK( glBindTexture( texPtr->texType, 0 ) );
}
	
void releaseTextureCache()
{
	for (auto it = gTextureCache.begin(); it != gTextureCache.end(); ++it)
	{
		ilDeleteImages(1, &it->second);
	}
}