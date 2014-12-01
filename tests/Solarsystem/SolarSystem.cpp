#include "SolarSystem.h"

#include <G2/CameraComponent.h>
#include <G2/LightComponent.h>
#include <G2/Effect.h>
#include <G2/Shader.h>


SolarSystem::SolarSystem(G2::SDL::Window& window) :
	mWindow(&window),
	mCamera(&window),
	mCameraAnimation(false)
{
	init();
}


SolarSystem::~SolarSystem()
{
	G2::EventDistributer::onViewportResize.unHookAll(this);
	G2::EventDistributer::onRenderFrame.unHookAll(this);
	G2::EventDistributer::onPhaseStarted.unHookAll(this);
	G2::EventDistributer::onKeyDown.unHookAll(this);
}

void
SolarSystem::init()
{
	mCamera
		.translate(0.f, 0.f)
		.rotate(25.f, 0.f)
		.zoom(-15.f)
		.getComponent<G2::CameraComponent>()->setAsRenderCamera();
	mCamera.setInternals(70.f, 0.01f, 1000.f);


	// new way of loading shader
	std::shared_ptr<G2::Effect> effect = mEffectImporter.import(ASSET_PATH + "Shader/Default.g2fx");
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem, G2::RenderComponent>()
		->setDefaultEffect(effect);
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem, G2::RenderComponent>()
		->setClearColor(glm::vec4(0.001f, 0.001f, 0.002f, 1.f));

	G2::EventDistributer::onViewportResize.hook(this, &SolarSystem::onViewportResize);
	G2::EventDistributer::onRenderFrame.hook(this, &SolarSystem::onRenderFrame);
	G2::EventDistributer::onPhaseStarted.hook(this, &SolarSystem::onPhaseStarted);
	G2::EventDistributer::onKeyDown.hook(this, &SolarSystem::onKeyDown);

	initPlanets();
	initSky();
}

void
SolarSystem::initPlanets()
{
	auto* light = mSun.addComponent<G2::LightComponent>(G2::LightType::POSITIONAL);

	light->diffuse = glm::vec4(1.f,1.f,1.f,1.f);
	light->linearAttenuation = 1.f;

	float sunScale = 0.01f;
	float scale = 0.0000003f;
	double realDaysPerSecond = 365.0 / 80.;
	double realSecondInSeconds = 1.0 / 86400.0 / realDaysPerSecond; // one day in reality is one second here

	auto sunEffect = mEffectImporter.import(ASSET_PATH + "SolarSystem/Shader/Sun.g2fx");
	
	
	
	
	//auto it = sunEffect->getPasses().begin();
	//++it;
	//auto list = it->getSetting("RenderLayerIncludes", "").toList(",");



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
		sunEffect
		)));
	mPlanets.back()->getPlanetMesh()->getComponent<G2::RenderComponent>()
		->material.setAmbient(glm::vec4(0.9f, 0.9f, 0.8f, 1.f));

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

	//createTexturedPlane(glm::vec3(20.f,0.f,0.f), 10.f, 10.f);

	//mGodRayPostProcess = mEffectImporter.import(ASSET_PATH + "Shader/BlurShader.g2fx");
	mGodRayPostProcess = mEffectImporter.import(ASSET_PATH + "SolarSystem/Shader/GodRays.g2fx");

	
	



	//G2::ECSManager::getShared()
	//	.getSystem<G2::RenderSystem, G2::RenderComponent>()
	//	->addPostProcessingEffect(mEffectImporter.import(ASSET_PATH + "SolarSystem/Shader/GodRayBlur.g2fx"));


	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem, G2::RenderComponent>()
		->addPostProcessingEffect(mGodRayPostProcess);

	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem, G2::RenderComponent>()
		->addPostProcessingEffect(mEffectImporter.import(ASSET_PATH + "SolarSystem/Shader/SceneCombine.g2fx"));
}

void
SolarSystem::initSky()
{
	// Editor camera has far clip plane of 1000
	mSkySphere = mFbxImporter.import(ASSET_PATH + "Resources/unit-sphere-high-res.fbx");
	auto* skyTransformation = mSkySphere->addComponent<G2::TransformComponent>();
	skyTransformation->setScale(glm::vec3(990.f, 990.f, 990.f));

	mSkySphere->addComponent<G2::NameComponent>("sky_cam_attached");
	auto* skyRender = mSkySphere->getComponent<G2::RenderComponent>();
	skyRender->setFaceCulling(G2Core::FaceCulling::FRONT_FACE);
	skyRender->setEffect(mEffectImporter.import(ASSET_PATH + "Shader/SimpleTex.g2fx"));
	skyRender->material.setTexture(
		G2::Sampler::DIFFUSE,
		mTexImporter.import(ASSET_PATH + "SolarSystem/starmap_low.jpg", G2Core::DataFormat::Internal::R32G32B32A32_F, G2Core::FilterMode::LINEAR, G2Core::FilterMode::LINEAR)
		);
	skyRender->material.setDiffuse(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	skyRender->material.setAmbient(glm::vec4(0.08f, 0.08f, 0.08f, 1.f));
	skyTransformation->rotateX(90.f);
	// connect skybox to main cameras inverse translation
	skyTransformation->setParent(mCamera.getComponent<G2::TransformComponent>());
	// load skybox end
}

void
SolarSystem::updatePostProcessShader()
{
	if (mGodRayPostProcess.get() != nullptr)
	{
		auto shader = mGodRayPostProcess->getShader();
		shader->bind();
		// set some variables

		int viewport[4] = { 0, 0, mGodRayPostProcess->getSetting("RenderTargetWidth", "-1").toInt(), mGodRayPostProcess->getSetting("RenderTargetHeight", "-1").toInt() };

		glm::detail::tvec3<double> screenCoord;


		double modelView[16];
		double projection[16];
		auto* camTrans = mCamera.addComponent<G2::TransformComponent>();
		auto* camComponent = mCamera.getComponent<G2::CameraComponent>();

		for (int i = 0; i < 16; ++i)
		{
			modelView[i] = glm::value_ptr(camTrans->getWorldSpaceMatrix())[i];
			projection[i] = glm::value_ptr(camComponent->getProjectionMatrix())[i];
		}

		gluProject(0.f,
			0.f,
			0.f,
			modelView,
			projection,
			viewport,
			&screenCoord.x,
			&screenCoord.y,
			&screenCoord.z);

		shader->setProperty("exposure", 0.007f);
		shader->setProperty("decay", 1.0f);
		shader->setProperty("density", 0.99f);
		shader->setProperty("weight", 0.3f);


		float OFF_SCREEN_RENDER_RATIO_X = camComponent->getViewportWidth() / mGodRayPostProcess->getSetting("RenderTargetWidth", "-1").toFloat();
		float OFF_SCREEN_RENDER_RATIO_Y = camComponent->getViewportHeight() / mGodRayPostProcess->getSetting("RenderTargetHeight", "-1").toFloat();

		float uniformLightX = screenCoord.x / ((float)camComponent->getViewportWidth() / OFF_SCREEN_RENDER_RATIO_X);
		float uniformLightY = screenCoord.y / ((float)camComponent->getViewportHeight() / OFF_SCREEN_RENDER_RATIO_Y);

		shader->setProperty("lightPositionOnScreen", glm::vec2(uniformLightX, uniformLightY));

		//G2::logger << glm::vec2(uniformLightX, uniformLightY) << G2::endl;
	}
}

void
SolarSystem::onRenderFrame(G2::FrameInfo const& frame)
{
	//updatePostProcessShader();

	if (mCameraAnimation)
	{
		auto* trans = mCamera.getComponent<G2::TransformComponent>();
		if (trans != nullptr)
		{
			glm::vec3 const& pos = trans->getPosition();
			if (pos != glm::vec3(0, 0, 0))
			{
				glm::mat4 rot = glm::lookAt(-pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
				mCamera.getComponent<G2::TransformComponent>()->setRotation(rot);
			}
		}
	}
}

void
SolarSystem::createTexturedPlane(glm::vec3 const& center, float width, float height)
{
	mTexturedPlanes.push_back(G2::Entity());
	auto* plane = mTexturedPlanes.back().addComponent<G2::RenderComponent>();
	plane->allocateVertexArrays(1);
	plane->allocateIndexArrays(1);

	//// prepare vao
	//// assign vao to RenderComponent using move semantic
	G2::VertexArrayObject& vao = plane->getVertexArray(0);

	vao.resizeElementCount(4);

	glm::vec3 planeGeometry[4];
	width *= 0.5f;
	height *= 0.5f;
	planeGeometry[0] = center + glm::vec3(-width, 0.f, -height);
	planeGeometry[1] = center + glm::vec3(width, 0.f, -height);
	planeGeometry[2] = center + glm::vec3(width, 0.f, height);
	planeGeometry[3] = center + glm::vec3(-width, 0.f, height);

	vao.writeData(G2Core::Semantics::POSITION, planeGeometry);

	glm::vec2 tex[4];
	tex[0] = glm::vec2(0.f, 0.f);
	tex[1] = glm::vec2(1.f, 0.f);
	tex[2] = glm::vec2(1.f, 1.f);
	tex[3] = glm::vec2(0.f, 1.f);
	vao.writeData(G2Core::Semantics::TEXCOORD_0, tex);

	// build indices
	G2::IndexArrayObject& iao = plane->getIndexArray(0);
	unsigned int indices[6] = { 0, 3, 1, 1, 3, 2 };
	iao.writeIndices(&indices[0], 6);

	// add draw call
	plane->addDrawCall(G2::DrawCall()
		.setDrawMode(G2Core::DrawMode::TRIANGLES)
		.setIaoIndex(0)
		.setVaoIndex(0));

	// load and assign texturing shader
	plane->setEffect(mEffectImporter.import(ASSET_PATH + "SolarSystem/Shader/TexViewer.g2fx"));
}


void
SolarSystem::onViewportResize(int width, int height)
{
	mCamera.setViewport(width, height);
}

void
SolarSystem::onPhaseStarted(std::string const& phase, G2::FrameInfo const& frame)
{
	if (phase == "render")
	{
		updatePostProcessShader();
	}
}

void
SolarSystem::onKeyDown(G2::KeyCode key)
{
	if (key == G2::KC_A)
	{

		std::vector<G2::CurveSample> curveSamples;

		float zOffset = 0.f;
		G2::CurveSample sample;
		sample.point = glm::vec3(0.67, -5.4, -14);
		sample.speed = 0.15f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(8.1,-8.75,-4.1);
		sample.speed = 0.25f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(6,1.3,20);
		sample.speed = 0.15f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(-8.9,10.75,13.5);
		sample.speed = 0.15f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(17,2.1,-1.1);
		sample.speed = 0.15f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(-0.8,-1.8,-13.6);
		sample.speed = 0.25f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(-77, -3.6, -25);
		sample.speed = 0.15f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(-79.4, -3.9, -25.8);
		sample.speed = 0.15f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(-60, -3.9, 73);
		sample.speed = 0.15f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(-60, -3.6, 111);
		sample.speed = 0.15f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(-56, -26, 107);
		sample.speed = 0.25f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(-63, -34, 102);
		sample.speed = 0.15f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(-77, -40, 93);
		sample.speed = 0.25f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(-92, -45, 85);
		sample.speed = 0.15f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(-89.5, -39, 87.7);
		sample.speed = 0.15f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(-111, -39, 31);
		sample.speed = 0.35f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(-19.2, -14, 53);
		sample.speed = 0.3f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(24, -14.7, 25.5);
		sample.speed = 0.25f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(45.5, 7.5, -4.6);
		sample.speed = 0.2f;
		curveSamples.push_back(sample);
		sample.point = glm::vec3(23, 6.4, -16.5);
		sample.speed = 0.15f;
		curveSamples.push_back(sample);

		std::shared_ptr<G2::Curve> curve = std::shared_ptr<G2::Curve>(new G2::CatmullRomCurve(G2::InterpolationDescription(), curveSamples));

		auto* animation = mCamera.addComponent<G2::SplineAnimation>(curve);
		mCameraAnimation = true;
	}


	if (key == G2::KC_W)
	{
		G2::CurveSample sample;
		sample.point = mCamera.getComponent<G2::TransformComponent>()->getPosition();
		sample.speed = 0.15f;
		mCameraSamples.push_back(sample);
		G2::logger << mCamera.getComponent<G2::TransformComponent>()->getPosition() << G2::endl;
	}
	if (key == G2::KC_D)
	{
		for (int i = 0; i < mCameraSamples.size(); ++i)
		{
			G2::logger << mCameraSamples[i].point << G2::endl;
		}
	}
}