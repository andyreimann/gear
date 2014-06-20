#ifdef TERRAINSDLL_EXPORTS
#define TERRAINSDLL_API __declspec(dllexport) 
#else
#define TERRAINSDLL_API __declspec(dllimport) 
#endif

#pragma comment(lib, "GEARCore.lib")
#pragma comment(lib, "GEAR.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "IL.lib")
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "cg.lib")
#pragma comment(lib, "cgGL.lib")
#pragma comment(lib, "freeglut.lib")
#ifdef _DEBUG
	#pragma comment(lib, "glew_d.lib")
#else
	#pragma comment(lib, "glew.lib")
#endif