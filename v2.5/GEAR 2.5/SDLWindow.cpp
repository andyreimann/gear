// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "SDLWindow.h"
#include "Defines.h"
#include "AABB.h"
#include "Logger.h"

#include <G2Core/ECSManager.h>
#include <G2Core/EventDistributer.h>

#include <iostream>
#include <unordered_map>

using namespace G2;

/* A simple function that prints a message, the error code returned by SDL,
 * and quits the application */
void sdldie(const char *msg)
{
	std::cout << msg << SDL_GetError() << std::endl;
	SDL_Quit();
	exit(1);
}

void checkSDLError(int line = -1)
{
#ifndef NDEBUG
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		printf("SDL Error: %s\n", error);
		if (line != -1)
			printf(" + line: %i\n", line);
		SDL_ClearError();
	}
#endif
}

SDLWindow::SDLWindow(std::string const& title, unsigned int width, unsigned int height, bool hideMouse)
	: AbstractWindow(title,width,height,hideMouse),
	mMousePosition(0,0),
	mZDown(false),
	mCtrlDown(false),
	mNsightActive(false)
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
	{/* Initialize SDL's Video subsystem */
		system("PAUSE");
		sdldie("Unable to initialize SDL"); /* Or die on error */
	}
	/* Request opengl 3.2 context.
	 * SDL doesn't have the ability to choose which profile at this time of writing,
	 * but it should default to the core profile */
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	/* Turn on double buffering with a 24bit Z buffer.
	 * You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// enable multisampling support on SQL 2
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	/* Create our window centered at 512x512 resolution */
	mSDLWindow = SDL_CreateWindow(mTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								  mWidth, mHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	SDL_SetWindowTitle(mSDLWindow, mTitle.c_str());

	if (!mSDLWindow) 
	{/* Die if creation failed */
		system("PAUSE");
		sdldie("Unable to create window");
	}
	checkSDLError(__LINE__);

	/* Create our opengl context and attach it to our window */
	mMainContext = SDL_GL_CreateContext(mSDLWindow);
	checkSDLError(__LINE__);

	/* enable v-sync */
	SDL_GL_SetSwapInterval(1);

	initHideMouseState();

	GLDEBUG( glViewport(0, 0, mWidth, mHeight) );
}

SDLWindow::~SDLWindow() 
{
	SDL_GL_DeleteContext(mMainContext);
	SDL_DestroyWindow(mSDLWindow);
	SDL_Quit();
}

bool
SDLWindow::renderSingleFrame() 
{
	if(mFrameInfo.frame == 0)
	{
		EventDistributer::onViewportResize(getWidth(),getHeight());
	}
	++mFrameInfo.frame;
	mFrameTimer.start(true);
	SDL_Event e;
	while ( SDL_PollEvent(&e) ) 
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN) {
			EventDistributer::onMouseDown(e.button.button, glm::detail::tvec2<int>(e.button.x,e.button.y));
		}
		else if (e.type == SDL_MOUSEBUTTONUP) {
			EventDistributer::onMouseUp(e.button.button, glm::detail::tvec2<int>(e.button.x,e.button.y));
		}
		else if (e.type == SDL_MOUSEMOTION) {
			if(mHideMouse)
			{
				if(!SDL_GetRelativeMouseMode() || mNsightActive) 
				{
						continue;
				}
			}
			// track relative motion to absolute motion
			mMousePosition.x += e.motion.xrel;
			mMousePosition.y += e.motion.yrel;
			EventDistributer::onMouseMove(mMousePosition);
		}
		else if (e.type == SDL_MOUSEWHEEL) {
			EventDistributer::onMouseWheel(e.wheel.y);
		}
		else if (e.type == SDL_KEYDOWN) {
			EventDistributer::onKeyDown(static_cast<G2::KeyCode>(e.key.keysym.sym));
			onKeyDown(static_cast<G2::KeyCode>(e.key.keysym.sym));
		}
		else if (e.type == SDL_KEYUP) {
			EventDistributer::onKeyUp(static_cast<G2::KeyCode>(e.key.keysym.sym));
			onKeyUp(static_cast<G2::KeyCode>(e.key.keysym.sym));
		}
		else if (e.type == SDL_WINDOWEVENT) {
			switch (e.window.event) 
			{
				case SDL_WINDOWEVENT_SHOWN:
					printf("[SDLWindow] : Window %d shown\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_HIDDEN:
					printf("[SDLWindow] : Window %d hidden\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_EXPOSED:
					printf("[SDLWindow] : Window %d exposed\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_MOVED:
					printf("[SDLWindow] : Window %d moved to %d,%d\n",
							e.window.windowID, e.window.data1,
							e.window.data2);
					break;
				case SDL_WINDOWEVENT_RESIZED:
					printf("[SDLWindow] : Window %d resized to %dx%d\n",
							e.window.windowID, e.window.data1,
							e.window.data2);
					GLDEBUG( glViewport(0, 0, e.window.data1, e.window.data2) );
					EventDistributer::onViewportResize(e.window.data1,e.window.data2);
					break;
				case SDL_WINDOWEVENT_MINIMIZED:
					printf("[SDLWindow] : Window %d minimized\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_MAXIMIZED:
					printf("[SDLWindow] : Window %d maximized\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_RESTORED:
					printf("[SDLWindow] : Window %d restored\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_ENTER:
					printf("[SDLWindow] : Mouse entered window %d\n",
							e.window.windowID);
					break;
				case SDL_WINDOWEVENT_LEAVE:
					printf("[SDLWindow] : Mouse left window %d\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					printf("[SDLWindow] : Window %d gained keyboard focus\n",
							e.window.windowID);
					// enable relative mouse mode
					if(mHideMouse && !mNsightActive)
					{
						SDL_SetRelativeMouseMode(SDL_TRUE);
					}
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					printf("[SDLWindow] : Window %d lost keyboard focus\n",
							e.window.windowID);
					if(mHideMouse)
					{
						// leave relative mouse mode
						SDL_SetRelativeMouseMode(SDL_FALSE);
					}
					break;
				case SDL_WINDOWEVENT_CLOSE:
					printf("[SDLWindow] : Window %d closed\n", e.window.windowID);
					break;
				default:
					printf("[SDLWindow] : Window %d got unknown event %d\n",
							e.window.windowID, e.window.event);
					break;

				break;
			}
			
		}
	}

	GLDEBUG( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
	
	EventDistributer::onRenderFrame(mFrameInfo);
	
	ECSManager::getShared().runPhaseOnSystems("preUpdate", mFrameInfo);
	ECSManager::getShared().runPhaseOnSystems("update", mFrameInfo);
	ECSManager::getShared().runPhaseOnSystems("postUpdate", mFrameInfo);
	ECSManager::getShared().runPhaseOnSystems("render", mFrameInfo);

	SDL_GL_SwapWindow(mSDLWindow);
	mFrameInfo.timeSinceLastFrame = mFrameTimer.getSeconds();
	return !mFrameInfo.stopRenderingAfterThisFrame;
}

void
SDLWindow::onKeyUp(G2::KeyCode keyCode) 
{
	if(mHideMouse)
	{
		if(keyCode == KC_Z)
		{
			mZDown = !mZDown;
		}
		else if(keyCode == KC_LCTRL)
		{
			mCtrlDown = !mCtrlDown;
		}
	}
}

void
SDLWindow::onKeyDown(G2::KeyCode keyCode) 
{
	if(mHideMouse)
	{
		if(keyCode == KC_Z)
		{
			mZDown = !mZDown;
			if((mCtrlDown && mZDown) || (!mCtrlDown && !mZDown))
			{
				// activate
				mNsightActive = !mNsightActive;
				if(mNsightActive)
				{
					logger << "Activate NSight mode" << endl;
					SDL_SetRelativeMouseMode(SDL_FALSE);
				}
				else
				{
					logger << "Deactivate NSight mode" << endl;
					SDL_SetRelativeMouseMode(SDL_TRUE);
				}
			}
		}
		else if(keyCode == KC_LCTRL)
		{
			mCtrlDown = !mCtrlDown;
			if((mCtrlDown && mZDown) || (!mCtrlDown && !mZDown))
			{
				// activate
				mNsightActive = !mNsightActive;
				if(mNsightActive)
				{
					logger << "Activate NSight mode" << endl;
					SDL_SetRelativeMouseMode(SDL_FALSE);
				}
				else
				{
					logger << "Deactivate NSight mode" << endl;
					SDL_SetRelativeMouseMode(SDL_TRUE);
				}
			}
		}
	}
}

void
SDLWindow::initHideMouseState() 
{
	if(mHideMouse)
	{
		// set mouse motion capturing to relative mode.
		// driver will hide mouse and report continuous relative movement only
		// without actually changing the absolute mouse position.
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else
	{
		mNsightActive = false;
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

void
SDLWindow::setHideMouseMode(bool mode) 
{
	mHideMouse = mode;
	initHideMouseState();
}