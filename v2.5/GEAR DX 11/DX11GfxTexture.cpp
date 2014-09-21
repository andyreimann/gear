#include "DX11GfxApi.h"
#include "DX11GfxData.h"

#include <G2/Logger.h>

G2Core::GfxResource* CreateTexture2D(
	unsigned int width, 
	unsigned int height, 
	G2Core::DataFormat::Name format, 
	G2Core::DataFormat::Name internalFormat, 
	G2Core::FilterMode::Name minFilter, 
	G2Core::FilterMode::Name magFilter, 
	G2Core::WrapMode::Name wrapS, 
	G2Core::WrapMode::Name wrapT,
	unsigned char * data)
{
	return new G2DX11::Texture2DResource();
}

G2Core::GfxResource* CreateTexture2DArray(
	unsigned int width, 
	unsigned int height, 
	unsigned int size, 
	G2Core::DataFormat::Name format, 
	G2Core::DataFormat::Name internalFormat, 
	G2Core::FilterMode::Name minFilter, 
	G2Core::FilterMode::Name magFilter, 
	G2Core::WrapMode::Name wrapS, 
	G2Core::WrapMode::Name wrapT,
	unsigned char * data)
{
	return new G2DX11::Texture2DArrayResource();
}

G2Core::GfxResource* CreateTextureCube(
	unsigned int width, 
	unsigned int height, 
	G2Core::DataFormat::Name format, 
	G2Core::DataFormat::Name internalFormat, 
	G2Core::FilterMode::Name minFilter, 
	G2Core::FilterMode::Name magFilter, 
	G2Core::WrapMode::Name wrapS, 
	G2Core::WrapMode::Name wrapT)
{
	return new G2DX11::TextureCubeResource();
}

G2Core::GfxResource* CreateTexture3D(
	unsigned int width, 
	unsigned int height, 
	unsigned int depth, 
	G2Core::DataFormat::Name format, 
	G2Core::DataFormat::Name internalFormat, 
	G2Core::FilterMode::Name minFilter, 
	G2Core::FilterMode::Name magFilter, 
	G2Core::WrapMode::Name wrapS, 
	G2Core::WrapMode::Name wrapT,
	G2Core::WrapMode::Name wrapR,
	unsigned char * data)
{
	return new G2DX11::Texture3DResource();
}

void BindTexture(G2Core::GfxResource* texResource, G2Core::TexSlot::Name texSlot)
{
	G2DX11::TextureResource* texPtr = static_cast<G2DX11::TextureResource*>(texResource);
}

void UnbindTexture(G2Core::GfxResource* texResource, G2Core::TexSlot::Name texSlot)
{
	G2DX11::TextureResource* texPtr = static_cast<G2DX11::TextureResource*>(texResource);
}
	