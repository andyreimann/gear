#pragma once
#include <G2/GEAR.h>

#include <G2Core/KeyCodes.h>


#include <G2Cameras/EditorCamera.h>

#include <glm/glm.hpp>

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>		// core
#include <d3dx11.h>		// extensions
#include <d3dx10.h>		// dx 11 is ext of dx 10 -> useful macros usable

class DX11Window : public G2::AbstractWindow
{
public:
	/// This constructs a new SDLWindow.
	/// @param title The title of the SDLWindow
	/// @param width The width of the SDLWindow
	/// @param heightThe width of the SDLWindow
	/// @param hideMouse the Flag for hiding the mouse
	DX11Window(std::string const& title, unsigned int width, unsigned int height, bool hideMouse);
	/** This function triggers the event processing of one single frame.
		*/
	virtual void processEvents(int frame) override;

	virtual void swapBuffer() override;

	virtual void setHideMouseMode(bool mode) override;
	/** normal destructor
		*/
	~DX11Window();
private:
	/// this function should be called whenever the hide mouse state is changed.
	void initHideMouseState();

	/** This function receives callbacks from the EventDistributer::onKeyUp Event
		* @note It only receives callbacks if the EditableScene is prepared for editing.
		*/
	void onKeyUp(G2::KeyCode keyCode); 
	/** This function receives callbacks from the EventDistributer::onKeyDown Event
		* @note It only receives callbacks if the EditableScene is prepared for editing.
		*/
	void onKeyDown(G2::KeyCode keyCode);

	glm::detail::tvec2<int>	mMousePosition;

	HWND hWnd;					// The handle of the window
	HINSTANCE hInstance;		// The application handle
};