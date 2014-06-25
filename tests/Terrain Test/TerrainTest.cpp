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
	light->diffuse = glm::vec4(1.f,1.f,1.f,1.f);
	//light->specular = glm::vec4(1.f,1.f,1.f,0.f);
	light->linearAttenuation = 1.f;
	 
	auto* lightTransformation = mLight->addComponent<G2::TransformComponent>();
	lightTransformation->rotateAxis(-10.0f, glm::vec3(1.f,0.f,0.f));

	mEditor.start();

	//auto* terrain = mTerrainComponent.addComponent<G2::Terrain::ClipmapTerrain>();
	//terrain->setup(
	//	mTexImporter.import(ASSET_PATH + "Resources/heightmap.jpg", GL_LINEAR, GL_LINEAR),
	//	mEffectImporter.import(ASSET_PATH + "Shader/ClipmapTerrain.g2fx"));

	auto* terrain = mTerrainComponent.addComponent<G2::Terrain::RoamTerrain>();
	terrain->setup(
		mTexImporter.import(ASSET_PATH + "Resources/heightmap512.png", GL_LINEAR, GL_LINEAR),
		15.f, // max height
		50000, // desired num triangles
		100000 // max number of triangles
	);
	auto* renderComponent = mTerrainComponent.getComponent<G2::RenderComponent>();
	renderComponent->material.setTexture(
		G2::Sampler::DIFFUSE, 
		mTexImporter.import(ASSET_PATH + "Resources/launch-button.jpg", GL_LINEAR, GL_LINEAR)
	);
	renderComponent->setEffect(mEffectImporter.import(ASSET_PATH + "Shader/DefaultTex.g2fx"));

	generateSpline();
}

#include <glm/gtx/spline.hpp>

void
TerrainTest::generateSpline() 
{
	// TEST build line strip from sampling a list of heights

	glm::vec3 heights[6] = { 
		glm::vec3(0.f,0.f,0.f),
		glm::vec3(-0.2f,1.f,0.f),
		glm::vec3(-0.4f,0.f,0.f),
		glm::vec3(-0.6f,1.f,-1.f),
		glm::vec3(-0.8f,0.f,0.f),
		glm::vec3(-1.f,1.f,0.f)
	};
	int size = sizeof(heights) / sizeof(heights[0]);

	float sampling = 0.1f;

	std::vector<glm::vec3> geometry;
	// add first point as exact point
	geometry.push_back(heights[0]);

	for(int i = 0; i < size-1; ++i)
	{
		int i0 = std::max(0,i-1);
		int i3 = std::min(i+2,size-2);
		for(int k = 0; k < int(1.f / sampling); ++k)
		{
			glm::vec3 samplingPoint = glm::catmullRom(heights[i0], heights[i], heights[i+1], heights[i3], k*sampling);
			geometry.push_back(samplingPoint);
		}
	}
	// add last point as exact point
	geometry.push_back(heights[size-1]);

	// add a test vertex array

	auto* renderComponent = mSpline.addComponent<G2::RenderComponent>();
	renderComponent->drawMode = GL_LINE_STRIP;
	renderComponent->allocateVertexArrays(1);
	auto& vao = renderComponent->getVertexArray(0);
	vao.resizeElementCount(geometry.size());
	vao.writeData(G2::Semantics::POSITION, &geometry[0]);
	auto* renderComponent2 = mSplinePoints.addComponent<G2::RenderComponent>();
	renderComponent2->drawMode = GL_POINTS;
	renderComponent2->allocateVertexArrays(1);
	auto& vao2 = renderComponent2->getVertexArray(0);
	vao2.resizeElementCount(size);
	vao2.writeData(G2::Semantics::POSITION, &heights[0]);
	renderComponent2->material.setAmbient(glm::vec4(1.f,0.f,0.f,1.f));
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