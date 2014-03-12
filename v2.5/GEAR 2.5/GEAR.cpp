// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "GEAR.h"
#include "Logger.h"
#include "FileResource.h"

using namespace G2;

void G2_init() 
{
	GLenum GlewInitResult;

    glewExperimental = GL_TRUE;
    GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult)
	{
		logger << "ERROR: " << glewGetErrorString(GlewInitResult) << endl;
    }
        
	GLDEBUG( logger << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl );

	// enable transparency and so on
	GLDEBUG( glEnable(GL_BLEND) );
	GLDEBUG( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

	GLint bufs;
    GLint samples;
    glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
    glGetIntegerv(GL_SAMPLES, &samples);

	GLDEBUG( logger << "Have " << bufs << " buffers and " << samples << " samples" << endl );
}

void
G2_loop(AbstractWindow& window) 
{
	bool render;
	do {
		render = window.renderSingleFrame();
	} while(render);
}