#pragma once
#include "CeguiSystem.h"

#include <G2Core/BaseComponent.h>

#include <string>

class CEGUI::Window;

namespace G2 
{
	namespace UI
	{
		/// This class defines...
		/// @created:	2014/05/31
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class CeguiComponent : public BaseComponent<CeguiSystem>
		{
			public:
				/** This constructs a new CeguiComponent.
				 * It will create an invisible new root CEGUI::Window of type DefaultWindow
				 */
				UICEGUIDLL_API CeguiComponent();
				/** This constructs a new CeguiComponent by
				 * loading the given CEGUI layout file.
				 * @param layoutFile The CEGUI layout file to load.
				 */
				UICEGUIDLL_API CeguiComponent(std::string const& layoutFile);
				/** This constructs a new CeguiComponent.
				 * It will create a CEGUI::Window with the given windowType and windowName.
				 * @param windowType The type of the CEGUI::Window to create.
				 * @param windowName The name of the CEGUI::Window to create.
				 * @note Make sure that the given windowType is available in the CEGUI environment.
				 * @note Make sure you loaded all needed scheme files before creating a CeguiComponent
				 * which needs this informations be available in the CEGUI environment.
				 */
				UICEGUIDLL_API CeguiComponent(std::string const& windowType, std::string const& windowName);
				/** This function will make the CEGUI::Window of this CeguiComponent
				 * to become the new root window.
				 * In order to attach other components windows to this new root window, 
				 * make sure to also call the attachToRootWindow function on those CeguiComponents, 
				 * which you want to also visualize if not already attached.
				 */
				UICEGUIDLL_API void setAsRootWindow();
				/** This function will attach the CEGUI::Window loaded in this CeguiComponent
				 * to the currently set root CEGUI::Window in the CeguiSystem.
				 */
				UICEGUIDLL_API void attachToRootWindow();
				/** This function will detach the CEGUI::Window loaded in this CeguiComponent
				 * from the currently set root CEGUI::Window in the CeguiSystem.
				 */
				UICEGUIDLL_API void detachFromRootWindow();
				/** This function will return the CEGUI Window of this component. 
				 * @return The CEGUI Window.
				 */
				UICEGUIDLL_API CEGUI::Window* getWindow() const { return mWindow; }
			private:

				CEGUI::Window*	mWindow;	// The window of this component
				CeguiSystem*	mSystem;	// The CeguiSystem, the CeguiComponent was created with
		};
	};
};