#ifdef TERRAINSDLL_EXPORTS
#define TERRAINSDLL_API __declspec(dllexport) 
#else
#define TERRAINSDLL_API __declspec(dllimport) 
#endif