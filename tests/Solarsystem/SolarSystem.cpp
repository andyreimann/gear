#include "SolarSystem.h"

#include <G2/CameraComponent.h>
#include <G2/LightComponent.h>



SolarSystem::SolarSystem(G2::SDL::Window& window) :
	mWindow(&window),
	mCamera(&window)
{
	init();
}


SolarSystem::~SolarSystem()
{
	G2::EventDistributer::onRenderFrame.unHookAll(this);
}

void
SolarSystem::init()
{
	mCamera
		.pan(0.f, 0.f)
		.rotate(25.f, 0.f)
		.moveView(-15.f)
		.getComponent<G2::CameraComponent>()->setAsRenderCamera();
	mCamera.setInternals(70.f, 0.01f, 1000.f);


	// new way of loading shader
	std::shared_ptr<G2::Effect> effect = mEffectImporter.import(ASSET_PATH + "Shader/Default.g2fx");
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem, G2::RenderComponent>()
		->setDefaultEffect(effect);
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem, G2::RenderComponent>()
		->setClearColor(glm::vec4(0.f, 0.f, 0.015f, 1.f));

	G2::EventDistributer::onViewportResize.hook(this, &SolarSystem::onViewportResize);

	initPlanets();
}

void
SolarSystem::initPlanets()
{
	auto* light = mSun.addComponent<G2::LightComponent>(G2::LightType::POSITIONAL);

	light->diffuse = glm::vec4(1.f,1.f,1.f,1.f);
	light->linearAttenuation = 1.f;

	float sunScale = 0.05f;
	float scale = 0.0000003f;
	double realDaysPerSecond = 365.0 / 8.;
	double realSecondInSeconds = 1.0 / 86400.0 / realDaysPerSecond; // one day in reality is one second here

	// 88 days = one circle
	mPlanets.push_back(std::shared_ptr<Planet>(new Planet(
		"Sun",
		mFbxImporter.import(ASSET_PATH + "SolarSystem/moon.FBX", true, true, false),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(scale * sunScale * 1392684.f),
		glm::vec3(0.f, 1.f, 0.f),
		24.8 * 86400. * realSecondInSeconds,
		3.,
		mTexImporter.import(ASSET_PATH + "SolarSystem/sunmap.jpg", G2Core::DataFormat::Internal::R32G32B32A32_F, G2Core::FilterMode::NEAREST_MIPMAP_LINEAR, G2Core::FilterMode::NEAREST),
		mEffectImporter.import(ASSET_PATH + "SolarSystem/Shader/Sun.g2fx")
		)));
	mPlanets.push_back(std::shared_ptr<Planet>(new Planet(
		"Mercury",
		mFbxImporter.import(ASSET_PATH + "SolarSystem/moon.FBX", true, true, false),
		glm::vec3(5.79f, 0.0f, 0.0f),	
		glm::vec3(scale * 4879.f),
		glm::vec3(0.f, 1.f, 0.f),
		88. * 86400. * realSecondInSeconds,
		3.,
		mTexImporter.import(ASSET_PATH + "SolarSystem/moonmap2k.jpg", G2Core::DataFormat::Internal::R32G32B32A32_F, G2Core::FilterMode::NEAREST_MIPMAP_LINEAR, G2Core::FilterMode::NEAREST),
		mEffectImporter.import(ASSET_PATH + "SolarSystem/Shader/Moon.g2fx")
		)));
	mPlanets.push_back(std::shared_ptr<Planet>(new Planet(
		"Venus",
		mFbxImporter.import(ASSET_PATH + "SolarSystem/moon.FBX", true, true, false),
		glm::vec3(10.8f, 0.0f, 0.0f),
		glm::vec3(scale * 12103.f),
		glm::vec3(0.f, 1.f, 0.f),
		225. * 86400. * realSecondInSeconds,
		3.,
		mTexImporter.import(ASSET_PATH + "SolarSystem/venusmap.jpg", G2Core::DataFormat::Internal::R32G32B32A32_F, G2Core::FilterMode::NEAREST_MIPMAP_LINEAR, G2Core::FilterMode::NEAREST),
		mEffectImporter.import(ASSET_PATH + "SolarSystem/Shader/Moon.g2fx")
		)));
	mPlanets.push_back(std::shared_ptr<Planet>(new Planet(
		"Earth",
		mFbxImporter.import(ASSET_PATH + "SolarSystem/moon.FBX", true, true, false),
		glm::vec3(15.0f, 0.0f, 0.0f),
		glm::vec3(scale * 12734.f),
		glm::vec3(0.f, 1.f, 0.f),
		365. * 86400. * realSecondInSeconds,
		3.,
		mTexImporter.import(ASSET_PATH + "SolarSystem/earthmap.jpg", G2Core::DataFormat::Internal::R32G32B32A32_F, G2Core::FilterMode::NEAREST_MIPMAP_LINEAR, G2Core::FilterMode::NEAREST),
		mEffectImporter.import(ASSET_PATH + "SolarSystem/Shader/Moon.g2fx")
		)));
	mPlanets.push_back(std::shared_ptr<Planet>(new Planet(
		"Mars",
		mFbxImporter.import(ASSET_PATH + "SolarSystem/moon.FBX", true, true, false),
		glm::vec3(22.8f, 0.0f, 0.0f),
		glm::vec3(scale * 6772.f),
		glm::vec3(0.f, 1.f, 0.f),
		687. * 86400. * realSecondInSeconds,
		3.,
		mTexImporter.import(ASSET_PATH + "SolarSystem/marsmap.jpg", G2Core::DataFormat::Internal::R32G32B32A32_F, G2Core::FilterMode::NEAREST_MIPMAP_LINEAR, G2Core::FilterMode::NEAREST),
		mEffectImporter.import(ASSET_PATH + "SolarSystem/Shader/Moon.g2fx")
		)));
	mPlanets.push_back(std::shared_ptr<Planet>(new Planet(
		"Jupiter",
		mFbxImporter.import(ASSET_PATH + "SolarSystem/moon.FBX", true, true, false),
		glm::vec3(77.8f, 0.0f, 0.0f),
		glm::vec3(scale * 138346.f),
		glm::vec3(0.f, 1.f, 0.f),
		4329. * 86400. * realSecondInSeconds,
		3.,
		mTexImporter.import(ASSET_PATH + "SolarSystem/jupitermap.jpg", G2Core::DataFormat::Internal::R32G32B32A32_F, G2Core::FilterMode::NEAREST_MIPMAP_LINEAR, G2Core::FilterMode::NEAREST),
		mEffectImporter.import(ASSET_PATH + "SolarSystem/Shader/Moon.g2fx")
		)));
	mPlanets.push_back(std::shared_ptr<Planet>(new Planet(
		"Saturn",
		mFbxImporter.import(ASSET_PATH + "SolarSystem/moon.FBX", true, true, false),
		glm::vec3(143.3f, 0.0f, 0.0f),
		glm::vec3(scale * 114632.f),
		glm::vec3(0.f, 1.f, 0.f),
		10751. * 86400. * realSecondInSeconds,
		3.,
		mTexImporter.import(ASSET_PATH + "SolarSystem/saturnmap.jpg", G2Core::DataFormat::Internal::R32G32B32A32_F, G2Core::FilterMode::NEAREST_MIPMAP_LINEAR, G2Core::FilterMode::NEAREST),
		mEffectImporter.import(ASSET_PATH + "SolarSystem/Shader/Moon.g2fx")
		)));
	mPlanets.push_back(std::shared_ptr<Planet>(new Planet(
		"Uranus",
		mFbxImporter.import(ASSET_PATH + "SolarSystem/moon.FBX", true, true, false),
		glm::vec3(287.2f, 0.0f, 0.0f),
		glm::vec3(scale * 50532.f),
		glm::vec3(0.f, 1.f, 0.f),
		30664. * 86400. * realSecondInSeconds,
		3.,
		mTexImporter.import(ASSET_PATH + "SolarSystem/uranusmap.jpg", G2Core::DataFormat::Internal::R32G32B32A32_F, G2Core::FilterMode::NEAREST_MIPMAP_LINEAR, G2Core::FilterMode::NEAREST),
		mEffectImporter.import(ASSET_PATH + "SolarSystem/Shader/Moon.g2fx")
		)));
	mPlanets.push_back(std::shared_ptr<Planet>(new Planet(
		"Neptune",
		mFbxImporter.import(ASSET_PATH + "SolarSystem/moon.FBX", true, true, false),
		glm::vec3(449.5f, 0.0f, 0.0f),
		glm::vec3(scale * 49105.f),
		glm::vec3(0.f, 1.f, 0.f),
		60148. * 86400. * realSecondInSeconds,
		3.,
		mTexImporter.import(ASSET_PATH + "SolarSystem/neptunemap.jpg", G2Core::DataFormat::Internal::R32G32B32A32_F, G2Core::FilterMode::NEAREST_MIPMAP_LINEAR, G2Core::FilterMode::NEAREST),
		mEffectImporter.import(ASSET_PATH + "SolarSystem/Shader/Moon.g2fx")
		)));
}

void
SolarSystem::onViewportResize(int width, int height)
{
	mCamera.setViewport(width, height);
}