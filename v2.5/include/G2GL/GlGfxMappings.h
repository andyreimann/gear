#pragma once

#include <G2Core/GfxDevice.h>

void _initMappings();

unsigned int toGlBufferAccessMode(G2Core::BufferAccessMode::Name mode);
unsigned int toGlDataFormat(G2Core::DataFormat::Name dataFormat);
unsigned int toGlFrameBufferAttachmentPoint(G2Core::FrameBufferAttachmentPoint::Name attachmentPoint);
unsigned int toGlTextureFormat(G2Core::TextureFormat::Name textureFormat);
unsigned int toGlWrapMode(G2Core::WrapMode::Name wrapMode);
unsigned int toGlFilterMode(G2Core::FilterMode::Name filterMode);
unsigned int toGlDrawMode(G2Core::DrawMode::Name drawMode);