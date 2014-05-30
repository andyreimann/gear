// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "GEAR.h"
#include "Logger.h"
#include "FileResource.h"
#include <G2Core/ECSManager.h>

#include <thread>

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

bool mainThreadRunning;

void
G2_loopSideThread() 
{
	TimeTracker	frameTimer;
	FrameInfo frameInfo;


	frameTimer.start(true);
	do 
	{
		frameTimer.start(true);
		ECSManager::getShared().runOnSideThread("updateSideThread", frameInfo);
		++frameInfo.frame;
		frameInfo.timeSinceLastFrame = frameTimer.getSeconds();
		frameInfo.timeSinceRenderStart += frameInfo.timeSinceLastFrame;
	} while (mainThreadRunning);
}

void
G2_loop(AbstractWindow& window) 
{
	mainThreadRunning = true;

	std::thread sideThread(G2_loopSideThread);

	FrameInfo frameInfo;
	do {
		window.renderSingleFrame(frameInfo);
		
		GLDEBUG( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );

		EventDistributer::onRenderFrame(frameInfo);

		ECSManager::getShared().runOnMainThread("preUpdate", frameInfo);
		ECSManager::getShared().runOnMainThread("update", frameInfo);
		ECSManager::getShared().runOnMainThread("postUpdate", frameInfo);
		ECSManager::getShared().runOnMainThread("render", frameInfo);
		
		EventDistributer::onFrameRendered(frameInfo);

		window.swapBuffer(frameInfo);
		if(frameInfo.frame%100 == 0)
		{
			G2::logger << "fps=" << (1.f / frameInfo.timeSinceLastFrame) << G2::endl;
		}
	} while(!frameInfo.stopRenderingAfterThisFrame);

	mainThreadRunning = false;
	sideThread.join();
}

void
G2_shutdown() 
{
	ECSManager::destroy();
}