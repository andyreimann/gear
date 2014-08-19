#pragma once
#include <G2/GEAR.h>
#include <G2SdlContext/SDLWindow.h>

#include <G2Cameras/EditorCamera.h>

#include <G2Editor/Editor.h>

#include <vector>

class SplineAnimationTest
{
	public:
		SplineAnimationTest(G2::SDL::Window& window);
		~SplineAnimationTest();
	private:
		/** This function receives callbacks from the EventDistributer::onViewportResize Event
		 * @note It only receives callbacks if the EditableScene is prepared for editing.
		 */
		void onViewportResize(int width, int height);
		/** This function receives callbacks from the EventDistributer::onKeyUp Event
			* @note It only receives callbacks if the EditableScene is prepared for editing.
			*/
		void onKeyUp(G2::KeyCode keyCode);
		/** This function receives callbacks from the EventDistributer::onKeyDown Event
			* @note It only receives callbacks if the EditableScene is prepared for editing.
			*/
		void onKeyDown(G2::KeyCode keyCode);
		/** This function receives callbacks from the EventDistributer::onMouseMove Event
		 * @note It only receives callbacks if the EditableScene is prepared for editing.
		 */
		void onMouseMove(glm::detail::tvec2<int> const& mouseCoords);
		void onMouseDown(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords);

		void onRenderFrame(G2::FrameInfo const&);

		void createSpline(std::shared_ptr<G2::Curve> curve, std::string const& name);
		
		bool						mExitRendering;
		G2::SDL::Window&			mWindow;
		G2Cameras::EditorCamera		mEditorCamera;
		G2::EffectImporter			mEffectImporter;
		G2::TextureImporter			mTexImporter;
		G2::FBXImporter				mFbxImporter;
		
		std::shared_ptr<G2::FBXMesh> mLight;

		G2::Editor::RootEditor		mEditor;
		
		std::vector<std::shared_ptr<G2::FBXMesh>>	mSplines;
		std::vector<G2::Entity>						mSplineVis;

		bool						mCameraSplineEnabled;
};