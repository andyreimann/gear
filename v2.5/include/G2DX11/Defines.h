#ifdef GEARDX11DLL_EXPORTS
#define GEARDX11DLL_API __declspec(dllexport) 
#else
#define GEARDX11DLL_API __declspec(dllimport) 
#endif

#pragma comment(lib, "GEARCore.lib")

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")