#pragma once
#ifdef COREDLL_EXPORTS
#define COREDLL_API __declspec(dllexport) 
#else
#define COREDLL_API __declspec(dllimport) 
#endif

namespace G2Core
{
	struct GfxResource
	{
		bool valid;
	};

	namespace Buffer
	{
		enum Name {
			DEPTH = 1,
			STENCIL = 2,
			COLOR = 4,
		};
	};
	typedef unsigned int BufferFlags;

};