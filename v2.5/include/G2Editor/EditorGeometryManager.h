#pragma once
#include <G2Core/Entity.h>
#include <G2/EffectImporter.h>
#include <G2Core/FrameInfo.h>

#include <unordered_map>

namespace G2 
{
	class RenderSystem;
	class CameraSystem;

	namespace Editor 
	{
		class RootEditor;
		class EditorSystem;
		/** This class defines...
		 * @created:	2014/06/15
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class EditorGeometryManager 
		{
			friend class RootEditor;
			public:
				/// This constructs a new EditorGeometryManager.
				EditorGeometryManager(RootEditor* editor);

				~EditorGeometryManager();
			private:

				void _releaseResources();
				void _setAsSelected(unsigned int entityId);
				void _onRenderFrame(G2::FrameInfo const& frameInfo);
				
				G2::RenderSystem*	mRenderSystem;
				G2::CameraSystem*	mCameraSystem;
				EditorSystem*		mEditorSystem;

				RootEditor*			mEditor;
				G2::EffectImporter	mEffectImporter;

				std::unordered_map<unsigned int,G2::Entity>		mCameraFrustumEntities;	// The cached frustum entity ids for all cameras in the scene
				G2::Entity										mSelectedRCVis;
		};
	};
};