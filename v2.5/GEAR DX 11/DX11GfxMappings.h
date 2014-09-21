#pragma once
#include "DX11GfxApi.h"

#include <G2Core/GfxDevice.h>

void _initMappings();

unsigned int toD3DBufferAccessMode(G2Core::BufferAccessMode::Name mode);
unsigned int toD3DDataFormat(G2Core::DataFormat::Name dataFormat);
unsigned int toD3DFrameBufferAttachmentPoint(G2Core::FrameBufferAttachmentPoint::Name attachmentPoint);
unsigned int toD3DTextureFormat(G2Core::TextureFormat::Name textureFormat);
unsigned int toD3DWrapMode(G2Core::WrapMode::Name wrapMode);
unsigned int toD3DFilterMode(G2Core::FilterMode::Name filterMode);
LPCSTR toD3DSemanticString(G2Core::Semantics::Name semantic);
D3D11_PRIMITIVE_TOPOLOGY toD3DDrawMode(G2Core::DrawMode::Name drawMode);
