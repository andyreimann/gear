#include "TerrainTest.h"

#include <G2Terrains/ClipmapTerrain.h>
#include <G2Terrains/RoamTerrain.h>

static std::string ASSET_PATH = "../../Assets/";

TerrainTest::TerrainTest(G2::SDL::Window& window)
	: mExitRendering(false),
	mWindow(window),
	mEditorCamera(&window),
	mEditor(&window, ASSET_PATH + "G2Editor/")/*,
	mTerrain(mTexImporter.import(ASSET_PATH + "Resources/heightmap.jpg", GL_NEAREST, GL_NEAREST))*/
{
	mEditorCamera
		.pan(0.f,10.f)
		.rotate(25.f, 0.f)
		.moveView(-15.f)
		.getComponent<G2::CameraComponent>()->setAsRenderCamera();
	mEditorCamera.setInternals(70.f,0.5f,100000.f);
	
	// new way of loading shader
	std::shared_ptr<G2::Effect> effect = mEffectImporter.import(ASSET_PATH + "Shader/Default.g2fx");
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem,G2::RenderComponent>()
		->setDefaultEffect(effect);
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem,G2::RenderComponent>()
		->setClearColor(glm::vec4(0.f,0.f,0.f,1.f));

	
	G2::EventDistributer::onRenderFrame.hook(this, &TerrainTest::onRenderFrame);
	G2::EventDistributer::onViewportResize.hook(this, &TerrainTest::onViewportResize);
	G2::EventDistributer::onKeyUp.hook(this, &TerrainTest::onKeyUp);
	G2::EventDistributer::onKeyDown.hook(this, &TerrainTest::onKeyDown);
	G2::EventDistributer::onMouseMove.hook(this, &TerrainTest::onMouseMove);
	G2::EventDistributer::onMouseDown.hook(this, &TerrainTest::onMouseDown);

	mLight = mFbxImporter.import(ASSET_PATH + "Resources/unit-sphere.fbx");

	
	auto* light = mLight->addComponent<G2::LightComponent>(G2::LightType::DIRECTIONAL);
	
	light->configureShadows(G2::ShadowDescriptor::cascadedShadowMaps(3,ASSET_PATH + "Shader/CSM.g2fx"));

	light->getType();
	light->diffuse = glm::vec4(0.3,0.6,0.f,0.f);
	light->specular = glm::vec4(1.f,1.f,1.f,0.f);
	light->linearAttenuation = 1.f;
	 
	auto* lightTransformation = mLight->addComponent<G2::TransformComponent>();
	lightTransformation->rotateAxis(-90.0f, glm::vec3(1.f,0.f,0.f));

	//mEditor.start();

	//auto* terrain = mTerrainComponent.addComponent<G2::Terrain::ClipmapTerrain>();
	//terrain->setup(
	//	mTexImporter.import(ASSET_PATH + "Resources/heightmap.jpg", GL_LINEAR, GL_LINEAR),
	//	mEffectImporter.import(ASSET_PATH + "Shader/ClipmapTerrain.g2fx"));

	auto* terrain = mTerrainComponent.addComponent<G2::Terrain::RoamTerrain>(
		mTexImporter.import(ASSET_PATH + "Resources/heightmap2048.png", GL_LINEAR, GL_LINEAR),
		mEffectImporter.import(ASSET_PATH + "Shader/Simple.g2fx"),
		1000000
	);
}


TerrainTest::~TerrainTest(void)
{
	G2::EventDistributer::onRenderFrame.unHook(this, &TerrainTest::onRenderFrame);
	G2::EventDistributer::onViewportResize.unHook(this, &TerrainTest::onViewportResize);
	G2::EventDistributer::onKeyUp.unHook(this, &TerrainTest::onKeyUp);
	G2::EventDistributer::onKeyDown.unHook(this, &TerrainTest::onKeyDown);
	G2::EventDistributer::onMouseMove.unHook(this, &TerrainTest::onMouseMove);
	G2::EventDistributer::onMouseDown.unHook(this, &TerrainTest::onMouseDown);
}

void
TerrainTest::onViewportResize(int width, int height) 
{
	mEditorCamera.setViewport(width, height);
}

void
TerrainTest::onKeyUp(G2::KeyCode keyCode) 
{

}

void
TerrainTest::onKeyDown(G2::KeyCode keyCode) 
{

}

void
TerrainTest::onMouseDown(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords) 
{

}

void
TerrainTest::onMouseMove(glm::detail::tvec2<int> const& mouseCoords) 
{

}

void
TerrainTest::onRenderFrame(G2::FrameInfo const&) 
{

}