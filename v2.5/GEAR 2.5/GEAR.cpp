// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "GEAR.h"
#include "Logger.h"
#include "FileResource.h"
#include "RenderComponent.h"

#include <G2Core/ECSManager.h>
#include <G2Core/GfxDevice.h>

#include <thread>

using namespace G2;


void G2_init() 
{


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
	G2_gfxDevice()->clearColor(clearColor);
	G2_gfxDevice()->clearBuffers(G2Core::Buffer::COLOR | G2Core::Buffer::DEPTH, nullptr);
	
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
		G2_gfxDevice()->clearColor(clearColor);
		G2_gfxDevice()->clearBuffers(G2Core::Buffer::COLOR | G2Core::Buffer::DEPTH, nullptr);
		
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

void
G2_shutdown() 
{
	ECSManager::destroy();
	G2_gfxDeviceDestroy();
}