// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "AbstractWindow.h"

using namespace G2;

AbstractWindow::AbstractWindow(std::string const& title, unsigned int width, unsigned int height, bool hideMouse)
	: mWidth(width),
	mHeight(height),
	mTitle(title),
	mHideMouse(hideMouse)
{
}
