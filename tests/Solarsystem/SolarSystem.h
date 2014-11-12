#pragma once
#include "Planet.h"
#include <G2SdlContext/SDLWindow.h>
#include <G2Cameras/EditorCamera.h>

class SolarSystem
{
	public:
		SolarSystem(G2::SDL::Window& window);
		~SolarSystem();

	private:
		void onViewportResize(int width, int height);

		void init();

		void initPlanets();

		G2::SDL::Window*			mWindow;
		G2Cameras::EditorCamera		mCamera;

		G2::TextureImporter			mTexImporter;
		G2::EffectImporter			mEffectImporter;
		G2::FBXImporter				mFbxImporter;

		G2::Entity					mSun;


		std::vector<std::shared_ptr<Planet>> mPlanets;

};

