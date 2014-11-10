#include "DX11GfxApi.h"
#include "DX11GfxData.h"
#include "DX11GfxMappings.h"

G2Core::GfxResource* CreateRenderTarget(unsigned int width, unsigned int height, G2Core::DataFormat::Internal::Name format)
{
	G2DX11::RenderTargetResource* renderTarget = new G2DX11::RenderTargetResource();
	return renderTarget;
}

void AttachTextureToRenderTarget(G2Core::GfxResource* renderTarget, G2Core::GfxResource* texture, G2Core::FrameBufferAttachmentPoint::Name attachment, G2Core::TextureFormat::Name texTarget, int mipmapLevel, int layer)
{
	G2DX11::TextureResource* tex = static_cast<G2DX11::TextureResource*>(texture);
}

void BindRenderTarget(G2Core::GfxResource* renderTarget)
{
	G2DX11::RenderTargetResource* renderTargetPtr = static_cast<G2DX11::RenderTargetResource*>(renderTarget);
}

void UnbindRenderTarget(G2Core::GfxResource* renderTarget)
{
	G2DX11::RenderTargetResource* renderTargetPtr = static_cast<G2DX11::RenderTargetResource*>(renderTarget);
}
