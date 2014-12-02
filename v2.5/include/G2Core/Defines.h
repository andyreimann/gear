#pragma once
#ifdef COREDLL_EXPORTS
#define COREDLL_API __declspec(dllexport) 
#else
#define COREDLL_API __declspec(dllimport) 
#endif
#include <string>

namespace G2Core
{

	namespace Flags
	{
		enum Name
		{
			NO_FLAGS = 0,
			ALL_FLAGS = -1,
		};
	}

	struct GfxResource
	{
		COREDLL_API GfxResource() : valid(true) {}
		COREDLL_API virtual ~GfxResource() {}

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
			READ_ONLY,
			WRITE_ONLY,
			READ_WRITE,
		};
	};

	namespace DrawMode 
	{
		enum Name 
		{
			POINTS,
			LINE_STRIP,
			LINE_LOOP,
			LINES,
			TRIANGLE_STRIP,
			TRIANGLE_FAN,
			TRIANGLES,
			QUADS,
		};
	};

	namespace DataType
	{
		enum Name
		{
			BYTE = 0,	
			UNSIGNED_BYTE,	
			UNSIGNED_SHORT,		
			SHORT,		
			UNSIGNED_INT,		
			INT,			
			FLOAT,
		};
	};

	namespace DataFormat 
	{

		namespace Base
		{
			enum Name
			{
				UNKNOWN,
				RED,
				RG,
				RGB,
				RGBA,
				LUMINANCE,
				DEPTH,
			};
		};

		namespace Internal /* sized data formats */
		{
			enum Name
			{
				UNKNOWN,
				// _UI		-> unsigned integer
				// _I		-> integer
				// _F		-> float

				R8_UI,				// GL: GL_RED				DX: DXGI_FORMAT_R8_UINT
				R8_I,				// GL: GL_RED				DX: DXGI_FORMAT_R8_SINT
				R16_UI,				// GL: GL_RED				DX: DXGI_FORMAT_R16_UINT
				R16_I,				// GL: GL_RED				DX: DXGI_FORMAT_R16_SINT
				R16_F,				// GL: GL_R16F				DX: DXGI_FORMAT_R16_FLOAT
				R32_UI,				// GL: GL_RED				DX: DXGI_FORMAT_R32_UINT	
				R32_I,				// GL: GL_RED				DX: DXGI_FORMAT_R32_SINT	
				R32_F,				// GL: GL_R32F				DX: DXGI_FORMAT_R32_FLOAT

				R8G8_UI,			// GL: GL_RG				DX: DXGI_FORMAT_R8G8_UINT
				R8G8_I,				// GL: GL_RG				DX: DXGI_FORMAT_R8G8_SINT
				R16G16_UI,			// GL: GL_RG				DX: DXGI_FORMAT_R16G16_UINT
				R16G16_I,			// GL: GL_RG				DX: DXGI_FORMAT_R16G16_SINT
				R16G16_F,			// GL: GL_R16F				DX: DXGI_FORMAT_R16G16_FLOAT
				R32G32_UI,			// GL: GL_RG				DX: DXGI_FORMAT_R32G32_UINT	
				R32G32_I,			// GL: GL_RG				DX: DXGI_FORMAT_R32G32_SINT	
				R32G32_F,			// GL: GL_R32F				DX: DXGI_FORMAT_R32G32_FLOAT

				R32G32B32_UI,		// GL: GL_RGB			DX: DXGI_FORMAT_R32G32B32_UINT	
				R32G32B32_I,		// GL: GL_RGB			DX: DXGI_FORMAT_R32G32B32_SINT	
				R32G32B32_F,		// GL: GL_RGB32F			DX: DXGI_FORMAT_R32G32B32_FLOAT

				R32G32B32A32_UI,	// GL: GL_RGBA			DX: DXGI_FORMAT_R32G32B32A32_UINT	
				R32G32B32A32_I,		// GL: GL_RGBA			DX: DXGI_FORMAT_R32G32B32A32_SINT	
				R32G32B32A32_F,		// GL: GL_RGBA32F			DX: DXGI_FORMAT_R32G32B32A32_FLOAT

				D16_F,				// GL: GL_DEPTH_COMPONENT16 DX: DXGI_FORMAT_D16_UNORM
				D24_F,				// GL: GL_DEPTH_COMPONENT24 DX: DXGI_FORMAT_D24_UNORM_S8_UINT ???
				D32_F,				// GL: GL_DEPTH_COMPONENT32 DX: DXGI_FORMAT_D32_FLOAT

				LUMINANCE_F,		// GL: GL_LUMINANCE DX: NOT_SUPPORTED
			};
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
		/** This function will parse the given string to the appropriate
		* FrameBufferAttachmentPoint enum value.
		* @param outputFormat The name to parse. 
		* @note Strings like 'COLOR' and 'RGB' will additionally be parsed to the enum value 'COLOR_0', 'COLOR_0', 
		* @return The parsed RenderLayerFrameBufferAttachmentPoint enum value.
		*/
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
			BACK_FACE,
			FRONT_FACE,
			FRONT_AND_BACK_FACE,
		};
	};
	namespace PolygonDrawMode
	{
		enum Name {
			FILL,
			LINE,
			POINT,
		};
	};
	namespace BlendFactor
	{
		enum Name {
			ONE,
			ZERO,
			SRC_COLOR,
			ONE_MINUS_SRC_COLOR,
			DST_COLOR,
			ONE_MINUS_DST_COLOR,
			SRC_ALPHA,
			ONE_MINUS_SRC_ALPHA,
			DST_ALPHA,
			ONE_MINUS_DST_ALPHA,
			CONSTANT_COLOR,
			ONE_MINUS_CONSTANT_COLOR,
			CONSTANT_ALPHA,
			ONE_MINUS_CONSTANT_ALPHA,
			SRC_ALPHA_SATURATE,
			SRC1_COLOR,
			ONE_MINUS_SRC1_COLOR,
			SRC1_ALPHA,
			ONE_MINUS_SRC1_ALPHA,
		};
	};

	namespace RenderLayer
	{
		typedef int RenderLayerMask;


		enum Name {
			TERRAIN = 1,
			WATER = 2,
			TRANSPARENT_MESH = 4,
			STATIC_MESH = 8,
			DYNAMIC_MESH = 16,
			LAYER_5 = 32,
			LAYER_6 = 64,
			LAYER_7 = 128,
			LAYER_8 = 256,
			LAYER_9 = 512,
			LAYER_10 = 1024,
			LAYER_11 = 2048,
			LAYER_12 = 4096,
			LAYER_13 = 8192,
			LAYER_14 = 16384,
			LAYER_15 = 32768,
			LAYER_16 = 65536,
			LAYER_17 = 131072,
			LAYER_18 = 262144,
			LAYER_19 = 524288,
			LAYER_20 = 1048576,
			LAYER_21 = 2097152,
			LAYER_22 = 4194304,
			LAYER_23 = 8388608,
			LAYER_24 = 16777216,
			LAYER_25 = 33554432,
			LAYER_26 = 67108864,
			LAYER_27 = 134217728,
			LAYER_28 = 268435456,
			LAYER_29 = 536870912,
			LAYER_30 = 1073741824,
		};
		/** This function will parse the given string to the appropriate
		* RenderLayer enum value.
		* @param renderLayer The name to parse.
		* @return The parsed RenderLayer enum value or 0 if no valid RenderLayer enum value was detected.
		*/
		COREDLL_API RenderLayer::RenderLayerMask getByRenderLayer(std::string const& renderLayer);
	};
};