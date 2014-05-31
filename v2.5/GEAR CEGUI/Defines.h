#ifdef UICEGUIDLL_EXPORTS
#define UICEGUIDLL_API __declspec(dllexport) 
#else
#define UICEGUIDLL_API __declspec(dllimport) 
#endif

#pragma comment(lib, "GEAR.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")
#ifdef _DEBUG
	#pragma comment(lib, "glew_d.lib")
	#pragma comment(lib, "CEGUIBase-0_d.lib")
	#pragma comment(lib, "CEGUIOpenGLRenderer-0_d.lib")

#else
	#pragma comment(lib, "glew.lib")
	#pragma comment(lib, "CEGUIBase-0.lib")
	#pragma comment(lib, "CEGUIOpenGLRenderer-0.lib")
#endif