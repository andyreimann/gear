#ifdef SDLCONTEXTDLL_EXPORTS
#define SDLDLL_API __declspec(dllexport) 
#else
#define SDLDLL_API __declspec(dllimport) 
#endif