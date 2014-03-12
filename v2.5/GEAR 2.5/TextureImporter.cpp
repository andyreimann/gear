// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "TextureImporter.h"
#include "Logger.h"

#include <IL/il.h>

using namespace G2;

std::shared_ptr<Texture2D>
TextureImporter::importResource(std::string const& fileName, unsigned minFilter, unsigned magFilter, bool compress) 
{
	
	auto it = mCache.find(fileName);
	if(it != mCache.end())
	{
		// cache hit
		return it->second->build(minFilter, magFilter, compress);
	}

	logger << "[TextureImporter] Import image file " << fileName << endl;

	Texture2D::init();
	
	ILuint imageID;				// index for DevIL texture
	ilGenImages(1,&imageID);	// generate IL-ID for texture
	ilBindImage(imageID);		// bind ID as current Texture
	ILboolean ret = ilLoadImage ( fileName.c_str() );
	if( !ret ) 
	{
		ilDeleteImages(1,&imageID);
		return std::shared_ptr<Texture2D>();
	}
	
	// Step 1: create builder and fill
	std::shared_ptr<Texture2D::Builder> builder = std::shared_ptr<Texture2D::Builder>(new Texture2D::Builder);
	builder->id = imageID;
	ilBindImage(0);
	// (Step 2: cache builder)
	mCache[fileName] = builder;
	// Step 3: build and return resource
	return builder->build(minFilter, magFilter, compress);
}