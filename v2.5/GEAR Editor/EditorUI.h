#pragma once
#include "Defines.h"

#include <G2Core/Entity.h>
#include <G2Cegui/CeguiComponent.h>
#include <G2Core/Event.h>

#include <unordered_set>

namespace G2 
{
	class Texture;
	class RenderSystem;
	class SplineAnimationSystem;
	class CameraSystem;
	class TransformSystem;

	namespace Editor 
	{

		struct ComponentEntryMetaData
		{
			ComponentEntryMetaData(unsigned int entityId, unsigned int rowId, G2::Editor::ComponentFlag::Name entryType) : entityId(entityId), rowId(rowId), existingComponents(entryType) {}
			unsigned int entityId;
			unsigned int rowId;
			unsigned int existingComponents;
		};

		class EditorSystem;
		class RootEditor;
		/** This class is responsible for the main UI of the editor.
		 * @created:	2014/06/15
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class EditorUI : public G2::Entity
		{
			public:
				/// This constructs a new EditorUI.
				EditorUI(RootEditor* editor);

				void show();
				void hide();
				/// normal destructor
				~EditorUI();

				G2::Event<unsigned int> onComponentSelectionChanged;

		private:

				void _init();
				void _initCeguiResources();

				bool _onComponentSelectionChanged(const CEGUI::EventArgs &e);
				bool _onWireframeModeChanged(const CEGUI::EventArgs &e);

				// GEAR CALLBACKS FOR CORE COMPONENTS START
				void _onRenderComponentAdded(G2::RenderSystem* system, unsigned int entityId);
				void _onSplineAnimationAdded(G2::SplineAnimationSystem* system, unsigned int entityId);
				void _onCameraComponentAdded(G2::CameraSystem* system, unsigned int entityId);
				void _onTransformComponentAdded(G2::TransformSystem* system, unsigned int entityId);
				void _onEditorComponentAdded(EditorSystem* system, unsigned int entityId);

				void _onRenderComponentRemoved(G2::RenderSystem* system, unsigned int entityId);
				void _onSplineAnimationRemoved(G2::SplineAnimationSystem* system, unsigned int entityId);
				void _onCameraComponentRemoved(G2::CameraSystem* system, unsigned int entityId);
				void _onTransformComponentRemoved(G2::TransformSystem* system, unsigned int entityId);
				void _onEditorComponentRemoved(EditorSystem* system, unsigned int entityId);
				// GEAR CALLBACKS FOR CORE COMPONENTS END

				void _updateProperties();

				bool _tryMergeEntry(unsigned int entityId, G2::Editor::ComponentFlag::Name entryFlag, std::string const& displayName);
				bool _removeEntry(unsigned int entityId, G2::Editor::ComponentFlag::Name entryFlag);
				void _createEntry(unsigned int entityId, G2::Editor::ComponentFlag::Name entryFlag, std::string const& displayName);


				RootEditor*				mEditor;
				std::string				mEditorAssetsFolder;
				ComponentEntryMetaData*	mSelectedEntry;
				
				CEGUI::MultiColumnList* mComponentList;
				CEGUI::ItemListbox*		mPropertiesMainList;

				// Properties UI Elements
				CEGUI::ToggleButton*	mWireframeModeToggle;
				CEGUI::DefaultWindow*	mNumTrianglesValue;
				CEGUI::DefaultWindow*	mNumDrawCallsValue;
				CEGUI::DefaultWindow*	mNumVaosValue;
				CEGUI::DefaultWindow*	mNumIaosValue;
				CEGUI::Editbox*			mEffectFileValue;

				std::shared_ptr<G2::Texture> dTestTex;

				std::unordered_set<unsigned int> mEntityIdsToIgnore;

		};
	};
};