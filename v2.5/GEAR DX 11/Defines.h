#ifdef GEARDX11DLL_EXPORTS
#define GEARDX11DLL_API __declspec(dllexport) 
#else
#define GEARDX11DLL_API __declspec(dllimport) 
#endif

#include <d3d11.h>		// core
#include <d3dx11.h>		// extensions
#include <d3dx10.h>		// dx 11 is ext of dx 10 -> useful macros usable
