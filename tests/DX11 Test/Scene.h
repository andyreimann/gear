#pragma once

#include <G2/GEAR.h>
#include <G2Cameras/EditorCamera.h>

class Scene
{
	public:
		Scene(G2::AbstractWindow* window);
		~Scene(void);

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
		
		G2::AbstractWindow*	mWindow;
		G2Cameras::EditorCamera mEditorCamera;
		G2::EffectImporter  mEffectImporter;
		bool mExitRendering;

		G2::Entity			mTriangle;

		G2::FBXImporter		mMeshImporter;
		std::vector<std::shared_ptr<G2::FBXMesh>> mMeshes;
};

