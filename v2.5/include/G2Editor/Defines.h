#ifdef EDITORDLL_EXPORTS
#define EDITORDLL_API __declspec(dllexport) 
#else
#define EDITORDLL_API __declspec(dllimport) 
#endif

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "IL.lib")
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "cg.lib")
#pragma comment(lib, "cgGL.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "GEARCegui.lib")
#ifdef _DEBUG
	#pragma comment(lib, "glew_d.lib")
#else
	#pragma comment(lib, "glew.lib")
#endif