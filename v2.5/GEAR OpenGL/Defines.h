#ifdef GEAROPENGLDLL_EXPORTS
#define GEAROPENGLDLL_API __declspec(dllexport) 
#else
#define GEAROPENGLDLL_API __declspec(dllimport) 
#endif

#pragma comment(lib, "GEARCore.lib")

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

#include <GL/glew.h>
#include <iostream>

#define GLCHECK(x) \
x; \
{ \
	GLenum e = glGetError(); \
	if( e != GL_NO_ERROR) \
	{ \
		std::cout << "Error at line number " << __LINE__ << ", in file " << __FILE__ << ". glGetError() returned " << gluErrorString(e) << " for call " << #x << "\n"; \
	} \
}