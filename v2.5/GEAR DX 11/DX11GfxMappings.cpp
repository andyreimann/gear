#include "DX11GfxApi.h"
#include "DX11GfxData.h"

#include <G2/Logger.h>

#include <unordered_map>

std::unordered_map<G2Core::BufferAccessMode::Name,unsigned int> bufferAccessMapping;
std::unordered_map<G2Core::DataFormat::Internal::Name,DXGI_FORMAT> internalDataFormatMapping;
std::unordered_map<G2Core::FrameBufferAttachmentPoint::Name,unsigned int> frameBufferAttachmentPointMapping;
std::unordered_map<G2Core::TextureFormat::Name,unsigned int> textureFormatMapping;
std::unordered_map<G2Core::FilterMode::Name,unsigned int> filterModeMapping;
std::unordered_map<G2Core::WrapMode::Name, D3D11_TEXTURE_ADDRESS_MODE> wrapModeMapping;
std::unordered_map<G2Core::Semantics::Name, LPCSTR> semanticMapping;
std::unordered_map<G2Core::DrawMode::Name, D3D11_PRIMITIVE_TOPOLOGY > drawModeMapping;
std::unordered_map<G2Core::TexSlot::Name, int > texSlotMapping;

void _initMappings()
{
	
	bufferAccessMapping[G2Core::BufferAccessMode::READ_ONLY]	= D3D11_CPU_ACCESS_READ;
	bufferAccessMapping[G2Core::BufferAccessMode::READ_WRITE]	= D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	bufferAccessMapping[G2Core::BufferAccessMode::WRITE_ONLY]	= D3D11_CPU_ACCESS_WRITE;

	internalDataFormatMapping[G2Core::DataFormat::Internal::R8_UI] = DXGI_FORMAT_R8_UINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R8_I] = DXGI_FORMAT_R8_SINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R16_UI] = DXGI_FORMAT_R16_UINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R16_I] = DXGI_FORMAT_R16_SINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R16_F] = DXGI_FORMAT_R16_FLOAT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32_UI] = DXGI_FORMAT_R32_UINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32_I] = DXGI_FORMAT_R32_SINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32_F] = DXGI_FORMAT_R32_FLOAT;
	// -------------------------------------------------------------------------
	internalDataFormatMapping[G2Core::DataFormat::Internal::R8G8_UI] = DXGI_FORMAT_R8G8_UINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R8G8_I] = DXGI_FORMAT_R8G8_SINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R16G16_UI] = DXGI_FORMAT_R16G16_UINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R16G16_I] = DXGI_FORMAT_R16G16_SINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R16G16_F] = DXGI_FORMAT_R16G16_FLOAT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32_UI] = DXGI_FORMAT_R32G32_UINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32_I] = DXGI_FORMAT_R32G32_SINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32_F] = DXGI_FORMAT_R32G32_FLOAT;
	// -------------------------------------------------------------------------
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32B32_UI] = DXGI_FORMAT_R32G32B32_UINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32B32_I] = DXGI_FORMAT_R32G32B32_SINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32B32_F] = DXGI_FORMAT_R32G32B32_FLOAT;
	// -------------------------------------------------------------------------
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32B32A32_UI] = DXGI_FORMAT_R32G32B32A32_UINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32B32A32_I] = DXGI_FORMAT_R32G32B32A32_SINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::R32G32B32A32_F] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	// -------------------------------------------------------------------------
	internalDataFormatMapping[G2Core::DataFormat::Internal::D16_F] = DXGI_FORMAT_D16_UNORM;
	internalDataFormatMapping[G2Core::DataFormat::Internal::D24_F] = DXGI_FORMAT_D24_UNORM_S8_UINT;
	internalDataFormatMapping[G2Core::DataFormat::Internal::D32_F] = DXGI_FORMAT_D32_FLOAT;

	wrapModeMapping[G2Core::WrapMode::CLAMP_TO_EDGE] = D3D11_TEXTURE_ADDRESS_CLAMP;
	wrapModeMapping[G2Core::WrapMode::CLAMP_TO_BORDER] = D3D11_TEXTURE_ADDRESS_BORDER;
	wrapModeMapping[G2Core::WrapMode::MIRROR_CLAMP_TO_EDGE] = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	wrapModeMapping[G2Core::WrapMode::MIRRORED_REPEAT] = D3D11_TEXTURE_ADDRESS_MIRROR;
	wrapModeMapping[G2Core::WrapMode::REPEAT] = D3D11_TEXTURE_ADDRESS_WRAP;
	
	/*
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

		
	filterModeMapping[G2Core::FilterMode::NEAREST]					= GL_NEAREST;
	filterModeMapping[G2Core::FilterMode::LINEAR]					= GL_LINEAR;
	filterModeMapping[G2Core::FilterMode::NEAREST_MIPMAP_NEAREST]	= GL_NEAREST_MIPMAP_NEAREST;
	filterModeMapping[G2Core::FilterMode::LINEAR_MIPMAP_NEAREST]	= GL_LINEAR_MIPMAP_NEAREST;
	filterModeMapping[G2Core::FilterMode::NEAREST_MIPMAP_LINEAR]	= GL_NEAREST_MIPMAP_LINEAR;
	filterModeMapping[G2Core::FilterMode::LINEAR_MIPMAP_LINEAR]		= GL_LINEAR_MIPMAP_LINEAR;
	*/
		
	semanticMapping[G2Core::Semantics::POSITION] = "POSITION";
	semanticMapping[G2Core::Semantics::BLENDWEIGHT] = "BLENDWEIGHT";
	semanticMapping[G2Core::Semantics::NORMAL] = "NORMAL";
	semanticMapping[G2Core::Semantics::COLOR_0] = "COLOR0";
	semanticMapping[G2Core::Semantics::COLOR_1] = "COLOR1";
	semanticMapping[G2Core::Semantics::FOGCOORD] = "FOG";
	semanticMapping[G2Core::Semantics::PSIZE] = "PSIZE";
	semanticMapping[G2Core::Semantics::BLENDINDICES] = "BLENDINDICES";
	semanticMapping[G2Core::Semantics::TEXCOORD_0] = "TEXCOORD0";
	semanticMapping[G2Core::Semantics::TEXCOORD_1] = "TEXCOORD1";
	semanticMapping[G2Core::Semantics::TEXCOORD_2] = "TEXCOORD2";
	semanticMapping[G2Core::Semantics::TEXCOORD_3] = "TEXCOORD3";
	semanticMapping[G2Core::Semantics::TEXCOORD_4] = "TEXCOORD4";
	semanticMapping[G2Core::Semantics::TEXCOORD_5] = "TEXCOORD5";
	semanticMapping[G2Core::Semantics::TANGENT] = "TANGENT";
	semanticMapping[G2Core::Semantics::BINORMAL] = "BINORMAL";

	drawModeMapping[G2Core::DrawMode::POINTS]			= D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	drawModeMapping[G2Core::DrawMode::LINE_STRIP]		= D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	drawModeMapping[G2Core::DrawMode::LINE_LOOP]		=	D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	drawModeMapping[G2Core::DrawMode::LINES]			= D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	drawModeMapping[G2Core::DrawMode::TRIANGLE_STRIP]	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	drawModeMapping[G2Core::DrawMode::TRIANGLE_FAN]		=	D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	drawModeMapping[G2Core::DrawMode::TRIANGLES]		= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	drawModeMapping[G2Core::DrawMode::QUADS]			=	D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;

	texSlotMapping[G2Core::TexSlot::TEX_SLOT1] = 0;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT2] = 1;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT3] = 2;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT4] = 3;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT5] = 4;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT6] = 5;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT7] = 6;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT8] = 7;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT9] = 8;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT10] = 9;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT11] = 10;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT12] = 11;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT13] = 12;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT14] = 13;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT15] = 14;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT16] = 15;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT17] = 16;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT18] = 17;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT19] = 18;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT20] = 19;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT21] = 20;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT22] = 21;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT23] = 22;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT24] = 23;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT25] = 24;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT26] = 25;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT27] = 26;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT28] = 27;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT29] = 28;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT30] = 29;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT31] = 30;
	texSlotMapping[G2Core::TexSlot::TEX_SLOT32] = 31;
}

unsigned int toD3DBufferAccessMode(G2Core::BufferAccessMode::Name mode)
{
	return bufferAccessMapping[mode];
}

DXGI_FORMAT toD3DInternalDataFormat(G2Core::DataFormat::Internal::Name dataFormat)
{
	return internalDataFormatMapping[dataFormat];
}

unsigned int toD3DFrameBufferAttachmentPoint(G2Core::FrameBufferAttachmentPoint::Name attachmentPoint)
{
	return frameBufferAttachmentPointMapping[attachmentPoint];
}

unsigned int toD3DTextureFormat(G2Core::TextureFormat::Name textureFormat)
{
	return textureFormatMapping[textureFormat];
}

D3D11_TEXTURE_ADDRESS_MODE toD3DWrapMode(G2Core::WrapMode::Name wrapMode)
{
	return wrapModeMapping[wrapMode];
}

unsigned int toD3DFilterMode(G2Core::FilterMode::Name filterMode)
{
	return filterModeMapping[filterMode];
}

LPCSTR toD3DSemanticString(G2Core::Semantics::Name semantic) 
{
	return semanticMapping[semantic];
}

D3D11_PRIMITIVE_TOPOLOGY toD3DDrawMode(G2Core::DrawMode::Name drawMode)
{
	return drawModeMapping[drawMode];
}

int
toD3DTexSlot(G2Core::TexSlot::Name texSlot)
{
	return texSlotMapping[texSlot];
}