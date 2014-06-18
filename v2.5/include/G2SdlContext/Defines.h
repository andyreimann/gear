#ifdef SDLCONTEXTDLL_EXPORTS
#define SDLDLL_API __declspec(dllexport) 
#else
#define SDLDLL_API __declspec(dllimport) 
#endif

#pragma comment(lib, "GEARCore.lib")
#pragma comment(lib, "GEAR.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "SDL2.lib")