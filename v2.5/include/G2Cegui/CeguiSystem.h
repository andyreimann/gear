#pragma once
#include "Defines.h"
#include <G2Core/BaseSystem.h>

#include <G2/Defines.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

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
				/// This constructs a new CeguiSystem.
				UICEGUIDLL_API CeguiSystem();
				
				UICEGUIDLL_API void runPhase(std::string const& name, G2::FrameInfo const& frameInfo);

				/// normal destructor
				UICEGUIDLL_API ~CeguiSystem();
			private:

				CEGUI::OpenGL3Renderer& mRenderer;
		};
	};
};