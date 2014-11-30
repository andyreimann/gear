#pragma once
#include "Planet.h"
#include <G2SdlContext/SDLWindow.h>
#include <G2Cameras/EditorCamera.h>
#include <G2/CatmullRomCurve.h>

class SolarSystem
{
	public:
		SolarSystem(G2::SDL::Window& window);
		~SolarSystem();

	private:
		void onViewportResize(int width, int height);
		void onRenderFrame(G2::FrameInfo const& frame);

		void init();

		void initPlanets();
		void initSky();


		void createTexturedPlane(glm::vec3 const& center, float width, float height);
		void updatePostProcessShader();

		void onPhaseStarted(std::string const& phase, G2::FrameInfo const& frame);

		void onKeyDown(G2::KeyCode key);

		G2::SDL::Window*			mWindow;
		G2Cameras::EditorCamera		mCamera;


		G2::TextureImporter			mTexImporter;
		G2::EffectImporter			mEffectImporter;
		G2::FBXImporter				mFbxImporter;

		G2::Entity					mSun;
		std::shared_ptr<G2::FBXMesh> mSkySphere;


		std::vector<std::shared_ptr<Planet>> mPlanets;
		std::vector<G2::Entity>		mTexturedPlanes;

		std::shared_ptr<G2::Effect> mGodRayPostProcess;

		std::vector<G2::CurveSample> mCameraSamples;
		bool						mCameraAnimation;

};

