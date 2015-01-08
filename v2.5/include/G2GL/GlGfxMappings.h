#pragma once

#include <G2Core/GfxDevice.h>

void _initMappings();

unsigned int toGlBufferAccessMode(G2Core::BufferAccessMode::Name mode);
unsigned int toGlBaseDataFormat(G2Core::DataFormat::Base::Name dataFormat);
unsigned int toGlInternalDataFormat(G2Core::DataFormat::Internal::Name dataFormat);
unsigned int toGlFrameBufferAttachmentPoint(G2Core::FrameBufferAttachmentPoint::Name attachmentPoint);
unsigned int toGlTextureFormat(G2Core::TextureFormat::Name textureFormat);
unsigned int toGlWrapMode(G2Core::WrapMode::Name wrapMode);
unsigned int toGlFilterMode(G2Core::FilterMode::Name filterMode);
unsigned int toGlDrawMode(G2Core::DrawMode::Name drawMode);
unsigned int toGlDataType(G2Core::DataType::Name dataType);
unsigned int toGlPolygonDrawMode(G2Core::PolygonDrawMode::Name drawMode);
unsigned int toGlFaceCullingMode(G2Core::FaceCulling::Name faceCullingMode);
unsigned int toGlBlendFunc(G2Core::BlendFactor::Name blendFunc);
unsigned int toGlBufferUsage(G2Core::BufferUsage::Name usage);
unsigned int toGlDepthFunc(G2Core::DepthFunc::Name depthFunc);
