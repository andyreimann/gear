// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "TextureImporter.h"
#include "Logger.h"

#include <IL/il.h>

using namespace G2;

std::shared_ptr<Texture2D>
TextureImporter::importResource(std::string const& fileName, G2Core::FilterMode::Name minFilter, G2Core::FilterMode::Name magFilter, bool compress, G2Core::WrapMode::Name wrapS, G2Core::WrapMode::Name wrapT, G2Core::DataFormat::Name internalFormat) 
{
	
	auto it = mCache.find(fileName);
	if(it != mCache.end())
	{
		// cache hit
		return it->second->build(minFilter, magFilter, compress, wrapS, wrapT, internalFormat);
	}
	// should never occur
	return std::shared_ptr<Texture2D>();
}

std::pair<std::string,std::shared_ptr<Texture2D::Builder>> 
TextureImporter::produceResourceBuilder(std::string const& fileName, G2Core::FilterMode::Name minFilter, G2Core::FilterMode::Name magFilter, bool compress, G2Core::WrapMode::Name wrapS, G2Core::WrapMode::Name wrapT, G2Core::DataFormat::Name internalFormat) 
{
	logger << "[TextureImporter] Import image file " << fileName << endl;

	Texture2D::init();
	
	ILuint imageID;				// index for DevIL texture
	ilGenImages(1,&imageID);	// generate IL-ID for texture
	ilBindImage(imageID);		// bind ID as current Texture
	ILboolean ret = ilLoadImage ( fileName.c_str() );
	if(!ret ) 
	{
		ilDeleteImages(1,&imageID);
	}
	
	// Step 1: create builder and fill
	std::shared_ptr<Texture2D::Builder> builder = std::shared_ptr<Texture2D::Builder>(new Texture2D::Builder);
	builder->id = imageID;
	ilBindImage(0);
	return std::make_pair(fileName, builder);
}