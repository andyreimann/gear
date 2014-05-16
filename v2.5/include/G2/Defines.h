// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include <G2Core/KeyCodes.h>

#include <GL/glew.h>
#include <sstream>
#include <iostream>
#include <Windows.h>
#include <float.h>


// PLATFORM DETECTION //////////////////////////////////////////////////////////////
#ifdef _WIN64
	#define G2_PLATFORM_WIN64
	#define G2_PLATFORM_WIN
#elif _WIN32
	#define G2_PLATFORM_WIN32
	#define G2_PLATFORM_WIN
#elif __APPLE__
	#include "TargetConditionals.h"
	#if TARGET_OS_IPHONE    
		 #define G2_PLATFORM_APPLE
		 #define G2_PLATFORM_IOS
		 #define G2_PLATFORM_IOS_DEVICE
	#elif TARGET_IPHONE_SIMULATOR
		 #define G2_PLATFORM_APPLE
		 #define G2_PLATFORM_IOS
		 #define G2_PLATFORM_IOS_SIMULATOR
	#elif TARGET_OS_MAC
		 #define G2_PLATFORM_APPLE
		 #define G2_PLATFORM_MAC
	#else
		 #define G2_PLATFORM_UNSUPPORTED
	#endif
#elif __linux
	#define G2_PLATFORM_UNIX
#elif __unix // all unixes not caught above
	#define G2_PLATFORM_UNIX
#elif __posix
	#define G2_PLATFORM_POSIX
#endif
// PLATFORM DETECTION END ///////////////////////////////////////////////////////////


// COMPILER DETECTION //////////////////////////////////////////////////////////////
#ifdef __GNUC__
	#define G2_COMPILER_GCC
#elif defined(__INTEL_COMPILER) || \
	  defined(__ICC) || \
	  defined(__ECC)
	#define G2_COMPILER_INTEL
#elif __llvm__
	#define G2_COMPILER_LLVM
#elif _MSC_VER
	#define G2_COMPILER_MSVC
#elif __CC_ARM
	#define G2_COMPILER_ARM
#endif
// more see http://sourceforge.net/p/predef/wiki/Compilers/
// COMPILER DETECTION END ///////////////////////////////////////////////////////////

#define GLDEBUG(x) \
x; \
{ \
	GLenum e = glGetError(); \
	if( e != GL_NO_ERROR) \
	{ \
		std::cout << "Error at line number " << __LINE__ << ", in file " << __FILE__ << ". glGetError() returned " << gluErrorString(e) << " for call " << #x << "\n"; \
	} \
}

#include <assert.h>
#define MESSAGE_ASSERT(m, e) assert((m && e ))

#define BUFFER_OFFSET(i) ((char *)NULL+(i)) //!< Hint to simply set the offset i of a generic opengl buffer

namespace G2 
{
	const float EPSILON = FLT_MIN;
	const float MAX_FLOAT = FLT_MAX;
	const float MAX_NEG_FLOAT = -FLT_MAX;

	// THE DIFFERENT SIZE TYPES
	//const unsigned BYTE = 0; // already used as a data format (see few lines above), but can be used too.
	const unsigned KILOBYTE = 1;
	const unsigned MEGABYTE = 2;
	const unsigned GIGABYTE = 3;

//deprecated attribut
#ifdef G2_PLATFORM_WIN
	#define G2_DEPRECATED __declspec(deprecated)
#elif def G2_PLATFORM_APPLE
	#define G2_DEPRECATED __attribute__ ((deprecated))
#else 
	#define G2_DEPRECATED 
#endif

	// fundamental type definitions
	typedef unsigned char		Byte;
	typedef signed char			Int8;
	typedef signed short		Int16;
	typedef signed int			Int32;
	typedef signed long         Int64;
	typedef unsigned char		UInt8;
	typedef unsigned short		UInt16;
	typedef unsigned int		UInt32;
	typedef unsigned long       UInt64;
	typedef Int32				Bool;
	typedef float				Float32;
	typedef double				Float64;
	
	const Bool G2_TRUE	= 1;
	const Bool G2_FALSE	= 0;

	namespace BufferAccessMode {
		const UInt32 READ_ONLY = GL_READ_ONLY;
		const UInt32 WRITE_ONLY = GL_WRITE_ONLY;
		const UInt32 READ_WRITE = GL_READ_WRITE;
	};

	const unsigned RGB          = GL_RGB;								//!< Format description: channels: 3, datatype: float, bytes per channel: 4
	const unsigned RGB_UB       = GL_RGB8;								//!< Format description: channels: 3, datatype: ubyte, bytes per channel: 1
	const unsigned RGB_US       = GL_RGB16;								//!< Format description: channels: 3, datatype: ushort, bytes per channel: 2

	const unsigned RGBA         = GL_RGBA;								//!< Format description: channels: 4, datatype: float, bytes per channel: 4
	const unsigned RGBA_UB      = GL_RGBA8;								//!< Format description: channels: 4, datatype: ubyte, bytes per channel: 1
	const unsigned RGBA_US      = GL_RGBA16;							//!< Format description: channels: 4, datatype: ushort, bytes per channel: 2
	const unsigned RGBA16_F     = GL_RGBA16F_ARB;						//!< Format description: channels: 4, datatype: half, bytes per channel: 2
	const unsigned RGBA_F       = GL_RGBA32F_ARB;						//!< Format description: channels: 4, datatype: float, bytes per channel: 4
	const unsigned RGBA_B       = GL_RGBA8I_EXT;						//!< Format description: channels: 4, datatype: byte, bytes per channel: 1
	const unsigned RGBA_S       = GL_RGBA16I_EXT;						//!< Format description: channels: 4, datatype: short, bytes per channel: 2
	const unsigned RGBA_I       = GL_RGBA32I_EXT;						//!< Format description: channels: 4, datatype: int, bytes per channel: 4
	const unsigned RGBA_UI      = GL_RGBA32UI_EXT;						//!< Format description: channels: 4, datatype: uint, bytes per channel: 4

	const unsigned ALPHA_UB   = GL_ALPHA8;								//!< Format description: channels: 1, datatype: ubyte, bytes per channel: 1
	const unsigned ALPHA_US   = GL_ALPHA16;								//!< Format description: channels: 1, datatype: ushort, bytes per channel: 2
	const unsigned ALPHA16_F  = GL_ALPHA16F_ARB;						//!< Format description: channels: 1, datatype: half, bytes per channel: 2
	const unsigned ALPHA_F    = GL_ALPHA32F_ARB;						//!< Format description: channels: 1, datatype: float, bytes per channel: 4
	const unsigned ALPHA_B    = GL_ALPHA8I_EXT;							//!< Format description: channels: 1, datatype: byte, bytes per channel: 1
	const unsigned ALPHA_S    = GL_ALPHA16I_EXT;						//!< Format description: channels: 1, datatype: short, bytes per channel: 2
	const unsigned ALPHA_I    = GL_ALPHA32I_EXT;						//!< Format description: channels: 1, datatype: int, bytes per channel: 4
	const unsigned ALPHA_UI   = GL_ALPHA32UI_EXT;						//!< Format description: channels: 1, datatype: uint, bytes per channel: 4

	const unsigned LUMINANCE			= GL_LUMINANCE;					//!< Format description: channels: 1, datatype: float, bytes per channel: 4
	const unsigned LUMINANCE_ALPHA_UB   = GL_LUMINANCE8_ALPHA8;			//!< Format description: channels: 2, datatype: ubyte, bytes per channel: 1
	const unsigned LUMINANCE_ALPHA_US   = GL_LUMINANCE16_ALPHA16;		//!< Format description: channels: 2, datatype: ushort, bytes per channel: 2
	const unsigned LUMINANCE_ALPHA16_F  = GL_LUMINANCE_ALPHA16F_ARB;	//!< Format description: channels: 2, datatype: half, bytes per channel: 2
	const unsigned LUMINANCE_ALPHA_F    = GL_LUMINANCE_ALPHA32F_ARB;	//!< Format description: channels: 2, datatype: float, bytes per channel: 4
	const unsigned LUMINANCE_ALPHA_B    = GL_LUMINANCE_ALPHA8I_EXT;		//!< Format description: channels: 2, datatype: byte, bytes per channel: 1
	const unsigned LUMINANCE_ALPHA_S    = GL_LUMINANCE_ALPHA16I_EXT;	//!< Format description: channels: 2, datatype: short, bytes per channel: 2
	const unsigned LUMINANCE_ALPHA_I    = GL_LUMINANCE_ALPHA32I_EXT;	//!< Format description: channels: 2, datatype: int, bytes per channel: 4
	const unsigned LUMINANCE_ALPHA_UI   = GL_LUMINANCE_ALPHA32UI_EXT;	//!< Format description: channels: 2, datatype: uint, bytes per channel: 4
	const unsigned LUMINANCE_UB			= GL_LUMINANCE8;				//!< Format description: channels: 1, datatype: ubyte, bytes per channel: 1
	const unsigned LUMINANCE_US 		= GL_LUMINANCE16;				//!< Format description: channels: 1, datatype: ushort, bytes per channel: 2
	const unsigned LUMINANCE16_F		= GL_LUMINANCE16F_ARB;			//!< Format description: channels: 1, datatype: half, bytes per channel: 2
	const unsigned LUMINANCE_F			= GL_LUMINANCE32F_ARB;			//!< Format description: channels: 1, datatype: float, bytes per channel: 4
	const unsigned LUMINANCE_B			= GL_LUMINANCE8I_EXT;			//!< Format description: channels: 1, datatype: byte, bytes per channel: 1
	const unsigned LUMINANCE_S			= GL_LUMINANCE16I_EXT;			//!< Format description: channels: 1, datatype: short, bytes per channel: 2
	const unsigned LUMINANCE_I			= GL_LUMINANCE32I_EXT;			//!< Format description: channels: 1, datatype: int, bytes per channel: 4
	const unsigned LUMINANCE_UI			= GL_LUMINANCE32UI_EXT;			//!< Format description: channels: 1, datatype: uint, bytes per channel: 4

	const unsigned INTENSITY		= GL_INTENSITY;						//!< Format description: channels: 1, datatype: float, bytes per channel: 4
	const unsigned INTENSITY_UB		= GL_INTENSITY8;					//!< Format description: channels: 1, datatype: ubyte, bytes per channel: 1
	const unsigned INTENSITY_US		= GL_INTENSITY16;					//!< Format description: channels: 1, datatype: ushort, bytes per channel: 2
	const unsigned INTENSITY16_F    = GL_INTENSITY16F_ARB;				//!< Format description: channels: 1, datatype: half, bytes per channel: 2
	const unsigned INTENSITY_F      = GL_INTENSITY32F_ARB;				//!< Format description: channels: 1, datatype: float, bytes per channel: 4
	const unsigned INTENSITY_B		= GL_INTENSITY8I_EXT;				//!< Format description: channels: 1, datatype: byte, bytes per channel: 1
	const unsigned INTENSITY_S		= GL_INTENSITY16I_EXT;				//!< Format description: channels: 1, datatype: short, bytes per channel: 2
	const unsigned INTENSITY_I		= GL_INTENSITY32I_EXT;				//!< Format description: channels: 1, datatype: int, bytes per channel: 4
	const unsigned INTENSITY_UI		= GL_INTENSITY32UI_EXT;				//!< Format description: channels: 1, datatype: uint, bytes per channel: 4

	const unsigned DEPTH        = GL_DEPTH_COMPONENT;					//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT - Use GEAR::DEPTH to let your driver choose the best resolution.
	const unsigned DEPTH16      = GL_DEPTH_COMPONENT16;					//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT16 - only use when your machine supports 32 bit depth maps. Use GEAR::DEPTH to let your driver choose the best resolution.
	const unsigned DEPTH24      = GL_DEPTH_COMPONENT24;					//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT24 - only use when your machine supports 32 bit depth maps. Use GEAR::DEPTH to let your driver choose the best resolution.
	const unsigned DEPTH32      = GL_DEPTH_COMPONENT32;					//!< only wrapper constant for opengl variable GL_DEPTH_COMPONENT32 - only use when your machine supports 32 bit depth maps. Use GEAR::DEPTH to let your driver choose the best resolution.
	
	// different mip map levels
	const unsigned NEAREST						= GL_NEAREST;					//!< only wrapper constant for opengl constant to be name consistent
	const unsigned LINEAR						= GL_LINEAR;					//!< only wrapper constant for opengl constant to be name consistent
	const unsigned NEAREST_MIPMAP_NEAREST		= GL_NEAREST_MIPMAP_NEAREST;	//!< only wrapper constant for opengl constant to be name consistent
	const unsigned LINEAR_MIPMAP_NEAREST		= GL_LINEAR_MIPMAP_NEAREST;		//!< only wrapper constant for opengl constant to be name consistent
	const unsigned NEAREST_MIPMAP_LINEAR		= GL_NEAREST_MIPMAP_LINEAR;		//!< only wrapper constant for opengl constant to be name consistent
	const unsigned LINEAR_MIPMAP_LINEAR			= GL_LINEAR_MIPMAP_LINEAR;		//!< only wrapper constant for opengl constant to be name consistent

	// THE DIFFERENT TEXTURE SLOTS (regardless whether they are available on a machine or not)
	const int TEX_SLOT1 = GL_TEXTURE0; //!< This is the global slot for the first texture.
	const int TEX_SLOT_DIFFUSE_MAP = TEX_SLOT1; 
	const int TEX_SLOT2 = GL_TEXTURE1; //!< This is the global slot for the second texture.
	const int TEX_SLOT_NORMAL_MAP = TEX_SLOT2; 
	const int TEX_SLOT3 = GL_TEXTURE2; //!< This is the global slot for the first texture.
	const int TEX_SLOT_ALPHA_MAP = TEX_SLOT3; 
	const int TEX_SLOT4 = GL_TEXTURE3; //!< This is the global slot for the second texture.
	const int TEX_SLOT5 = GL_TEXTURE4; //!< This is the global slot for the first texture.
	const int TEX_SLOT6 = GL_TEXTURE5; //!< This is the global slot for the second texture.
	const int TEX_SLOT7 = GL_TEXTURE6; //!< This is the global slot for the third exture.
	const int TEX_SLOT8 = GL_TEXTURE7; //!< This is the global slot for the fourth texture.
	const int TEX_SLOT9 = GL_TEXTURE7+1; //!< This is the global slot for the additional special texture 1. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT10 = GL_TEXTURE7+2; //!< This is the global slot for the additional special texture 2. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT11 = GL_TEXTURE7+3; //!< This is the global slot for the additional special texture 3. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT12 = GL_TEXTURE7+4; //!< This is the global slot for the additional special texture 4. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT13 = GL_TEXTURE7+5; //!< This is the global slot for the additional special texture 5. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT14 = GL_TEXTURE7+6; //!< This is the global slot for the additional special texture 6. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT15 = GL_TEXTURE7+7; //!< This is the global slot for the additional special texture 7. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT16 = GL_TEXTURE7+8; //!< This is the global slot for the additional special texture 8. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT17 = GL_TEXTURE7+9; //!< This is the global slot for the additional special texture 9. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT18 = GL_TEXTURE7+10; //!< This is the global slot for the additional special texture 10. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT19 = GL_TEXTURE7+11; //!< This is the global slot for the additional special texture 11. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT20 = GL_TEXTURE7+12; //!< This is the global slot for the additional special texture 12. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT21 = GL_TEXTURE7+13; //!< This is the global slot for the additional special texture 13. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT22 = GL_TEXTURE7+14; //!< This is the global slot for the additional special texture 14. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT23 = GL_TEXTURE7+15; //!< This is the global slot for the additional special texture 15. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT24 = GL_TEXTURE7+16; //!< This is the global slot for the additional special texture 16. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT25 = GL_TEXTURE7+17; //!< This is the global slot for the additional special texture 17. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT26 = GL_TEXTURE7+18; //!< This is the global slot for the additional special texture 18. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT27 = GL_TEXTURE7+19; //!< This is the global slot for the additional special texture 19. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT28 = GL_TEXTURE7+20; //!< This is the global slot for the additional special texture 20. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT29 = GL_TEXTURE7+21; //!< This is the global slot for the additional special texture 21. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT30 = GL_TEXTURE7+22; //!< This is the global slot for the additional special texture 22. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT31 = GL_TEXTURE7+23; //!< This is the global slot for the additional special texture 23. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT32 = GL_TEXTURE7+24; //!< This is the global slot for the additional special texture 24. It is not secure that your machine really support as many textures as this one stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	
	const int TEX_SLOT1_SAMPLER = 0; //!< This is the global sampler2D variable for the slot for the first color texture.
	const int TEX_SLOT_SAMPLER_DIFFUSE_MAP = TEX_SLOT1_SAMPLER;
	const int TEX_SLOT2_SAMPLER = 1; //!< This is the global sampler2D variable for the slot for the second color texture.
	const int TEX_SLOT_SAMPLER_NORMAL_MAP = TEX_SLOT2_SAMPLER;
	const int TEX_SLOT3_SAMPLER = 2; //!< This is the global sampler2D variable for the slot for the first special texture.
	const int TEX_SLOT_SAMPLER_ALPHA_MAP = TEX_SLOT3_SAMPLER;
	const int TEX_SLOT4_SAMPLER = 3; //!< This is the global sampler2D variable for the slot for the second special texture.
	const int TEX_SLOT5_SAMPLER = 4; //!< This is the global sampler variable for the slot for the first shadow texture.
	const int TEX_SLOT6_SAMPLER = 5; //!< This is the global sampler variable for the slot for the second shadow texture.
	const int TEX_SLOT7_SAMPLER = 6; //!< This is the global sampler variable for the slot for the third shadow texture.
	const int TEX_SLOT8_SAMPLER = 7; //!< This is the global sampler variable for the slot for the fourth shadow texture.
	const int TEX_SLOT9_SAMPLER = 8; //!< This is the global sampler variable for the slot number 9. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT10_SAMPLER = 9; //!< This is the global sampler variable for the slot number 10. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT11_SAMPLER = 10; //!< This is the global sampler variable for the slot number 11. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT12_SAMPLER = 11; //!< This is the global sampler variable for the slot number 12. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT13_SAMPLER = 12; //!< This is the global sampler variable for the slot number 13. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT14_SAMPLER = 13; //!< This is the global sampler variable for the slot number 14. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT15_SAMPLER = 14; //!< This is the global sampler variable for the slot number 15. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT16_SAMPLER = 15; //!< This is the global sampler variable for the slot number 16. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT17_SAMPLER = 16; //!< This is the global sampler variable for the slot number 17. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT18_SAMPLER = 17; //!< This is the global sampler variable for the slot number 18. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT19_SAMPLER = 18; //!< This is the global sampler variable for the slot number 19. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT20_SAMPLER = 19; //!< This is the global sampler variable for the slot number 20. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT21_SAMPLER = 20; //!< This is the global sampler variable for the slot number 21. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT22_SAMPLER = 21; //!< This is the global sampler variable for the slot number 22. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT23_SAMPLER = 22; //!< This is the global sampler variable for the slot number 23. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT24_SAMPLER = 23; //!< This is the global sampler variable for the slot number 24. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT25_SAMPLER = 24; //!< This is the global sampler variable for the slot number 25. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT26_SAMPLER = 25; //!< This is the global sampler variable for the slot number 26. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT27_SAMPLER = 26; //!< This is the global sampler variable for the slot number 27. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT28_SAMPLER = 27; //!< This is the global sampler variable for the slot number 28. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT29_SAMPLER = 28; //!< This is the global sampler variable for the slot number 29. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT30_SAMPLER = 29; //!< This is the global sampler variable for the slot number 30. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT31_SAMPLER = 30; //!< This is the global sampler variable for the slot number 31. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	const int TEX_SLOT32_SAMPLER = 31; //!< This is the global sampler variable for the slot number 32. It is not secure that your machine really support as many textures as this slot stands for. Please check the maximum number of available slots using the static function getMaxSupportedTextures of the TextureManager.
	
	const int CUBE_MAP_POS_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	const int CUBE_MAP_NEG_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	const int CUBE_MAP_POS_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	const int CUBE_MAP_NEG_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	const int CUBE_MAP_POS_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	const int CUBE_MAP_NEG_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	
}; 