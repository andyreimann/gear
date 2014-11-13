#ifdef GEAROPENGLDLL_EXPORTS
#define GEAROPENGLDLL_API __declspec(dllexport) 
#else
#define GEAROPENGLDLL_API __declspec(dllimport) 
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