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
	}

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
	}
};