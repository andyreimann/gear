#pragma once
#include "GameObject.h"

#include <G2/GEAR.h>
#include <vector>

class TestScene
{
	public:
		TestScene();
		~TestScene();


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

		void onRenderFrame(G2::FrameInfo const&);

		void createPlane(glm::vec4 const& corner, std::shared_ptr<G2::Texture2D> const& diffuseTex, glm::quat const& rot = glm::quat());
		
		void createWalls();

		GameObject	mCamera;
		
		G2::AABB	mCube;

		std::vector<GameObject> mPlanes;

		std::vector<std::shared_ptr<G2::FBXMesh>> mWalls;

		
		bool		mMoveForward;
		bool		mMoveBackward;
		bool		mMoveLeft;
		bool		mMoveRight;

		int			mMx;		
		int			mMy;

		bool		mExitRendering;
			
		G2::TextureImporter mTexImporter;
		G2::MD5Importer		mMeshImporter;
		G2::FBXImporter		mMeshImporter2;
		G2::EffectImporter  mEffectImporter;

		int							 mLightType;
		std::shared_ptr<G2::FBXMesh> mLight;


		std::shared_ptr<G2::MD5Mesh> mSampleMesh;
		std::shared_ptr<G2::FBXMesh> mSampleMesh2;
		std::shared_ptr<G2::FBXMesh> mSampleMesh3;
		std::vector<std::shared_ptr<G2::MD5Mesh>> mMd5Meshes;
		std::vector<std::shared_ptr<G2::FBXMesh>> mFbxMeshes;
};

