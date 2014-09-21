#pragma once
#ifdef COREDLL_EXPORTS
#define COREDLL_API __declspec(dllexport) 
#else
#define COREDLL_API __declspec(dllimport) 
#endif
#include <string>

namespace G2Core
{
	struct GfxResource
	{
		COREDLL_API GfxResource() : valid(true) {}
		bool valid;
	};

	namespace Buffer
	{
		enum Name 
		{
			DEPTH = 1,
			STENCIL = 2,
			COLOR = 4,
		};
	};
	typedef unsigned int BufferFlags;
	
	namespace Semantics 
	{
		enum Name 
		{
			POSITION = 0,
			BLENDWEIGHT,
			NORMAL,
			COLOR_0,
			COLOR_1,
			FOGCOORD,
			PSIZE,
			BLENDINDICES,
			TEXCOORD_0,
			TEXCOORD_1,
			TEXCOORD_2,
			TEXCOORD_3,
			TEXCOORD_4,
			TEXCOORD_5,
			BINORMAL,
			TANGENT,
			NUM_SEMANTICS,
			SEMANTIC_INVALID
		};
		/** This function will parse the given string to the appropriate
		 * Semantic enum value.
		 * @param name The name to parse. Should match the name of the enum.
		 * @return The parsed Semantic enum value.
		 */
		COREDLL_API Semantics::Name parse(std::string const& name);
	};
	
	namespace SemanticDataTypes
	{
		enum Name 
		{
			RGBA_FLOAT = 0,	// 32-bit per component
			RGBA_INT,		// 32-bit per component
			RGB_FLOAT,		// 32-bit per component
			RGB_INT,		// 32-bit per component
			RG_FLOAT,		// 32-bit per component
			RG_INT,			// 32-bit per component
			R_FLOAT,		// 32-bit per component
			R_INT,			// 32-bit per component,
			NUM_SEMANTIC_DATATYPES,
			SEMANTIC_DATATYPE_INVALID
		};
	};

	// TODO This initialization will be removed soon: the GFX Layer implementation for OpenGL will create a mapping instead
	namespace BufferAccessMode 
	{
		enum Name 
		{
			READ_ONLY = 0x88B8,
			WRITE_ONLY = 0x88B9,
			READ_WRITE = 0x88BA,
		};
	};

	namespace DrawMode 
	{
		enum Name 
		{
			POINTS = 0x0000,
			LINE_STRIP = 0x0003,
			LINE_LOOP = 0x0002,
			LINES = 0x0001,
			TRIANGLE_STRIP = 0x0005,
			TRIANGLE_FAN = 0x0006,
			TRIANGLES = 0x0004,
			QUADS = 0x0007,
		};
	};

	namespace DataFormat 
	{
		enum Name 
		{
			UNKNOWN			   ,	//!< Unknown Format
			RED				   ,	//!< Format description: channels: 1, datatype: ubyte, bytes per channel: 1
			RG				   ,	//!< Format description: channels: 2, datatype: ubyte, bytes per channel: 1
			RGB				   ,	//!< Format description: channels: 3, datatype: float, bytes per channel: 4
			BGR				   ,	//!< Format description: channels: 3, datatype: float, bytes per channel: 4
			RGB_UB			   ,	//!< Format description: channels: 3, datatype: ubyte, bytes per channel: 1
			RGB_US			   ,	//!< Format description: channels: 3, datatype: ushort, bytes per channel: 2
							   	
			RGBA			   ,	//!< Format description: channels: 4, datatype: float, bytes per channel: 4
			BGRA			   ,	//!< Format description: channels: 4, datatype: float, bytes per channel: 4
			RGBA_UB			   ,	//!< Format description: channels: 4, datatype: ubyte, bytes per channel: 1
			RGBA_US			   ,	//!< Format description: channels: 4, datatype: ushort, bytes per channel: 2
			RGBA16_F		   ,	//!< Format description: channels: 4, datatype: half, bytes per channel: 2
			RGBA_F			   ,	//!< Format description: channels: 4, datatype: float, bytes per channel: 4
			RGBA_B			   ,	//!< Format description: channels: 4, datatype: byte, bytes per channel: 1
			RGBA_S			   ,	//!< Format description: channels: 4, datatype: short, bytes per channel: 2
			RGBA_I			   ,	//!< Format description: channels: 4, datatype: int, bytes per channel: 4
			RGBA_UI			   ,	//!< Format description: channels: 4, datatype: uint, bytes per channel: 4
							   	
			ALPHA_UB		   ,	//!< Format description: channels: 1, datatype: ubyte, bytes per channel: 1
			ALPHA_US		   ,	//!< Format description: channels: 1, datatype: ushort, bytes per channel: 2
			ALPHA16_F		   ,	//!< Format description: channels: 1, datatype: half, bytes per channel: 2
			ALPHA_F			   ,	//!< Format description: channels: 1, datatype: float, bytes per channel: 4
			ALPHA_B			   ,	//!< Format description: channels: 1, datatype: byte, bytes per channel: 1
			ALPHA_S			   ,	//!< Format description: channels: 1, datatype: short, bytes per channel: 2
			ALPHA_I			   ,	//!< Format description: channels: 1, datatype: int, bytes per channel: 4
			ALPHA_UI		   ,	//!< Format description: channels: 1, datatype: uint, bytes per channel: 4
							   	
			LUMINANCE		   ,	//!< Format description: channels: 1, datatype: float, bytes per channel: 4
			LUMINANCE_ALPHA_UB ,	//!< Format description: channels: 2, datatype: ubyte, bytes per channel: 1
			LUMINANCE_ALPHA_US ,	//!< Format description: channels: 2, datatype: ushort, bytes per channel: 2
			LUMINANCE_ALPHA16_F,	//!< Format description: channels: 2, datatype: half, bytes per channel: 2
			LUMINANCE_ALPHA_F  ,	//!< Format description: channels: 2, datatype: float, bytes per channel: 4
			LUMINANCE_ALPHA_B  ,	//!< Format description: channels: 2, datatype: byte, bytes per channel: 1
			LUMINANCE_ALPHA_S  ,	//!< Format description: channels: 2, datatype: short, bytes per channel: 2
			LUMINANCE_ALPHA_I  ,	//!< Format description: channels: 2, datatype: int, bytes per channel: 4
			LUMINANCE_ALPHA_UI ,	//!< Format description: channels: 2, datatype: uint, bytes per channel: 4
			LUMINANCE_UB	   ,	//!< Format description: channels: 1, datatype: ubyte, bytes per channel: 1
			LUMINANCE_US 	   ,	//!< Format description: channels: 1, datatype: ushort, bytes per channel: 2
			LUMINANCE16_F	   ,	//!< Format description: channels: 1, datatype: half, bytes per channel: 2
			LUMINANCE_F		   ,	//!< Format description: channels: 1, datatype: float, bytes per channel: 4
			LUMINANCE_B		   ,	//!< Format description: channels: 1, datatype: byte, bytes per channel: 1
			LUMINANCE_S		   ,	//!< Format description: channels: 1, datatype: short, bytes per channel: 2
			LUMINANCE_I		   ,	//!< Format description: channels: 1, datatype: int, bytes per channel: 4
			LUMINANCE_UI	   ,	//!< Format description: channels: 1, datatype: uint, bytes per channel: 4
							   	
			INTENSITY		   ,	//!< Format description: channels: 1, datatype: float, bytes per channel: 4
			INTENSITY_UB	   ,	//!< Format description: channels: 1, datatype: ubyte, bytes per channel: 1
			INTENSITY_US	   ,	//!< Format description: channels: 1, datatype: ushort, bytes per channel: 2
			INTENSITY16_F	   ,	//!< Format description: channels: 1, datatype: half, bytes per channel: 2
			INTENSITY_F		   ,	//!< Format description: channels: 1, datatype: float, bytes per channel: 4
			INTENSITY_B		   ,	//!< Format description: channels: 1, datatype: byte, bytes per channel: 1
			INTENSITY_S		   ,	//!< Format description: channels: 1, datatype: short, bytes per channel: 2
			INTENSITY_I		   ,	//!< Format description: channels: 1, datatype: int, bytes per channel: 4
			INTENSITY_UI	   ,	//!< Format description: channels: 1, datatype: uint, bytes per channel: 4
							   	
			DEPTH 			   ,	//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT - Use GEAR::DEPTH to let your driver choose the best resolution.
			DEPTH16			   ,	//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT16 - only use when your machine supports 32 bit depth maps. Use GEAR::DEPTH to let your driver choose the best resolution.
			DEPTH24			   ,	//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT24 - only use when your machine supports 32 bit depth maps. Use GEAR::DEPTH to let your driver choose the best resolution.
			DEPTH32			   ,	//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT32 - only use when your machine supports 32 bit depth maps. Use GEAR::DEPTH to let your driver choose the best resolution.
		};
	};

	namespace TextureFormat 
	{
		enum Name 
		{
			TEXTURE_1D,
			TEXTURE_2D,
			TEXTURE_2D_ARRAY,
			TEXTURE_3D,
			TEXTURE_CUBE_MAP,
			CUBE_MAP_POS_X,
			CUBE_MAP_NEG_X,
			CUBE_MAP_POS_Y,
			CUBE_MAP_NEG_Y,
			CUBE_MAP_POS_Z,
			CUBE_MAP_NEG_Z,
		};
	};

	namespace FrameBufferAttachmentPoint
	{
		enum Name 
		{
			COLOR_0 ,
			COLOR_1 ,
			COLOR_2 ,
			COLOR_3 ,
			COLOR_4 ,
			COLOR_5 ,
			COLOR_6 ,
			COLOR_7 ,
			COLOR_8 ,
			COLOR_9 ,
			COLOR_10,
			COLOR_11,
			COLOR_12,
			COLOR_13,
			COLOR_14,
			COLOR_15,
			DEPTH   ,
			STENCIL ,
			ATTACHMENT_INVALID,
		};
		COREDLL_API FrameBufferAttachmentPoint::Name getByDataFormat(std::string const& outputFormat);
	};

	
	namespace WrapMode
	{
		enum Name {
			CLAMP_TO_EDGE,
			CLAMP_TO_BORDER,
			MIRROR_CLAMP_TO_EDGE,
			MIRRORED_REPEAT,
			REPEAT,
			NUM_WRAP_MODES,
			WRAP_MODE_INVALID,
		};
		/** This function will parse the given string to the appropriate
		 * WrapMode enum value.
		 * @param name The name to parse.
		 * @return The parsed WrapMode enum value.
		 */
		COREDLL_API WrapMode::Name parse(std::string const& name);
	};
	
	namespace FilterMode
	{
		enum Name {
			NEAREST,
			LINEAR,
			NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR,
			NUM_WRAP_MODES,
			WRAP_MODE_INVALID,
		};
	};

	namespace TexSlot
	{
		enum Name {
			TEX_SLOT1 = 0,
			TEX_SLOT2, 
			TEX_SLOT3, 
			TEX_SLOT4, 
			TEX_SLOT5, 
			TEX_SLOT6, 
			TEX_SLOT7, 
			TEX_SLOT8, 
			TEX_SLOT9, 
			TEX_SLOT10,
			TEX_SLOT11,
			TEX_SLOT12,
			TEX_SLOT13,
			TEX_SLOT14,
			TEX_SLOT15,
			TEX_SLOT16,
			TEX_SLOT17,
			TEX_SLOT18,
			TEX_SLOT19,
			TEX_SLOT20,
			TEX_SLOT21,
			TEX_SLOT22,
			TEX_SLOT23,
			TEX_SLOT24,
			TEX_SLOT25,
			TEX_SLOT26,
			TEX_SLOT27,
			TEX_SLOT28,
			TEX_SLOT29,
			TEX_SLOT30,
			TEX_SLOT31,
			TEX_SLOT32,
		};
	};
	
	namespace FaceCulling
	{
		enum Name {
			BACK_FACE = 0x0405,
			FRONT_FACE = 0x0404,
			FRONT_AND_BACK_FACE = 0x0408,
		};
	};
	namespace PolygonDrawMode
	{
		enum Name {
			FILL = 0x1B02,
			LINE = 0x1B01,
			POINT = 0x1B00,
		};
	};
	namespace BlendFactor
	{
		enum Name {
			ONE = 1, // OpenGL default
			ZERO = 0,
			SRC_COLOR = 0x0300,
			ONE_MINUS_SRC_COLOR = 0x0301,
			DST_COLOR = 0x0306,
			ONE_MINUS_DST_COLOR = 0x0307,
			SRC_ALPHA = 0x0302,
			ONE_MINUS_SRC_ALPHA = 0x0303,
			DST_ALPHA = 0x0304,
			ONE_MINUS_DST_ALPHA = 0x0305,
			CONSTANT_COLOR = 0x8001,
			ONE_MINUS_CONSTANT_COLOR = 0x8002,
			CONSTANT_ALPHA = 0x8003,
			ONE_MINUS_CONSTANT_ALPHA = 0x8004,
			SRC_ALPHA_SATURATE = 0x0308,
			SRC1_COLOR = 0x88F9,
			ONE_MINUS_SRC1_COLOR = 0x88FA,
			SRC1_ALPHA = 0x8589,
			ONE_MINUS_SRC1_ALPHA = 0x88FB,
		};
	};
};