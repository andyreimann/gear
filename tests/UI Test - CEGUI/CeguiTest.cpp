#include "CeguiTest.h"
#include <G2/TransformComponent.h>
#include <G2/FBXAnimationComponent.h>

#include <G2/MatrixMultiplicator.h>
#include <G2/Logger.h>
#include <G2/TimeTracker.h>
#include <G2/Effect.h>

#include <G2Cegui/CeguiComponent.h>

static std::string ASSET_PATH = "../../Assets/";

CeguiTest::CeguiTest(G2::SDLWindow& window)
	: mExitRendering(false),
	mWindow(window),
	mEditorCamera(&window)
{
	srand(2006);

	mEditorCamera
		.pan(0.f,10.f)
		.rotate(25.f, 0.f)
		.moveView(-15.f);

	// new way of loading shader
	G2::logger << "IMPORTING DEFAULT SHADER" << G2::endl;
	std::shared_ptr<G2::Effect> effect = mEffectImporter.import(ASSET_PATH + "Shader/Default.g2fx");
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem,G2::RenderComponent>()
		->setDefaultEffect(effect);
	G2::logger << "IMPORTING DEFAULT SHADER DONE" << G2::endl;


	G2::EventDistributer::onRenderFrame.hook(this, &CeguiTest::onRenderFrame);
	G2::EventDistributer::onViewportResize.hook(this, &CeguiTest::onViewportResize);
	G2::EventDistributer::onKeyUp.hook(this, &CeguiTest::onKeyUp);
	G2::EventDistributer::onKeyDown.hook(this, &CeguiTest::onKeyDown);
	G2::EventDistributer::onMouseMove.hook(this, &CeguiTest::onMouseMove);
	G2::EventDistributer::onMouseDown.hook(this, &CeguiTest::onMouseDown);
	
	mLight = mFBXImporter.import(ASSET_PATH + "Resources/unit-sphere.fbx");

	auto* light = mLight->addComponent<G2::LightComponent>(G2::LightType::POSITIONAL);
	mLight->addComponent<G2::RenderComponent>();

	light->diffuse = glm::vec4(0.3,0.6,0.f,0.f);
	light->specular = glm::vec4(1.f,1.f,1.f,0.f);
	light->linearAttenuation = 1.f;
	
	auto* lightTransformation = mLight->addComponent<G2::TransformComponent>();
	lightTransformation->setPosition(glm::vec3(0.123f,0.7f,1.0123f));

	createWalls();

	//bool changed = G2::ECSManager::getShared().updateInvokeOrder<G2::UI::CeguiSystem,G2::UI::CeguiComponent,G2::RenderSystem,G2::RenderComponent>();

	auto uiComponent = mLight->addComponent<G2::UI::CeguiComponent>();
}

CeguiTest::~CeguiTest()
{
	mEditorCamera.~EditorCamera();
	G2::EventDistributer::onRenderFrame.unHook(this, &CeguiTest::onRenderFrame);
	G2::EventDistributer::onViewportResize.unHook(this, &CeguiTest::onViewportResize);
	G2::EventDistributer::onKeyUp.unHook(this, &CeguiTest::onKeyUp);
	G2::EventDistributer::onKeyDown.unHook(this, &CeguiTest::onKeyDown);
	G2::EventDistributer::onMouseMove.unHook(this, &CeguiTest::onMouseMove);
	G2::EventDistributer::onMouseDown.unHook(this, &CeguiTest::onMouseDown);
}
void 
CeguiTest::createWalls()
{
	float rot = 5.f;

	mWalls.push_back(mFBXImporter.import(ASSET_PATH + "Resources/unit-cube.fbx"));
	auto* transformation = mWalls.back()->addComponent<G2::TransformComponent>();
	transformation->setScale(glm::vec3(5.f, 0.5f, 5.f));
	transformation->setPosition(glm::vec3(0.f, -20.f, 0.f));
	//transformation->rotateX(45.f);
	transformation->updateWorldSpaceMatrix(0);
	auto* renderComp = mWalls.back()->addComponent<G2::RenderComponent>();
	renderComp->material.setSpecular(glm::vec4(1.f,0.f,0.f,1.f));
	renderComp->material.setShininess(128.f);
	renderComp->material.setAmbient(glm::vec4(0.5f,0.f,0.f,1.f));
	renderComp->material.setDiffuse(glm::vec4(1.f,1.f,1.f,1.f));
}

void 
CeguiTest::onKeyUp(G2::KeyCode keyCode)
{
	
}

void 
CeguiTest::onKeyDown(G2::KeyCode keyCode) {
	
	if(keyCode == G2::KC_ESCAPE)
	{ 
		mExitRendering = true; 
	}
	else if(keyCode == G2::KC_UP)
	{
		auto* trans = mLight->getComponent<G2::TransformComponent>();
		trans->translate(glm::vec3(0.f,0.f,-0.1f));
	}
	else if(keyCode == G2::KC_DOWN) 
	{ 
		auto* trans = mLight->getComponent<G2::TransformComponent>();
		trans->translate(glm::vec3(0.f,0.f,0.1f));
	}
	else if(keyCode == G2::KC_LEFT) 
	{ 
		auto* trans = mLight->getComponent<G2::TransformComponent>();
		trans->translate(glm::vec3(-0.1f,0.f,0.f));
	}
	else if(keyCode == G2::KC_RIGHT)
	{ 
		auto* trans = mLight->getComponent<G2::TransformComponent>();
		trans->translate(glm::vec3(0.1f,0.f,0.f));
	}
}

void
CeguiTest::onViewportResize(int width, int height) 
{
	mEditorCamera.setViewport(width, height);
}

void 
CeguiTest::onMouseMove(glm::detail::tvec2<int> const& mouseCoords)
{

}

void
CeguiTest::onRenderFrame(G2::FrameInfo const& frameInfo) 
{
	// set exit state - the only mutable property!
	frameInfo.stopRenderingAfterThisFrame = mExitRendering;
}

void
CeguiTest::onMouseDown(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords) 
{
}