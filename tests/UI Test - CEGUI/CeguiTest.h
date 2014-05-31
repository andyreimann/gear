#pragma once

#include <G2/GEAR.h>
#include <G2Cameras/EditorCamera.h>
#include <G2/SDLWindow.h>
#include <vector>

class CeguiTest
{
	public:
		CeguiTest(G2::SDLWindow& window);
		~CeguiTest();


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

		void createWalls();

		G2::SDLWindow&			mWindow;
		G2Cameras::EditorCamera mEditorCamera;
		
		//G2::AABB	mCube;

		std::vector<G2::Entity> mPlanes;

		std::vector<std::shared_ptr<G2::FBXMesh>> mWalls;

		bool		mExitRendering;
			
		G2::TextureImporter mTexImporter;
		G2::MD5Importer		mMD5Importer;
		G2::FBXImporter		mFBXImporter;
		G2::EffectImporter  mEffectImporter;

		std::shared_ptr<G2::FBXMesh> mLight;

		std::vector<std::shared_ptr<G2::MD5Mesh>> mMd5Meshes;
		std::vector<std::shared_ptr<G2::FBXMesh>> mFbxMeshes;
};