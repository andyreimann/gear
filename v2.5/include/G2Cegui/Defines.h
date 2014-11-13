#ifdef UICEGUIDLL_EXPORTS
#define UICEGUIDLL_API __declspec(dllexport) 
#else
#define UICEGUIDLL_API __declspec(dllimport) 
#endif