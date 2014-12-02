
#include <GL/glew.h>
#ifndef GLDEBUG
#define GLDEBUG(x) \
x; \
{ \
	GLenum e = glGetError(); \
	if( e != GL_NO_ERROR) \
					{ \
		std::cout << "Error at line number " << __LINE__ << ", in file " << __FILE__ << ". glGetError() returned " << gluErrorString(e) << " for call " << #x << "\n"; \
					} \
}
#endif