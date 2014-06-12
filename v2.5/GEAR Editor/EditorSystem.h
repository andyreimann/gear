#pragma once
#include "Defines.h"
#include <G2Core/Entity.h>
#include <G2Core/BaseSystem.h>

#include <unordered_map>


namespace G2 
{
	namespace Editor 
	{
		class EditorComponent;
		class RootEditor;

		/// This class defines the system, which integrates the CEGUI library functionality into the ECS system
		/// @created:	2014/05/31
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class EditorSystem : public BaseSystem<EditorSystem,EditorComponent>
		{
			friend class RootEditor;
			public:
				/** This constructs a new CeguiSystem.
				 * It also creates a default empty root CEGUI::Window in the default CEGUI::GUIContext for you.
				 * If you plan to use the window of another CeguiComponent instead, use the CeguiComponent::setAsRootWindow function.
				 */ 
				EDITORDLL_API EditorSystem();
				
				EDITORDLL_API void runPhase(std::string const& name, G2::FrameInfo const& frameInfo);

				/// normal destructor
				EDITORDLL_API ~EditorSystem();
			private:
				void _onRenderFrame(G2::FrameInfo const& frameInfo);
				void _setRootEditor(RootEditor* editor);

				RootEditor*										mEditor;

				std::unordered_map<unsigned int,G2::Entity>	mCameraFrustumEntities;	// The cached frustum entity ids for all cameras in the scene
		};
	};
};