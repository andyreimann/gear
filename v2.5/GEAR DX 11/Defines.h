#ifdef GEARDX11DLL_EXPORTS
#define GEARDX11DLL_API __declspec(dllexport) 
#else
#define GEARDX11DLL_API __declspec(dllimport) 
#endif