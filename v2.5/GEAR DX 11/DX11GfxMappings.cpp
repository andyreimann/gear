#include "DX11GfxApi.h"
#include "DX11GfxData.h"

#include <G2/Logger.h>

#include <unordered_map>

std::unordered_map<G2Core::BufferAccessMode::Name,unsigned int> bufferAccessMapping;
std::unordered_map<G2Core::DataFormat::Name,DXGI_FORMAT> dataFormatMapping;
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
	
	dataFormatMapping[G2Core::DataFormat::RED]					= DXGI_FORMAT_R8_UINT;					//!< Format description: channels: 1; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::RG]					= DXGI_FORMAT_R8G8_UINT;				//!< Format description: channels: 2; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::RGB]					= DXGI_FORMAT_R32G32B32A32_FLOAT;		//!< Format description: channels: 3; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::BGR]					= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 3; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::RGB_UB]				= DXGI_FORMAT_R8G8B8A8_UINT;			//!< Format description: channels: 3; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::RGB_US]				= DXGI_FORMAT_R8G8B8A8_SINT;			//!< Format description: channels: 3; datatype: ushort; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::RGBA]					= DXGI_FORMAT_R32G32B32A32_FLOAT;		//!< Format description: channels: 4; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::BGRA]					= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 3; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::RGBA_UB]				= DXGI_FORMAT_R8G8B8A8_UINT;			//!< Format description: channels: 4; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::RGBA_US]				= DXGI_FORMAT_R8G8B8A8_SINT;			//!< Format description: channels: 4; datatype: ushort; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::RGBA16_F]				= DXGI_FORMAT_R16G16B16A16_FLOAT;		//!< Format description: channels: 4; datatype: half; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::RGBA_F]				= DXGI_FORMAT_R32G32B32A32_FLOAT;		//!< Format description: channels: 4; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::RGBA_B]				= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 4; datatype: byte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::RGBA_S]				= DXGI_FORMAT_R16G16B16A16_SINT;		//!< Format description: channels: 4; datatype: short; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::RGBA_I]				= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 4; datatype: int; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::RGBA_UI]				= DXGI_FORMAT_R32G32B32A32_UINT;		//!< Format description: channels: 4; datatype: uint; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::ALPHA_UB]				= DXGI_FORMAT_A8_UNORM;					//!< Format description: channels: 1; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::ALPHA_US]				= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: ushort; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::ALPHA16_F]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: half; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::ALPHA_F]				= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::ALPHA_B]				= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: byte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::ALPHA_S]				= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: short; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::ALPHA_I]				= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: int; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::ALPHA_UI]				= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: uint; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::LUMINANCE]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA_UB]	= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 2; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA_US]	= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 2; datatype: ushort; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA16_F]	= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 2; datatype: half; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA_F]	= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 2; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA_B]	= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 2; datatype: byte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA_S]	= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 2; datatype: short; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA_I]	= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 2; datatype: int; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA_UI]	= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 2; datatype: uint; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_UB]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_US]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: ushort; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::LUMINANCE16_F]		= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: half; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_F]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_B]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: byte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_S]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: short; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_I]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: int; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_UI]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: uint; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::INTENSITY]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::INTENSITY_UB]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::INTENSITY_US]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: ushort; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::INTENSITY16_F]		= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: half; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::INTENSITY_F]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::INTENSITY_B]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: byte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::INTENSITY_S]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: short; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::INTENSITY_I]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: int; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::INTENSITY_UI]			= DXGI_FORMAT_UNKNOWN;					//!< Format description: channels: 1; datatype: uint; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::DEPTH]				= DXGI_FORMAT_D32_FLOAT;				//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT - Use GEAR::DEPTH to let your driver choose the best resolution.
	dataFormatMapping[G2Core::DataFormat::DEPTH16]				= DXGI_FORMAT_D16_UNORM;				//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT16 - only use when your machine supports 32 bit depth maps. Use GEAR::DEPTH to let your driver choose the best resolution.
	dataFormatMapping[G2Core::DataFormat::DEPTH24]				= DXGI_FORMAT_D24_UNORM_S8_UINT;		//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT24 - only use when your machine supports 32 bit depth maps. Use GEAR::DEPTH to let your driver choose the best resolution.
	dataFormatMapping[G2Core::DataFormat::DEPTH32]				= DXGI_FORMAT_D32_FLOAT;				//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT32 - only use when your machine supports 32 bit depth maps. Use GEAR::DEPTH to let your driver choose the best resolution.
	

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

unsigned int toD3DDataFormat(G2Core::DataFormat::Name dataFormat)
{
	return dataFormatMapping[dataFormat];
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

DXGI_FORMAT toD3DFormat(G2Core::DataFormat::Name format)
{
	return dataFormatMapping[format];
}

int
toD3DTexSlot(G2Core::TexSlot::Name texSlot)
{
	return texSlotMapping[texSlot];
}