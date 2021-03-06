#pragma once
#include "GameConsole.h"

#include <G2/GEAR.h>
#include <G2Cameras/EditorCamera.h>
#include <G2SdlContext/SDLWindow.h>
#include <vector>

class CeguiTest
{
	public:
		CeguiTest(G2::SDL::Window& window);
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

		void initCegui();

		G2::SDL::Window&			mWindow;
		G2Cameras::EditorCamera mEditorCamera;
		
		//G2::AABB	mCube;
		
		std::vector<G2::Entity> mGUIWindows;
		std::shared_ptr<GameConsoleWindow> mGameConsole;


		std::vector<G2::Entity> mPlanes;

		std::vector<G2::Entity> mWalls;

		bool		mExitRendering;
			
		G2::TextureImporter mTexImporter;
		G2::MD5Importer		mMD5Importer;
		G2::FBXImporter		mFBXImporter;
		G2::EffectImporter  mEffectImporter;

		G2::Entity mLight;

		std::vector<G2::Entity> mMd5Meshes;
		std::vector<G2::Entity> mFbxMeshes;
};