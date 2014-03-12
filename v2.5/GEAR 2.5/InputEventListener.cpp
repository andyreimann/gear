// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "InputEventListener.h"

#include <iostream>
#include <sstream>

#include <OIS/OISInputManager.h>
#include <OIS/OISException.h>

using namespace G2;

const char *g_DeviceType[6] = {"OISUnknown", "OISKeyboard", "OISMouse", "OISJoyStick",
							 "OISTablet", "OISOther"};

InputEventListener::InputEventListener(HWND windowHandle)
{
	std::ostringstream wnd;
	wnd << (size_t)windowHandle;
	
	OIS::ParamList pl;
	pl.insert(std::make_pair( std::string("WINDOW"), wnd.str() ));
	//This never returns null.. it will raise an exception on errors
	mInputManager = OIS::InputManager::createInputSystem(pl);

	//Lets enable all addons that were compiled in:
	mInputManager->enableAddOnFactory(OIS::InputManager::AddOn_All);

	//Print debugging information
	unsigned int v = mInputManager->getVersionNumber();
	std::cout << "OIS Version: " << (v>>16 ) << "." << ((v>>8) & 0x000000FF) << "." << (v & 0x000000FF)
		<< "\nRelease Name: " << mInputManager->getVersionName()
		<< "\nManager: " << mInputManager->inputSystemName()
		<< "\nTotal Keyboards: " << mInputManager->getNumberOfDevices(OIS::OISKeyboard)
		<< "\nTotal Mice: " << mInputManager->getNumberOfDevices(OIS::OISMouse)
		<< "\nTotal JoySticks: " << mInputManager->getNumberOfDevices(OIS::OISJoyStick);

	//List all devices
	OIS::DeviceList list = mInputManager->listFreeDevices();
	for( OIS::DeviceList::iterator i = list.begin(); i != list.end(); ++i )
		std::cout << "\n\tDevice: " << g_DeviceType[i->first] << " Vendor: " << i->second;

	mKeyboard = (OIS::Keyboard*)mInputManager->createInputObject( OIS::OISKeyboard, true );
	mKeyboard->setEventCallback( this );

	mMouse = (OIS::Mouse*)mInputManager->createInputObject( OIS::OISMouse, true );
	mMouse->setEventCallback( this );
	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = 100;
	ms.height = 100;

	try
	{
		//This demo uses at most 4 joysticks - use old way to create (i.e. disregard vendor)
#undef min
		int numSticks = std::min(mInputManager->getNumberOfDevices(OIS::OISJoyStick), 4);
		for( int i = 0; i < numSticks; ++i )
		{
			mJoysticks[i] = (OIS::JoyStick*)mInputManager->createInputObject( OIS::OISJoyStick, true );
			mJoysticks[i]->setEventCallback( this );
			std::cout << "\n\nCreating Joystick " << (i + 1)
				<< "\n\tAxes: " << mJoysticks[i]->getNumberOfComponents(OIS::OIS_Axis)
				<< "\n\tSliders: " << mJoysticks[i]->getNumberOfComponents(OIS::OIS_Slider)
				<< "\n\tPOV/HATs: " << mJoysticks[i]->getNumberOfComponents(OIS::OIS_POV)
				<< "\n\tButtons: " << mJoysticks[i]->getNumberOfComponents(OIS::OIS_Button)
				<< "\n\tVector3: " << mJoysticks[i]->getNumberOfComponents(OIS::OIS_Vector3);
		}
	}
	catch(OIS::Exception &ex)
	{
		std::cout << "\nException raised on joystick creation: " << ex.eText << std::endl;
	}
}


InputEventListener::~InputEventListener()
{
}

bool
InputEventListener::vector3Moved(const OIS::JoyStickEvent &arg, int index) 
{
	std::cout.precision(2);
	std::cout.flags(std::ios::fixed | std::ios::right);
	std::cout << std::endl << arg.device->vendor() << ". Orientation # " << index 
		<< " X Value: " << arg.state.mVectors[index].x
		<< " Y Value: " << arg.state.mVectors[index].y
		<< " Z Value: " << arg.state.mVectors[index].z;
	std::cout.precision();
	std::cout.flags();
	return true;
}

bool
InputEventListener::povMoved(const OIS::JoyStickEvent &arg, int pov) 
{
	std::cout << std::endl << arg.device->vendor() << ". POV" << pov << " ";

	if( arg.state.mPOV[pov].direction & OIS::Pov::North ) //Going up
		std::cout << "North";
	else if( arg.state.mPOV[pov].direction & OIS::Pov::South ) //Going down
		std::cout << "South";

	if( arg.state.mPOV[pov].direction & OIS::Pov::East ) //Going right
		std::cout << "East";
	else if( arg.state.mPOV[pov].direction & OIS::Pov::West ) //Going left
		std::cout << "West";

	if( arg.state.mPOV[pov].direction == OIS::Pov::Centered ) //stopped/centered out
		std::cout << "Centered";
	return true;
}

bool
InputEventListener::axisMoved(const OIS::JoyStickEvent &arg, int axis) 
{
	//Provide a little dead zone
	if( arg.state.mAxes[axis].abs > 2500 || arg.state.mAxes[axis].abs < -2500 )
		std::cout << std::endl << arg.device->vendor() << ". Axis # " << axis << " Value: " << arg.state.mAxes[axis].abs;
	return true;
}

bool
InputEventListener::buttonReleased(const OIS::JoyStickEvent &arg, int button) 
{
	std::cout << std::endl << arg.device->vendor() << ". Button Released # " << button;
	return true;
}

bool
InputEventListener::buttonPressed(const OIS::JoyStickEvent &arg, int button) 
{
	std::cout << std::endl << arg.device->vendor() << ". Button Pressed # " << button;
	return true;
}

bool
InputEventListener::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) 
{
	const OIS::MouseState& s = arg.state;
	std::cout << "\nMouse button #" << id << " released. Abs("
		<< s.X.abs << ", " << s.Y.abs << ", " << s.Z.abs << ") Rel("
		<< s.X.rel << ", " << s.Y.rel << ", " << s.Z.rel << ")";
	return true;
}

bool
InputEventListener::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) 
{
	const OIS::MouseState& s = arg.state;
	std::cout << "\nMouse button #" << id << " pressed. Abs("
		<< s.X.abs << ", " << s.Y.abs << ", " << s.Z.abs << ") Rel("
		<< s.X.rel << ", " << s.Y.rel << ", " << s.Z.rel << ")";
	return true;
}

bool
InputEventListener::mouseMoved(const OIS::MouseEvent &arg) 
{
	const OIS::MouseState& s = arg.state;
	std::cout << "\nMouseMoved: Abs("
		<< s.X.abs << ", " << s.Y.abs << ", " << s.Z.abs << ") Rel("
		<< s.X.rel << ", " << s.Y.rel << ", " << s.Z.rel << ")";
	return true;
}

bool
InputEventListener::keyReleased(const OIS::KeyEvent &arg) 
{
	std::cout << "KeyReleased {" << ((OIS::Keyboard*)(arg.device))->getAsString(arg.key) << "}\n";
	return true;
}

bool
InputEventListener::keyPressed(const OIS::KeyEvent &arg) 
{
	std::cout << " KeyPressed {" << arg.key
		<< ", " << ((OIS::Keyboard*)(arg.device))->getAsString(arg.key)
		<< "} || Character (" << (char)arg.text << ")" << std::endl;
	return true;
}