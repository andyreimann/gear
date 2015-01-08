#include "GlGfxApi.h"
#include "GlGfxData.h"

#include <G2/Logger.h>

#include <unordered_map>

std::unordered_map<G2Core::BufferAccessMode::Name, unsigned int> bufferAccessMapping;
std::unordered_map<G2Core::DataFormat::Base::Name, unsigned int> baseDataFormatMapping;
std::unordered_map<G2Core::DataFormat::Internal::Name, unsigned int> internalDataFormatMapping;
std::unordered_map<G2Core::FrameBufferAttachmentPoint::Name,unsigned int> frameBufferAttachmentPointMapping;
std::unordered_map<G2Core::TextureFormat::Name,unsigned int> textureFormatMapping;
std::unordered_map<G2Core::FilterMode::Name,unsigned int> filterModeMapping;
std::unordered_map<G2Core::WrapMode::Name, unsigned int> wrapModeMapping;
std::unordered_map<G2Core::DrawMode::Name, unsigned int> drawModeMapping;
std::unordered_map<G2Core::DataType::Name, unsigned int> dataTypeMapping;
std::unordered_map<G2Core::PolygonDrawMode::Name, unsigned int> polygonDrawModeMapping;
std::unordered_map<G2Core::FaceCulling::Name, unsigned int> faceCullingModeMapping;
std::unordered_map<G2Core::BlendFactor::Name, unsigned int> blendFuncMapping;
std::unordered_map<G2Core::BufferUsage::Name, unsigned int> bufferUsageMappingGl;
std::unordered_map<G2Core::DepthFunc::Name, unsigned int> depthFuncMapping;

void _initMappings()
{
	bufferAccessMapping[G2Core::BufferAccessMode::READ_ONLY]	= GL_READ_ONLY;
	bufferAccessMapping[G2Core::BufferAccessMode::READ_WRITE]	= GL_READ_WRITE;
	bufferAccessMapping[G2Core::BufferAccessMode::WRITE_ONLY] = GL_WRITE_ONLY;

	bufferUsageMappingGl[G2Core::BufferUsage::STREAM_DRAW] = GL_STREAM_DRAW;
	bufferUsageMappingGl[G2Core::BufferUsage::STREAM_READ] = GL_STREAM_READ;
	bufferUsageMappingGl[G2Core::BufferUsage::STREAM_COPY] = GL_STREAM_COPY;
	bufferUsageMappingGl[G2Core::BufferUsage::STATIC_DRAW] = GL_STATIC_DRAW;
	bufferUsageMappingGl[G2Core::BufferUsage::STATIC_READ] = GL_STATIC_READ;
	bufferUsageMappingGl[G2Core::BufferUsage::STATIC_COPY] = GL_STATIC_COPY;
	bufferUsageMappingGl[G2Core::BufferUsage::DYNAMIC_DRAW] = GL_DYNAMIC_DRAW;
	bufferUsageMappingGl[G2Core::BufferUsage::DYNAMIC_READ] = GL_DYNAMIC_READ;
	bufferUsageMappingGl[G2Core::BufferUsage::DYNAMIC_COPY] = GL_DYNAMIC_COPY;

	baseDataFormatMapping[G2Core::DataFormat::Base::RED] = GL_RED;
	baseDataFormatMapping[G2Core::DataFormat::Base::RG] = GL_RG;
	baseDataFormatMapping[G2Core::DataFormat::Base::RGB] = GL_RGB;
	baseDataFormatMapping[G2Core::DataFormat::Base::RGBA] = GL_RGBA;
	baseDataFormatMapping[G2Core::DataFormat::Base::DEPTH] = GL_DEPTH_COMPONENT;
	baseDataFormatMapping[G2Core::DataFormat::Base::LUMINANCE] = GL_LUMINANCE;

	internalDataFormatMapping[G2Core::DataFormat::Internal::R8_UI] = GL_RED;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R8_I] = GL_RED;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R16_UI] = GL_RED;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R16_I] = GL_RED;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R16_F] = GL_R16F;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32_UI] = GL_RED;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32_I] = GL_RED;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32_F] = GL_R32F;					
	// -------------------------------------------------------------------------
	internalDataFormatMapping[G2Core::DataFormat::Internal::R8G8_UI] = GL_RG;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R8G8_I] = GL_RG;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R16G16_UI] = GL_RG;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R16G16_I] = GL_RG;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R16G16_F] = GL_RG16F;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32_UI] = GL_RG;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32_I] = GL_RG;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32_F] = GL_RG32F;				
	// -------------------------------------------------------------------------
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32B32_UI] = GL_RGB;				
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32B32_I] = GL_RGB;				
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32B32_F] = GL_RGB32F;			
	// -------------------------------------------------------------------------
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32B32A32_UI] = GL_RGBA;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32B32A32_I] = GL_RGBA;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32B32A32_F] = GL_RGBA32F;		
	// -------------------------------------------------------------------------
	internalDataFormatMapping[G2Core::DataFormat::Internal::D16_F] = GL_DEPTH_COMPONENT16;
	internalDataFormatMapping[G2Core::DataFormat::Internal::D24_F] = GL_DEPTH_COMPONENT24;
	internalDataFormatMapping[G2Core::DataFormat::Internal::D32_F] = GL_DEPTH_COMPONENT32;
	// -------------------------------------------------------------------------
	internalDataFormatMapping[G2Core::DataFormat::Internal::LUMINANCE_F] = GL_LUMINANCE;

	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_0]				= GL_COLOR_ATTACHMENT0;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_1]				= GL_COLOR_ATTACHMENT1;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_2]				= GL_COLOR_ATTACHMENT2;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_3]				= GL_COLOR_ATTACHMENT3;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_4]				= GL_COLOR_ATTACHMENT4;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_5]				= GL_COLOR_ATTACHMENT5;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_6]				= GL_COLOR_ATTACHMENT6;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_7]				= GL_COLOR_ATTACHMENT7;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_8]				= GL_COLOR_ATTACHMENT8;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_9]				= GL_COLOR_ATTACHMENT9;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_1]				= GL_COLOR_ATTACHMENT10;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_1]				= GL_COLOR_ATTACHMENT11;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_1]				= GL_COLOR_ATTACHMENT12;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_1]				= GL_COLOR_ATTACHMENT13;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_1]				= GL_COLOR_ATTACHMENT14;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::COLOR_1]				= GL_COLOR_ATTACHMENT15;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::DEPTH]				= GL_DEPTH_ATTACHMENT;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::STENCIL]				= GL_STENCIL_ATTACHMENT;
	frameBufferAttachmentPointMapping[G2Core::FrameBufferAttachmentPoint::ATTACHMENT_INVALID]	= GL_INVALID_VALUE;
	
	textureFormatMapping[G2Core::TextureFormat::TEXTURE_1D]			= GL_TEXTURE_1D;
	textureFormatMapping[G2Core::TextureFormat::TEXTURE_2D]			= GL_TEXTURE_2D;
	textureFormatMapping[G2Core::TextureFormat::TEXTURE_2D_ARRAY]	= GL_TEXTURE_2D_ARRAY;
	textureFormatMapping[G2Core::TextureFormat::TEXTURE_3D]			= GL_TEXTURE_3D;
	textureFormatMapping[G2Core::TextureFormat::TEXTURE_CUBE_MAP]	= GL_TEXTURE_CUBE_MAP;
	textureFormatMapping[G2Core::TextureFormat::CUBE_MAP_POS_X]		= GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	textureFormatMapping[G2Core::TextureFormat::CUBE_MAP_NEG_X]		= GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	textureFormatMapping[G2Core::TextureFormat::CUBE_MAP_POS_Y]		= GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	textureFormatMapping[G2Core::TextureFormat::CUBE_MAP_NEG_Y]		= GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	textureFormatMapping[G2Core::TextureFormat::CUBE_MAP_POS_Z]		= GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	textureFormatMapping[G2Core::TextureFormat::CUBE_MAP_NEG_Z]		= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

	wrapModeMapping[G2Core::WrapMode::CLAMP_TO_EDGE]		= GL_CLAMP_TO_EDGE;
	wrapModeMapping[G2Core::WrapMode::CLAMP_TO_BORDER]		= GL_CLAMP_TO_BORDER;
	wrapModeMapping[G2Core::WrapMode::MIRROR_CLAMP_TO_EDGE] = GL_MIRROR_CLAMP_TO_EDGE;
	wrapModeMapping[G2Core::WrapMode::MIRRORED_REPEAT]		= GL_MIRRORED_REPEAT;
	wrapModeMapping[G2Core::WrapMode::REPEAT]				= GL_REPEAT;
		
	filterModeMapping[G2Core::FilterMode::NEAREST]					= GL_NEAREST;
	filterModeMapping[G2Core::FilterMode::LINEAR]					= GL_LINEAR;
	filterModeMapping[G2Core::FilterMode::NEAREST_MIPMAP_NEAREST]	= GL_NEAREST_MIPMAP_NEAREST;
	filterModeMapping[G2Core::FilterMode::LINEAR_MIPMAP_NEAREST]	= GL_LINEAR_MIPMAP_NEAREST;
	filterModeMapping[G2Core::FilterMode::NEAREST_MIPMAP_LINEAR]	= GL_NEAREST_MIPMAP_LINEAR;
	filterModeMapping[G2Core::FilterMode::LINEAR_MIPMAP_LINEAR]		= GL_LINEAR_MIPMAP_LINEAR;

	drawModeMapping[G2Core::DrawMode::POINTS]			= GL_POINTS;
	drawModeMapping[G2Core::DrawMode::LINE_STRIP]		= GL_LINE_STRIP;
	drawModeMapping[G2Core::DrawMode::LINE_LOOP]		=	GL_LINE_LOOP;
	drawModeMapping[G2Core::DrawMode::LINES]			= GL_LINES;
	drawModeMapping[G2Core::DrawMode::TRIANGLE_STRIP]	= GL_TRIANGLE_STRIP;
	drawModeMapping[G2Core::DrawMode::TRIANGLE_FAN]		=	GL_TRIANGLE_FAN;
	drawModeMapping[G2Core::DrawMode::TRIANGLES]		= GL_TRIANGLES;
	drawModeMapping[G2Core::DrawMode::QUADS]			=	GL_QUADS;

	polygonDrawModeMapping[G2Core::PolygonDrawMode::FILL] = GL_FILL;
	polygonDrawModeMapping[G2Core::PolygonDrawMode::LINE] = GL_LINE;
	polygonDrawModeMapping[G2Core::PolygonDrawMode::POINT] = GL_POINTS;


	faceCullingModeMapping[G2Core::FaceCulling::BACK_FACE] = GL_BACK;
	faceCullingModeMapping[G2Core::FaceCulling::FRONT_FACE] = GL_FRONT;
	faceCullingModeMapping[G2Core::FaceCulling::FRONT_AND_BACK_FACE] = GL_FRONT_AND_BACK;


	dataTypeMapping[G2Core::DataType::BYTE]				= GL_BYTE;
	dataTypeMapping[G2Core::DataType::UNSIGNED_BYTE]	= GL_UNSIGNED_BYTE;
	dataTypeMapping[G2Core::DataType::UNSIGNED_SHORT]	= GL_UNSIGNED_SHORT;
	dataTypeMapping[G2Core::DataType::SHORT]			= GL_SHORT;
	dataTypeMapping[G2Core::DataType::UNSIGNED_INT]		= GL_UNSIGNED_INT;
	dataTypeMapping[G2Core::DataType::INT]				= GL_INT;
	dataTypeMapping[G2Core::DataType::FLOAT]			= GL_FLOAT;

	blendFuncMapping[G2Core::BlendFactor::CONSTANT_ALPHA];

	blendFuncMapping[G2Core::BlendFactor::ONE]						= GL_ONE; // OpenGL default
	blendFuncMapping[G2Core::BlendFactor::ZERO]						= GL_ZERO;
	blendFuncMapping[G2Core::BlendFactor::SRC_COLOR]				= GL_SRC_COLOR;
	blendFuncMapping[G2Core::BlendFactor::ONE_MINUS_SRC_COLOR]		= GL_ONE_MINUS_SRC_COLOR;
	blendFuncMapping[G2Core::BlendFactor::DST_COLOR]				= GL_DST_COLOR;
	blendFuncMapping[G2Core::BlendFactor::ONE_MINUS_DST_COLOR]		= GL_ONE_MINUS_DST_COLOR;
	blendFuncMapping[G2Core::BlendFactor::SRC_ALPHA]				= GL_SRC_ALPHA;
	blendFuncMapping[G2Core::BlendFactor::ONE_MINUS_SRC_ALPHA]		= GL_ONE_MINUS_SRC_ALPHA;
	blendFuncMapping[G2Core::BlendFactor::DST_ALPHA]				= GL_DST_ALPHA;
	blendFuncMapping[G2Core::BlendFactor::ONE_MINUS_DST_ALPHA]		= GL_ONE_MINUS_DST_ALPHA;
	blendFuncMapping[G2Core::BlendFactor::CONSTANT_COLOR]			= GL_CONSTANT_COLOR;
	blendFuncMapping[G2Core::BlendFactor::ONE_MINUS_CONSTANT_COLOR] = GL_ONE_MINUS_CONSTANT_COLOR;
	blendFuncMapping[G2Core::BlendFactor::CONSTANT_ALPHA]			= GL_CONSTANT_ALPHA;
	blendFuncMapping[G2Core::BlendFactor::ONE_MINUS_CONSTANT_ALPHA] = GL_ONE_MINUS_CONSTANT_ALPHA;
	blendFuncMapping[G2Core::BlendFactor::SRC_ALPHA_SATURATE]		= GL_SRC_ALPHA_SATURATE;
	blendFuncMapping[G2Core::BlendFactor::SRC1_COLOR]				= GL_SRC1_COLOR;
	blendFuncMapping[G2Core::BlendFactor::ONE_MINUS_SRC1_COLOR]		= GL_ONE_MINUS_SRC1_COLOR;
	blendFuncMapping[G2Core::BlendFactor::SRC1_ALPHA]				= GL_SRC1_ALPHA;
	blendFuncMapping[G2Core::BlendFactor::ONE_MINUS_SRC1_ALPHA] = GL_ONE_MINUS_SRC1_ALPHA;

	depthFuncMapping[G2Core::DepthFunc::NEVER] = GL_NEVER; // OpenGL default
	depthFuncMapping[G2Core::DepthFunc::LESS] = GL_LESS;
	depthFuncMapping[G2Core::DepthFunc::EQUAL] = GL_EQUAL;
	depthFuncMapping[G2Core::DepthFunc::LEQUAL] = GL_LEQUAL;
	depthFuncMapping[G2Core::DepthFunc::GREATER] = GL_GREATER;
	depthFuncMapping[G2Core::DepthFunc::NOT_EQUAL] = GL_NOTEQUAL;
	depthFuncMapping[G2Core::DepthFunc::GEQUAL] = GL_GEQUAL;
	depthFuncMapping[G2Core::DepthFunc::ALWAYS] = GL_ALWAYS;
}

unsigned int toGlBufferAccessMode(G2Core::BufferAccessMode::Name mode)
{
	return bufferAccessMapping[mode];
}

unsigned int toGlBaseDataFormat(G2Core::DataFormat::Base::Name dataFormat)
{
	return baseDataFormatMapping[dataFormat];
}

unsigned int toGlInternalDataFormat(G2Core::DataFormat::Internal::Name dataFormat)
{
	return internalDataFormatMapping[dataFormat];
}

unsigned int toGlFrameBufferAttachmentPoint(G2Core::FrameBufferAttachmentPoint::Name attachmentPoint)
{
	return frameBufferAttachmentPointMapping[attachmentPoint];
}

unsigned int toGlTextureFormat(G2Core::TextureFormat::Name textureFormat)
{
	return textureFormatMapping[textureFormat];
}

unsigned int toGlWrapMode(G2Core::WrapMode::Name wrapMode)
{
	return wrapModeMapping[wrapMode];
}

unsigned int toGlFilterMode(G2Core::FilterMode::Name filterMode)
{
	return filterModeMapping[filterMode];
}

unsigned int toGlDrawMode(G2Core::DrawMode::Name drawMode)
{
	return drawModeMapping[drawMode];
}

unsigned int toGlDataType(G2Core::DataType::Name dataType)
{
	return dataTypeMapping[dataType];
}

unsigned int
toGlPolygonDrawMode(G2Core::PolygonDrawMode::Name drawMode)
{
	return polygonDrawModeMapping[drawMode];
}

unsigned int
toGlFaceCullingMode(G2Core::FaceCulling::Name faceCullingMode)
{
	return faceCullingModeMapping[faceCullingMode];
}

unsigned int
toGlBlendFunc(G2Core::BlendFactor::Name blendFunc)
{
	return blendFuncMapping[blendFunc];
}

unsigned int
toGlBufferUsage(G2Core::BufferUsage::Name usage)
{
	return bufferUsageMappingGl[usage];
}

unsigned int
toGlDepthFunc(G2Core::DepthFunc::Name depthFunc)
{
	return depthFuncMapping[depthFunc];
}