#pragma once
#include "Defines.h"
#include <G2Core/BaseSystem.h>
#include <G2Core/MouseButton.h>
#include <G2Core/KeyCodes.h>

#include <G2/Defines.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <glm/glm.hpp>


namespace G2 
{
	namespace UI 
	{
		class CeguiComponent;

		/// This class defines the system, which integrates the CEGUI library functionality into the ECS system
		/// @created:	2014/05/31
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class CeguiSystem : public BaseSystem<CeguiSystem,CeguiComponent>
		{
			public:
				/** This constructs a new CeguiSystem.
				 * It also creates a default empty root CEGUI::Window in the default CEGUI::GUIContext for you.
				 * If you plan to use the window of another CeguiComponent instead, use the CeguiComponent::setAsRootWindow function.
				 */ 
				UICEGUIDLL_API CeguiSystem();
				
				UICEGUIDLL_API void runPhase(std::string const& name, G2::FrameInfo const& frameInfo);

				/// normal destructor
				UICEGUIDLL_API ~CeguiSystem();
				/** This function will return the CEGUI Window of this component. 
				 * @return The CEGUI Window.
				 */
				UICEGUIDLL_API CEGUI::Window* getRootWindow() const;

				UICEGUIDLL_API CEGUI::WindowManager& getWindowManager();
			private:
				void _onMouseMove(glm::detail::tvec2<int> const& mouseCoords);
				void _onMouseDown(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords);
				void _onMouseUp(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords);
				void _onMouseWheel(int delta);
				
				void _onKeyDown(G2::KeyCode keyCode);
				void _onKeyUp(G2::KeyCode keyCode);

				CEGUI::OpenGL3Renderer& mRenderer;

				glm::detail::tvec2<int> mMouseCoords;
		};
	};
};