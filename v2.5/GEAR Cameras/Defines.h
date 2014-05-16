#ifdef CAMERASDLL_EXPORTS
#define CAMERADLL_API __declspec(dllexport) 
#else
#define CAMERADLL_API __declspec(dllimport) 
#endif