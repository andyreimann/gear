#include "CeguiComponent.h"
#include <G2Core/ECSManager.h>
#include <CEGUI/CEGUI.h>

using namespace G2::UI;

CeguiComponent::CeguiComponent() 
	: mWindow(CEGUI::WindowManager::getSingleton().createWindow( "DefaultWindow", "root" )),
	mSystem(ECSManager::getShared().getSystem<CeguiSystem,CeguiComponent>())
{

}

CeguiComponent::CeguiComponent(std::string const& layoutFile) 
	: mWindow(CEGUI::WindowManager::getSingleton().loadLayoutFromFile(layoutFile)),
	mSystem(ECSManager::getShared().getSystem<CeguiSystem,CeguiComponent>())
{
	
}

CeguiComponent::CeguiComponent(std::string const& windowType, std::string const& windowName) 
	: mWindow(CEGUI::WindowManager::getSingleton().createWindow( windowType, windowName )),
	mSystem(ECSManager::getShared().getSystem<CeguiSystem,CeguiComponent>())
{

}
void
CeguiComponent::setAsRootWindow() 
{
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow( mWindow );
}

void
CeguiComponent::attachToRootWindow() 
{
	assert(mWindow != nullptr);
	mSystem->getRootWindow()->addChild(mWindow);
}

void
CeguiComponent::detachFromRootWindow() 
{
	assert(mWindow != nullptr);
	mSystem->getRootWindow()->removeChild(mWindow->getID());
}