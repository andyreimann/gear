#ifdef EDITORDLL_EXPORTS
#define EDITORDLL_API __declspec(dllexport) 
#else
#define EDITORDLL_API __declspec(dllimport) 
#endif