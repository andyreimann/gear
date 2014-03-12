// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>
#include <OIS/OISJoyStick.h>
#include <windows.h>

namespace G2 
{	

	/** This class catches all events coming from any input device such as mouse, joystick, keyboard, ...
	 * THIS CLASS IS CURRENTLY NOT USED!
	 * @created:	2014/02/17
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class InputEventListener : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
	{
		public:

			InputEventListener(HWND windowHandle);

			bool keyPressed( const OIS::KeyEvent &arg );
			bool keyReleased( const OIS::KeyEvent &arg );
			bool mouseMoved( const OIS::MouseEvent &arg );
			bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
			bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
			bool buttonPressed( const OIS::JoyStickEvent &arg, int button );
			bool buttonReleased( const OIS::JoyStickEvent &arg, int button );
			bool axisMoved( const OIS::JoyStickEvent &arg, int axis );
			bool povMoved( const OIS::JoyStickEvent &arg, int pov );

			bool vector3Moved( const OIS::JoyStickEvent &arg, int index);

			~InputEventListener();
		private:
			
			OIS::InputManager*				mInputManager;
			OIS::Keyboard*					mKeyboard;			//Keyboard Device
			OIS::Mouse*						mMouse;				//Mouse Device
			OIS::JoyStick*					mJoysticks[4];		//This demo supports up to 4 controllers
	};
};