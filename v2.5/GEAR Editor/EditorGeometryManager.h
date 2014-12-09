#pragma once
#include <G2Core/Entity.h>
#include <G2/EffectImporter.h>
#include <G2Core/FrameInfo.h>

#include <unordered_map>

namespace G2 
{
	class RenderSystem;
	class CameraSystem;
	class TransformSystem;

	namespace Editor 
	{
		class RootEditor;
		class EditorSystem;
		/** This class is responsible for the editorial content placed directly in the scene.
		 * @created:	2014/06/15
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class EditorGeometryManager 
		{
			friend class RootEditor;
			public:
				/// This constructs a new EditorGeometryManager.
				EditorGeometryManager(RootEditor* editor);
				/** This will enable/disable the visibility of all
				 * editorial entities, which are placed directly in the scene.
				 * @param mode If true, everything will be shown, hidden otherwise.
				 */
				void setEditorGeometryVisibility(bool mode);

				~EditorGeometryManager();
			private:

				void _releaseResources();
				void _setAsSelected(unsigned int entityId);
				void _onRenderFrame(G2::FrameInfo const& frameInfo);

				void _initEditorGrid();
				
				// POINTER TO CORE SYSTEMS START
				G2::RenderSystem*	mRenderSystem;
				G2::CameraSystem*	mCameraSystem;
				G2::TransformSystem* mTransformSystem;
				EditorSystem*		mEditorSystem;
				// POINTER TO CORE SYSTEMS END

				RootEditor*			mEditor;					// Pointer to the root editor instance.
				G2::EffectImporter	mEffectImporter;			// an effect importer instance

				G2::Entity			mSelectedEntityHighlight;

				std::shared_ptr<G2::Effect>	mSolidFx;			// The effect for solid ambient lighting
				G2::Entity			mEditorGrid;				// The entity of the editor grid in 3D
		};
	};
};