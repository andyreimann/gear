#pragma once
#include "Defines.h"

#include <G2Core/FrameInfo.h>
#include <G2/VertexArrayObject.h>
#include <G2Cameras/EditorCamera.h>

#include <unordered_map>

namespace G2 
{
	class AbstractWindow;
	class RenderSystem;
	class CameraSystem;

	namespace Editor 
	{
		/// This class defines...
		/// @created:	2014/06/12
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class RootEditor 
		{
			friend class EditorSystem;
			public:
				/// This constructs a new Editor.
				EDITORDLL_API RootEditor(G2::AbstractWindow* window, std::string const& editorAssetsFolder);
				
				EDITORDLL_API void start();
				EDITORDLL_API void stop();

				EDITORDLL_API ~RootEditor();

			private:
				/** This function receives callbacks from the EventDistributer::onViewportResize Event
				 * @note It only receives callbacks if the EditableScene is prepared for editing.
				 */
				void _onViewportResize(int width, int height);

				std::string				mEditorAssetsFolder;
				unsigned int			mPreviousMainCameraId;

				G2::AbstractWindow*		mWindow;
				G2Cameras::EditorCamera mEditorCamera;
				G2::RenderSystem*		mRenderSystem;
				G2::CameraSystem*		mCameraSystem;

				
		};
	};
};