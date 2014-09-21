#include "Scene.h"
#include <G2/AbstractWindow.h>

static std::string ASSET_PATH = "../../Assets/";

Scene::Scene(G2::AbstractWindow* window) :
	mWindow(window),
	mEditorCamera(window),
	mExitRendering(false)
{
	mEditorCamera
		.pan(0.f,10.f)
		.rotate(25.f, 0.f)
		.moveView(-15.f)
		.getComponent<G2::CameraComponent>()->setAsRenderCamera();
	
	// new way of loading shader
	std::shared_ptr<G2::Effect> effect = mEffectImporter.import(ASSET_PATH + "Shader/Default_CG.g2fx");
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem,G2::RenderComponent>()
		->setDefaultEffect(effect);
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem,G2::RenderComponent>()
		->setClearColor(glm::vec4(0.0f,0.3f,0.7f,1.0f));

	
	G2::EventDistributer::onRenderFrame.hook(this, &Scene::onRenderFrame);
	G2::EventDistributer::onViewportResize.hook(this, &Scene::onViewportResize);
	G2::EventDistributer::onKeyUp.hook(this, &Scene::onKeyUp);
	G2::EventDistributer::onKeyDown.hook(this, &Scene::onKeyDown);
	G2::EventDistributer::onMouseMove.hook(this, &Scene::onMouseMove);
	G2::EventDistributer::onMouseDown.hook(this, &Scene::onMouseDown);
	
	mMeshes.push_back(mMeshImporter.import(ASSET_PATH + "Resources/monkey.fbx"));
	auto* transformation = mMeshes.back()->addComponent<G2::TransformComponent>();
	transformation->setScale(glm::vec3(1.f, 1.f, 1.f));
	transformation->setPosition(glm::vec3(0.f, -15.f, 0.f));
	//transformation->rotateX(45.f);
	transformation->updateWorldSpaceMatrix(0);
	auto* renderComp = mMeshes.back()->addComponent<G2::RenderComponent>();
	renderComp->material.setSpecular(glm::vec4(0.f,1.f,0.f,1.f));
	renderComp->material.setShininess(128.f);
	renderComp->material.setAmbient(glm::vec4(0.2f,0.2f,0.13f,0.3));
	renderComp->material.setDiffuse(glm::vec4(1.f,0.23f,0.f,0.3));
	//renderComp->setEffect(mEffectImporter.import(ASSET_PATH + "Shader/Glass.g2fx"));
}

Scene::~Scene(void)
{
	mEditorCamera.~EditorCamera();
	G2::EventDistributer::onRenderFrame.unHook(this, &Scene::onRenderFrame);
	G2::EventDistributer::onViewportResize.unHook(this, &Scene::onViewportResize);
	G2::EventDistributer::onKeyUp.unHook(this, &Scene::onKeyUp);
	G2::EventDistributer::onKeyDown.unHook(this, &Scene::onKeyDown);
	G2::EventDistributer::onMouseMove.unHook(this, &Scene::onMouseMove);
	G2::EventDistributer::onMouseDown.unHook(this, &Scene::onMouseDown);
}
void 
Scene::onKeyUp(G2::KeyCode keyCode)
{
	std::cout << "KEY: " << keyCode << std::endl;
}

void 
Scene::onKeyDown(G2::KeyCode keyCode) {
	
	if(keyCode == G2::KC_ESCAPE)
	{ 
		mExitRendering = true; 
	}
}

void
Scene::onViewportResize(int width, int height) 
{
	mEditorCamera.setViewport(width, height);
}

void 
Scene::onMouseMove(glm::detail::tvec2<int> const& mouseCoords)
{

}

void 
Scene::onMouseDown(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords)
{

}

void
Scene::onRenderFrame(G2::FrameInfo const& frameInfo) 
{
	// set exit state - the only mutable property!
	frameInfo.stopRenderingAfterThisFrame = mExitRendering;
}