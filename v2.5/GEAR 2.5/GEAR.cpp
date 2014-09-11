// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "GEAR.h"
#include "Logger.h"
#include "FileResource.h"
#include "RenderComponent.h"

#include <G2Core/ECSManager.h>

#include <thread>

using namespace G2;


static GfxDevice* gGfxDevice = nullptr;

void G2_init(G2::GfxDevice* gfxDevice) 
{
	gGfxDevice = gfxDevice;
	assert(gGfxDevice != nullptr);

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
	GLDEBUG( glEnable(GL_DEPTH_TEST) );
	GLDEBUG( glEnable(GL_MULTISAMPLE) );
	GLDEBUG( glEnable(GL_CULL_FACE) );
	GLDEBUG( glEnable(GL_BLEND) );
	int max;
	GLDEBUG( glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max));
	logger << "Info: GL_MAX_VERTEX_ATTRIBS=" << max << endl;
	
		

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
		ECSManager::getShared().runSideThread(frameInfo);
		++frameInfo.frame;
		frameInfo.timeSinceLastFrame = frameTimer.getSeconds();
		frameInfo.timeSinceRenderStart += frameInfo.timeSinceLastFrame;
	} while (mainThreadRunning);
}

static std::thread* gSideThread = nullptr;

void G2_startSideThread()
{
	if(gSideThread == nullptr)
	{
		gSideThread = new std::thread(G2_loopSideThread);
	}
}

void G2_stopSideThread()
{
	if(gSideThread != nullptr)
	{
		gSideThread->join();
		delete gSideThread;
		gSideThread = nullptr;
	}
}
 
void 
G2_singleFrame(AbstractWindow& window, FrameInfo& frameInfo)
{
	TimeTracker	frameTimer;
	frameTimer.start(true);
		
	window.processEvents(frameInfo.frame);

	glm::vec4 const& clearColor = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>()->getClearColor();
	//GLDEBUG( glClearColor(clearColor.r,clearColor.g,clearColor.b,clearColor.a) );
	G2_gfxDevice()->clearColor(clearColor);
	GLDEBUG( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );

	EventDistributer::onRenderFrame(frameInfo);

	ECSManager::getShared().runMainThread(frameInfo);
		
	EventDistributer::onFrameRendered(frameInfo);

	window.swapBuffer();

	++frameInfo.frame;
	frameInfo.timeSinceLastFrame = frameTimer.getSeconds();
	frameInfo.timeSinceRenderStart += frameInfo.timeSinceLastFrame;

	if(frameInfo.frame%100 == 0)
	{
		G2::logger << "fps=" << (1.f / frameInfo.timeSinceLastFrame) << G2::endl;
	}
}

void
G2_loop(AbstractWindow& window) 
{
	mainThreadRunning = true;

	G2_startSideThread();
	
	TimeTracker	frameTimer;
	FrameInfo frameInfo;
	do {
		frameTimer.start(true);
		
		window.processEvents(frameInfo.frame);

		glm::vec4 const& clearColor = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>()->getClearColor();
		//GLDEBUG( glClearColor(clearColor.r,clearColor.g,clearColor.b,clearColor.a) );
		G2_gfxDevice()->clearColor(clearColor);
		GLDEBUG( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );

		EventDistributer::onRenderFrame(frameInfo);

		ECSManager::getShared().runMainThread(frameInfo);
		
		EventDistributer::onFrameRendered(frameInfo);

		window.swapBuffer();

		++frameInfo.frame;
		frameInfo.timeSinceLastFrame = frameTimer.getSeconds();
		frameInfo.timeSinceRenderStart += frameInfo.timeSinceLastFrame;

		if(frameInfo.frame%100 == 0)
		{
			G2::logger << "fps=" << (1.f / frameInfo.timeSinceLastFrame) << G2::endl;
		}

	} while(!frameInfo.stopRenderingAfterThisFrame);

	mainThreadRunning = false;
	G2_stopSideThread();
}

G2::GfxDevice* G2_gfxDevice()
{
	return gGfxDevice;
}

void G2_gfxDeviceDestroy()
{
	delete gGfxDevice;
	gGfxDevice = nullptr;
}

void
G2_shutdown() 
{
	G2_gfxDeviceDestroy();
	ECSManager::destroy();
}