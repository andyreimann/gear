#include "DX11GfxApi.h"
#include "DX11GfxData.h"

#include <G2/Logger.h>

#include <unordered_map>

std::unordered_map<G2Core::BufferAccessMode::Name,unsigned int> bufferAccessMapping;
std::unordered_map<G2Core::DataFormat::Name,unsigned int> dataFormatMapping;
std::unordered_map<G2Core::FrameBufferAttachmentPoint::Name,unsigned int> frameBufferAttachmentPointMapping;
std::unordered_map<G2Core::TextureFormat::Name,unsigned int> textureFormatMapping;
std::unordered_map<G2Core::FilterMode::Name,unsigned int> filterModeMapping;
std::unordered_map<G2Core::WrapMode::Name,unsigned int> wrapModeMapping;
std::unordered_map<G2Core::Semantics::Name,LPCSTR> semanticMapping;
std::unordered_map<G2Core::DrawMode::Name,D3D11_PRIMITIVE_TOPOLOGY > drawModeMapping;

void _initMappings()
{
	/*
	bufferAccessMapping[G2Core::BufferAccessMode::READ_ONLY]	= GL_READ_ONLY;
	bufferAccessMapping[G2Core::BufferAccessMode::READ_WRITE]	= GL_READ_WRITE;
	bufferAccessMapping[G2Core::BufferAccessMode::WRITE_ONLY]	= GL_WRITE_ONLY;

	dataFormatMapping[G2Core::DataFormat::RED]					= GL_RED;						//!< Format description: channels: 1; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::RG]					= GL_RG;						//!< Format description: channels: 2; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::RGB]					= GL_RGB;						//!< Format description: channels: 3; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::BGR]					= GL_BGR;						//!< Format description: channels: 3; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::RGB_UB]				= GL_RGB8;						//!< Format description: channels: 3; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::RGB_US]				= GL_RGB16;						//!< Format description: channels: 3; datatype: ushort; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::RGBA]					= GL_RGBA;						//!< Format description: channels: 4; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::BGRA]					= GL_BGRA;						//!< Format description: channels: 3; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::RGBA_UB]				= GL_RGBA8;						//!< Format description: channels: 4; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::RGBA_US]				= GL_RGBA16;					//!< Format description: channels: 4; datatype: ushort; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::RGBA16_F]				= GL_RGBA16F_ARB;				//!< Format description: channels: 4; datatype: half; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::RGBA_F]				= GL_RGBA32F_ARB;				//!< Format description: channels: 4; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::RGBA_B]				= GL_RGBA8I_EXT;				//!< Format description: channels: 4; datatype: byte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::RGBA_S]				= GL_RGBA16I_EXT;				//!< Format description: channels: 4; datatype: short; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::RGBA_I]				= GL_RGBA32I_EXT;				//!< Format description: channels: 4; datatype: int; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::RGBA_UI]				= GL_RGBA32UI_EXT;				//!< Format description: channels: 4; datatype: uint; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::ALPHA_UB]				= GL_ALPHA8;					//!< Format description: channels: 1; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::ALPHA_US]				= GL_ALPHA16;					//!< Format description: channels: 1; datatype: ushort; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::ALPHA16_F]			= GL_ALPHA16F_ARB;				//!< Format description: channels: 1; datatype: half; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::ALPHA_F]				= GL_ALPHA32F_ARB;				//!< Format description: channels: 1; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::ALPHA_B]				= GL_ALPHA8I_EXT;				//!< Format description: channels: 1; datatype: byte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::ALPHA_S]				= GL_ALPHA16I_EXT;				//!< Format description: channels: 1; datatype: short; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::ALPHA_I]				= GL_ALPHA32I_EXT;				//!< Format description: channels: 1; datatype: int; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::ALPHA_UI]				= GL_ALPHA32UI_EXT;				//!< Format description: channels: 1; datatype: uint; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::LUMINANCE]			= GL_LUMINANCE;					//!< Format description: channels: 1; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA_UB]	= GL_LUMINANCE8_ALPHA8;			//!< Format description: channels: 2; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA_US]	= GL_LUMINANCE16_ALPHA16;		//!< Format description: channels: 2; datatype: ushort; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA16_F]	= GL_LUMINANCE_ALPHA16F_ARB;	//!< Format description: channels: 2; datatype: half; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA_F]	= GL_LUMINANCE_ALPHA32F_ARB;	//!< Format description: channels: 2; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA_B]	= GL_LUMINANCE_ALPHA8I_EXT;		//!< Format description: channels: 2; datatype: byte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA_S]	= GL_LUMINANCE_ALPHA16I_EXT;	//!< Format description: channels: 2; datatype: short; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA_I]	= GL_LUMINANCE_ALPHA32I_EXT;	//!< Format description: channels: 2; datatype: int; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_ALPHA_UI]	= GL_LUMINANCE_ALPHA32UI_EXT;	//!< Format description: channels: 2; datatype: uint; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_UB]			= GL_LUMINANCE8;				//!< Format description: channels: 1; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_US ]		= GL_LUMINANCE16;				//!< Format description: channels: 1; datatype: ushort; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::LUMINANCE16_F]		= GL_LUMINANCE16F_ARB;			//!< Format description: channels: 1; datatype: half; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_F]			= GL_LUMINANCE32F_ARB;			//!< Format description: channels: 1; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_B]			= GL_LUMINANCE8I_EXT;			//!< Format description: channels: 1; datatype: byte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_S]			= GL_LUMINANCE16I_EXT;			//!< Format description: channels: 1; datatype: short; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_I]			= GL_LUMINANCE32I_EXT;			//!< Format description: channels: 1; datatype: int; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::LUMINANCE_UI]			= GL_LUMINANCE32UI_EXT;			//!< Format description: channels: 1; datatype: uint; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::INTENSITY	]			= GL_INTENSITY;					//!< Format description: channels: 1; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::INTENSITY_UB]			= GL_INTENSITY8;				//!< Format description: channels: 1; datatype: ubyte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::INTENSITY_US]			= GL_INTENSITY16;				//!< Format description: channels: 1; datatype: ushort; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::INTENSITY16_F]		= GL_INTENSITY16F_ARB;			//!< Format description: channels: 1; datatype: half; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::INTENSITY_F]			= GL_INTENSITY32F_ARB;			//!< Format description: channels: 1; datatype: float; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::INTENSITY_B]			= GL_INTENSITY8I_EXT;			//!< Format description: channels: 1; datatype: byte; bytes per channel: 1
	dataFormatMapping[G2Core::DataFormat::INTENSITY_S]			= GL_INTENSITY16I_EXT;			//!< Format description: channels: 1; datatype: short; bytes per channel: 2
	dataFormatMapping[G2Core::DataFormat::INTENSITY_I]			= GL_INTENSITY32I_EXT;			//!< Format description: channels: 1; datatype: int; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::INTENSITY_UI]			= GL_INTENSITY32UI_EXT;			//!< Format description: channels: 1; datatype: uint; bytes per channel: 4
	dataFormatMapping[G2Core::DataFormat::DEPTH]				= GL_DEPTH_COMPONENT;			//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT - Use GEAR::DEPTH to let your driver choose the best resolution.
	dataFormatMapping[G2Core::DataFormat::DEPTH16]				= GL_DEPTH_COMPONENT16;			//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT16 - only use when your machine supports 32 bit depth maps. Use GEAR::DEPTH to let your driver choose the best resolution.
	dataFormatMapping[G2Core::DataFormat::DEPTH24]				= GL_DEPTH_COMPONENT24;			//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT24 - only use when your machine supports 32 bit depth maps. Use GEAR::DEPTH to let your driver choose the best resolution.
	dataFormatMapping[G2Core::DataFormat::DEPTH32]				= GL_DEPTH_COMPONENT32;			//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT32 - only use when your machine supports 32 bit depth maps. Use GEAR::DEPTH to let your driver choose the best resolution.

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
}

unsigned int toGlBufferAccessMode(G2Core::BufferAccessMode::Name mode)
{
	return bufferAccessMapping[mode];
}

unsigned int toGlDataFormat(G2Core::DataFormat::Name dataFormat)
{
	return dataFormatMapping[dataFormat];
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

LPCSTR toD3DSemanticString(G2Core::Semantics::Name semantic) 
{
	return semanticMapping[semantic];
}

D3D11_PRIMITIVE_TOPOLOGY toD3DDrawMode(G2Core::DrawMode::Name drawMode)
{
	return drawModeMapping[drawMode];
}