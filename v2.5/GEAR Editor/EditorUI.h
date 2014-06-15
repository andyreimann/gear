#pragma once
#include <G2Core/Entity.h>
#include <G2Cegui/CeguiComponent.h>
#include <G2Core/Event.h>

namespace G2 
{
	namespace Editor 
	{
		class RootEditor;
		/** This class defines...
		 * @created:	2014/06/15
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class EditorUI : public G2::Entity
		{
			public:
				/// This constructs a new EditorUI.
				EditorUI(RootEditor* editor);

				void setup();

				void show();
				void hide();

				void initCeguiResources();
				/// normal destructor
				~EditorUI();

				G2::Event<unsigned int> onComponentSelectionChanged;

			private:
				bool _onComponentSelectionChanged(const CEGUI::EventArgs &e);
				void _onRenderComponentAdded(unsigned int entityId);
				void _onRenderComponentRemoved(unsigned int entityId);
				void _updateProperties(unsigned int entityId);

				RootEditor* mEditor;
				std::string mEditorAssetsFolder;
				
				CEGUI::MultiColumnList* mComponentList;
				CEGUI::ItemListbox*		mPropertiesMainList;

		};
	};
};