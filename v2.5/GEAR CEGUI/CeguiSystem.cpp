#include "CeguiSystem.h"
#include "CeguiComponent.h"

#include <G2Core/EventDistributer.h>
#include <G2/Logger.h>
#include <utility>

using namespace G2::UI;

CeguiSystem::CeguiSystem() 
	: mRenderer(CEGUI::OpenGL3Renderer::bootstrapSystem())
{
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	// The GUIContext::setRootWindow function is used, unsurprisingly, to specify a window to be used as the root of the GUI for a given GUIContext. 
	// This will replace any current root window, although do note that the previous window hierarchy is not actually destroyed 
	// - it is just detached from the GUIContext - you can easily switch between GUI 'pages' by flipping between them using the GUIContext::setRootWindow function.
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow( wmgr.createWindow( "DefaultWindow", "root" ));
	 

	// http://static.cegui.org.uk/docs/current/rendering_tutorial.html
	// Bootstrap CEGUI::System with an OpenGL3Renderer object that uses the
	// current GL viewport, the DefaultResourceProvider, and the default
	// ImageCodec.
	//
	// NB: Your OpenGL context must already be initialised when you call this; CEGUI
	// will not create the OpenGL context itself.
	EventDistributer::onMouseMove.hook(this, &CeguiSystem::_onMouseMove);
	EventDistributer::onMouseDown.hook(this, &CeguiSystem::_onMouseDown);
	EventDistributer::onMouseUp.hook(this, &CeguiSystem::_onMouseUp);
	EventDistributer::onMouseWheel.hook(this, &CeguiSystem::_onMouseWheel);
	EventDistributer::onKeyDown.hook(this, &CeguiSystem::_onKeyDown);
	EventDistributer::onKeyUp.hook(this, &CeguiSystem::_onKeyUp);
}

CeguiSystem::~CeguiSystem() 
{
	EventDistributer::onMouseMove.unHook(this, &CeguiSystem::_onMouseMove);
	EventDistributer::onMouseDown.unHook(this, &CeguiSystem::_onMouseDown);
	EventDistributer::onMouseUp.unHook(this, &CeguiSystem::_onMouseUp);
	EventDistributer::onMouseWheel.unHook(this, &CeguiSystem::_onMouseWheel);
	EventDistributer::onKeyDown.unHook(this, &CeguiSystem::_onKeyDown);
	EventDistributer::onKeyUp.unHook(this, &CeguiSystem::_onKeyUp);
}

void
CeguiSystem::runPhase(std::string const& name, G2::FrameInfo const& frameInfo) 
{ 
	if(name == "render")
	{
		GLDEBUG( glBindTexture(GL_TEXTURE_2D, 0) );
		GLDEBUG( glUseProgram(0) );
		GLDEBUG( glActiveTexture(GL_TEXTURE0) );
		GLDEBUG( glDisable(GL_DEPTH_TEST) );
		
		CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
		context.injectTimePulse((float)frameInfo.timeSinceLastFrame);
		// There is currently an oddity in the system that requires time pulses 
		// to be injected both into whatever GUIContext objects you need input 
		// for and additionally for CEGUI::System via CEGUI::System::injectTimePulse - 
		// if you fail to inject into System, you may find some things – 
		// such as animations – not working quite as they should!
		CEGUI::System::getSingleton().injectTimePulse((float)frameInfo.timeSinceLastFrame);

		CEGUI::System::getSingleton().renderAllGUIContexts();
		
		GLDEBUG( glEnable(GL_DEPTH_TEST) );
	}
}

void
CeguiSystem::_onMouseMove(glm::detail::tvec2<int> const& mouseCoords) 
{
	int dx = mouseCoords.x - mMouseCoords.x;
	int dy = mouseCoords.y - mMouseCoords.y;
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	//context.injectMouseMove((float)dx, (float)dy);
	context.injectMousePosition((float)mouseCoords.x, (float)mouseCoords.y);
	mMouseCoords = mouseCoords;
}

void
CeguiSystem::_onMouseWheel(int delta) 
{
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectMouseWheelChange((float)delta);
}

void
CeguiSystem::_onMouseDown(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords) 
{
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	if(button == G2::MOUSE_LEFT)
	{
		context.injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
	}
	else if(button == G2::MOUSE_MIDDLE)
	{
		context.injectMouseButtonDown(CEGUI::MouseButton::MiddleButton);
	}
	else if(button == G2::MOUSE_MIDDLE)
	{
		context.injectMouseButtonDown(CEGUI::MouseButton::RightButton);
	}
}

void
CeguiSystem::_onMouseUp(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords) 
{
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	if(button == G2::MOUSE_LEFT)
	{
		context.injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
	}
	else if(button == G2::MOUSE_MIDDLE)
	{
		context.injectMouseButtonUp(CEGUI::MouseButton::MiddleButton);
	}
	else if(button == G2::MOUSE_MIDDLE)
	{
		context.injectMouseButtonUp(CEGUI::MouseButton::RightButton);
	}
}

void
CeguiSystem::_onKeyDown(G2::KeyCode keyCode) 
{
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectKeyDown((CEGUI::Key::Scan)keyCode);
}

void
CeguiSystem::_onKeyUp(G2::KeyCode keyCode) 
{
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectKeyUp((CEGUI::Key::Scan)keyCode);
}

CEGUI::WindowManager&
CeguiSystem::getWindowManager() 
{
	return CEGUI::WindowManager::getSingleton();
}

CEGUI::Window*
CeguiSystem::getRootWindow() const 
{
	return CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
}