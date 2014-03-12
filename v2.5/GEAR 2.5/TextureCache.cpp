// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "TextureCache.h"

using namespace G2;

TextureCache* TextureCache::mInstance_ = nullptr;

TextureCache&
TextureCache::getShared()
{
	if( !mInstance_ )
		mInstance_ = new TextureCache();
	return *mInstance_;
}

void TextureCache::destroy()
{
	if(mInstance_)
		delete mInstance_;
}

TextureCache::~TextureCache()
{

}
