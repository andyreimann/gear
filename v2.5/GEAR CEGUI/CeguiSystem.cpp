#include "CeguiSystem.h"
#include "CeguiComponent.h"

#include <G2Core/EventDistributer.h>
#include <G2/Logger.h>
#include <utility>

using namespace G2::UI;

CeguiSystem::CeguiSystem() 
	: mRenderer(CEGUI::OpenGL3Renderer::bootstrapSystem()),
	mMouseCoordsInjected(false)
{
	// initialize the keymapper
	_init();
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
	EventDistributer::onViewportResize.hook(this, &CeguiSystem::_onViewportResize);
}

CeguiSystem::~CeguiSystem() 
{
	EventDistributer::onMouseMove.unHook(this, &CeguiSystem::_onMouseMove);
	EventDistributer::onMouseDown.unHook(this, &CeguiSystem::_onMouseDown);
	EventDistributer::onMouseUp.unHook(this, &CeguiSystem::_onMouseUp);
	EventDistributer::onMouseWheel.unHook(this, &CeguiSystem::_onMouseWheel);
	EventDistributer::onKeyDown.unHook(this, &CeguiSystem::_onKeyDown);
	EventDistributer::onKeyUp.unHook(this, &CeguiSystem::_onKeyUp);
	EventDistributer::onViewportResize.unHook(this, &CeguiSystem::_onViewportResize);
}

void
CeguiSystem::runPhase(std::string const& name, G2::FrameInfo const& frameInfo) 
{ 
	if(name == "render")
	{
		GLDEBUG( glBindTexture(GL_TEXTURE_2D, 0) );
		GLDEBUG( glUseProgram(0) );
		GLDEBUG( glActiveTexture(GL_TEXTURE0) );
		GLDEBUG( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );
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
		GLDEBUG( glDisable(GL_SCISSOR_TEST) ); // CEGUI doesn't disable it again!
	}
}

void
CeguiSystem::_onMouseMove(glm::detail::tvec2<int> const& mouseCoords) 
{
	int dx = mouseCoords.x - mMouseCoords.x;
	int dy = mouseCoords.y - mMouseCoords.y;
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	//if(mMouseCoordsInjected)
	//{
	//	context.injectMouseMove((float)dx, (float)dy);
	//}
	//else
	//{
	//	
	//	mMouseCoordsInjected = true;
	//}

	//G2::logger << "["<<mouseCoords.x<<":"<<mouseCoords.y<<"] ";
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
	auto& it = mSpecialKeyMapping.find(keyCode);
	if(it == mSpecialKeyMapping.end())
	{
		context.injectKeyDown((CEGUI::Key::Scan)keyCode);
	}
	else
	{
		context.injectKeyDown(it->second);
	}
	context.injectChar(keyCode);
}

void
CeguiSystem::_onKeyUp(G2::KeyCode keyCode) 
{
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectKeyUp((CEGUI::Key::Scan)keyCode);
}

void
CeguiSystem::_onViewportResize(int w, int h)
{
	mRenderer.setDisplaySize(CEGUI::Sizef((float)w, (float)h));
	mViewport.x = w;
	mViewport.y = h;
}

CEGUI::WindowManager&
CeguiSystem::getWindowManager() 
{
	return CEGUI::WindowManager::getSingleton();
}

CEGUI::OpenGL3Renderer&
CeguiSystem::getRenderer() 
{
	return mRenderer;
}

CEGUI::Window*
CeguiSystem::getRootWindow() const 
{
	return CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
}

void
CeguiSystem::_init() 
{
	mSpecialKeyMapping[G2::KC_ESCAPE] = CEGUI::Key::Escape;
	mSpecialKeyMapping[G2::KC_BACKSPACE] = CEGUI::Key::Backspace;
	mSpecialKeyMapping[G2::KC_TAB] = CEGUI::Key::Tab;
	mSpecialKeyMapping[G2::KC_LCTRL] = CEGUI::Key::LeftControl;
	mSpecialKeyMapping[G2::KC_LSHIFT] = CEGUI::Key::LeftShift;
	mSpecialKeyMapping[G2::KC_BACKSLASH] = CEGUI::Key::Backslash;
	mSpecialKeyMapping[G2::KC_RSHIFT] = CEGUI::Key::RightShift;
	mSpecialKeyMapping[G2::KC_RCTRL] = CEGUI::Key::RightControl;
	mSpecialKeyMapping[G2::KC_SPACE] = CEGUI::Key::Space;
	mSpecialKeyMapping[G2::KC_F1] = CEGUI::Key::F1;
	mSpecialKeyMapping[G2::KC_F2] = CEGUI::Key::F2;
	mSpecialKeyMapping[G2::KC_F3] = CEGUI::Key::F3;
	mSpecialKeyMapping[G2::KC_F4] = CEGUI::Key::F4;
	mSpecialKeyMapping[G2::KC_F5] = CEGUI::Key::F5;
	mSpecialKeyMapping[G2::KC_F6] = CEGUI::Key::F6;
	mSpecialKeyMapping[G2::KC_F7] = CEGUI::Key::F7;
	mSpecialKeyMapping[G2::KC_F8] = CEGUI::Key::F8;
	mSpecialKeyMapping[G2::KC_F9] = CEGUI::Key::F9;
	mSpecialKeyMapping[G2::KC_F10] = CEGUI::Key::F10;
	mSpecialKeyMapping[G2::KC_NUMLOCKCLEAR] = CEGUI::Key::NumLock;
	mSpecialKeyMapping[G2::KC_NUM_0] = CEGUI::Key::Numpad0;
	mSpecialKeyMapping[G2::KC_NUM_1] = CEGUI::Key::Numpad1;
	mSpecialKeyMapping[G2::KC_NUM_2] = CEGUI::Key::Numpad2;
	mSpecialKeyMapping[G2::KC_NUM_3] = CEGUI::Key::Numpad3;
	mSpecialKeyMapping[G2::KC_NUM_4] = CEGUI::Key::Numpad4;
	mSpecialKeyMapping[G2::KC_NUM_5] = CEGUI::Key::Numpad5;
	mSpecialKeyMapping[G2::KC_NUM_6] = CEGUI::Key::Numpad6;
	mSpecialKeyMapping[G2::KC_NUM_7] = CEGUI::Key::Numpad7;
	mSpecialKeyMapping[G2::KC_NUM_8] = CEGUI::Key::Numpad8;
	mSpecialKeyMapping[G2::KC_NUM_9] = CEGUI::Key::Numpad9;
	mSpecialKeyMapping[G2::KC_NUM_PLUS] = CEGUI::Key::Add;
	mSpecialKeyMapping[G2::KC_NUM_MINUS] = CEGUI::Key::Subtract;
	mSpecialKeyMapping[G2::KC_NUM_DIVIDE] = CEGUI::Key::Divide;
	mSpecialKeyMapping[G2::KC_NUM_MULTIPLY] = CEGUI::Key::Multiply;
	mSpecialKeyMapping[G2::KC_RETURN] = CEGUI::Key::Return;
	mSpecialKeyMapping[G2::KC_HOME] = CEGUI::Key::Home;
	mSpecialKeyMapping[G2::KC_PAUSE] = CEGUI::Key::Pause;
	mSpecialKeyMapping[G2::KC_UP] = CEGUI::Key::ArrowUp;
	mSpecialKeyMapping[G2::KC_DOWN] = CEGUI::Key::ArrowDown;
	mSpecialKeyMapping[G2::KC_LEFT] = CEGUI::Key::ArrowLeft;
	mSpecialKeyMapping[G2::KC_RIGHT] = CEGUI::Key::ArrowRight;
	mSpecialKeyMapping[G2::KC_DELETE] = CEGUI::Key::Delete;
	mSpecialKeyMapping[G2::KC_POWER] = CEGUI::Key::Power;
	mSpecialKeyMapping[G2::KC_SLEEP] = CEGUI::Key::Sleep;
	mSpecialKeyMapping[G2::KC_MAIL] = CEGUI::Key::Mail;
	mSpecialKeyMapping[G2::KC_NUM_ENTER] = CEGUI::Key::NumpadEnter;
	mSpecialKeyMapping[G2::KC_LALT] = CEGUI::Key::LeftAlt;
	mSpecialKeyMapping[G2::KC_RALT] = CEGUI::Key::RightAlt;
	mSpecialKeyMapping[G2::KC_MUTE] = CEGUI::Key::Mute;
	mSpecialKeyMapping[G2::KC_MINUS] = CEGUI::Key::Minus;
	mSpecialKeyMapping[G2::KC_EQUALS] = CEGUI::Key::Equals;
	mSpecialKeyMapping[G2::KC_LEFTBRACKET] = CEGUI::Key::LeftBracket;
	mSpecialKeyMapping[G2::KC_RIGHTBRACKET] = CEGUI::Key::RightBracket;
	mSpecialKeyMapping[G2::KC_PAGEUP] = CEGUI::Key::PageUp;
	mSpecialKeyMapping[G2::KC_PAGEDOWN] = CEGUI::Key::PageDown;
}