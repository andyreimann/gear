#include "PhyTestNewton.h"
#include <G2/TransformComponent.h>
#include <G2/FBXAnimationComponent.h>

#include <G2/MatrixMultiplicator.h>
#include <G2/Logger.h>
#include <G2/TimeTracker.h>
#include <G2/Effect.h>

#include <G2NewtonPhysics/Defines.h>

static std::string ASSET_PATH = "../../Assets/";

PhyTestNewton::PhyTestNewton(G2::SDLWindow& window)
	: mExitRendering(false),
	mWindow(window),
	mEditorCamera(&window),
	mPhysicsSystem(G2::ECSManager::getShared().getSystem<G2::Physics::PhysicsSystem,G2::Physics::PhysicsComponent>())
{
	srand(2006);

	mEditorCamera
		.pan(0.f,10.f)
		.rotate(25.f, 0.f)
		.moveView(-15.f)
		.getComponent<G2::CameraComponent>()->setAsRenderCamera();

	// new way of loading shader
	std::shared_ptr<G2::Effect> effect = mEffectImporter.import(ASSET_PATH + "Shader/Default.g2fx");
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem,G2::RenderComponent>()
		->setDefaultEffect(effect);

	/*auto* renderComponent = mCube.addComponent<G2::RenderComponent>();

	float s = 0.5f;
	mCube = G2::AABB(glm::vec3(-s,-s,-s), glm::vec3(s,s,s));
	mCube.enableRendering();
	*/
	G2::EventDistributer::onRenderFrame.hook(this, &PhyTestNewton::onRenderFrame);
	G2::EventDistributer::onViewportResize.hook(this, &PhyTestNewton::onViewportResize);
	G2::EventDistributer::onKeyUp.hook(this, &PhyTestNewton::onKeyUp);
	G2::EventDistributer::onKeyDown.hook(this, &PhyTestNewton::onKeyDown);
	G2::EventDistributer::onMouseMove.hook(this, &PhyTestNewton::onMouseMove);
	G2::EventDistributer::onMouseDown.hook(this, &PhyTestNewton::onMouseDown);
	
	//createPlane(glm::vec4(0.f,0.f,0.f,0.0), mTexImporter.import(ASSET_PATH + "Resources/launch-button.jpg", G2::NEAREST, G2::NEAREST, false));
	//createPlane(glm::vec4(1.0,0.0,0.0,0.0), mTexImporter.import(RESOURCE_PATH + "Resources/launch-button.jpg", G2::NEAREST, G2::NEAREST, true));
	//createPlane(glm::vec4(2.0,0.0,0.0,0.0), mTexImporter.import(RESOURCE_PATH + "Resources/launch-button.jpg", G2::NEAREST, G2::LINEAR, false));
	//createPlane(glm::vec4(3.0,0.0,0.0,0.0), mTexImporter.import(RESOURCE_PATH + "Resources/launch-button.jpg", G2::LINEAR, G2::LINEAR, false));
	//createPlane(glm::vec4(4.0,0.0,0.0,0.0), mTexImporter.import(RESOURCE_PATH + "Resources/launch-button.jpg", G2::LINEAR, G2::NEAREST, false));
	
	mLight = mFBXImporter.import(ASSET_PATH + "Resources/unit-sphere.fbx");

	
	auto* light = mLight->addComponent<G2::LightComponent>(G2::LightType::DIRECTIONAL);
	light->configureShadows(G2::ShadowDescriptor::cascadedShadowMaps(3,ASSET_PATH + "Shader/CSM.g2fx"));

	light->diffuse = glm::vec4(0.3,0.6,0.f,0.f);
	light->specular = glm::vec4(1.f,1.f,1.f,0.f);
	light->linearAttenuation = 1.f;
	
	auto* physics = mLight->addComponent<G2::Physics::PhysicsComponent>();
	physics->add(
		G2::Physics::CollisionShapeDescriptor::sphere(0.1), 
		G2::Physics::RigidBodyDescriptor()
			.setMass(10.f)
			.setMaterial(mPhysicsSystem->createMaterial())
	);
	physics->addCollisionListener(this, &PhyTestNewton::onLightCollide);
	
	auto* lightTransformation = mLight->addComponent<G2::TransformComponent>();
	lightTransformation->setPosition(glm::vec3(0.123f,0.7f,1.0123f));

	//mFBXImporter.setCachingMode(false);
	for(int i = 0; i < 3; ++i)
	{
		
		for(int x = 0; x < 10; ++x)
		{
		
			for(int y = 0; y < 10; ++y)
			{
				float dx = (rand() % 1000) / 10000.f;
				float dz = (rand() % 1000) / 10000.f;
				mFbxMeshes.push_back(mFBXImporter.import(ASSET_PATH + "Resources/unit-sphere.fbx"));
				auto* renderComp = mFbxMeshes.back()->getComponent<G2::RenderComponent>();
				physics = mFbxMeshes.back()->addComponent<G2::Physics::PhysicsComponent>();
				physics->add(
					G2::Physics::CollisionShapeDescriptor::sphere(0.1), 
					G2::Physics::RigidBodyDescriptor()
						.setMass(0.1f),
					glm::vec3(-4.0 + (x * 0.8f) + dx, 100 + i, -4.0 + (y * 0.8f) + dz)
				);
				renderComp->material.setSpecular(glm::vec4(1.f,1.f,1.f,1.f));
				renderComp->material.setShininess(16.f);
				renderComp->material.setAmbient(glm::vec4(0.5f,0.f,0.5f,1.f));
				renderComp->material.setDiffuse(glm::vec4(1.f,0.5f,1.f,1.f));
			}
		}
	}

	createWalls();

	bool changed = G2::ECSManager::getShared().updateInvokeOrder<G2::TransformSystem,G2::TransformComponent,G2::RenderSystem,G2::RenderComponent>();
	G2::logger << "Changed 1: " << changed << G2::endl;
	changed = G2::ECSManager::getShared().updateInvokeOrder<G2::RenderSystem,G2::RenderComponent,G2::TransformSystem,G2::TransformComponent>();
	G2::logger << "Changed 2: " << changed << G2::endl;
	changed = G2::ECSManager::getShared().updateInvokeOrder<G2::RenderSystem,G2::RenderComponent,G2::TransformSystem,G2::TransformComponent>();
	G2::logger << "Changed 3 (should not): " << changed << G2::endl;
	//changed = G2::ECSManager::getShared().updateInvokeOrder<G2::TransformSystem,G2::TransformComponent,G2::RenderSystem,G2::RenderComponent>();
	//G2::logger << "Changed 4 (should): " << changed << G2::endl;
}

void
PhyTestNewton::onLightCollide(G2::Physics::Collision const& collision) 
{
	//G2::logger << "Got collision in App at " << collision.position << G2::endl;
}

PhyTestNewton::~PhyTestNewton()
{
	mEditorCamera.~EditorCamera();
	G2::EventDistributer::onRenderFrame.unHook(this, &PhyTestNewton::onRenderFrame);
	G2::EventDistributer::onViewportResize.unHook(this, &PhyTestNewton::onViewportResize);
	G2::EventDistributer::onKeyUp.unHook(this, &PhyTestNewton::onKeyUp);
	G2::EventDistributer::onKeyDown.unHook(this, &PhyTestNewton::onKeyDown);
	G2::EventDistributer::onMouseMove.unHook(this, &PhyTestNewton::onMouseMove);
	G2::EventDistributer::onMouseDown.unHook(this, &PhyTestNewton::onMouseDown);
}
void 
PhyTestNewton::createWalls()
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
	auto* physics = mWalls.back()->addComponent<G2::Physics::PhysicsComponent>();
	physics->add(
		G2::Physics::CollisionShapeDescriptor::box(glm::vec3(5.f, 0.5f, 5.f)),
		G2::Physics::RigidBodyDescriptor()
			.setMass(0.f)
			.setMaterial(mPhysicsSystem->createMaterial()),
		glm::vec3(0.f, -20.f, 0.f)
	);
	
	G2::Physics::PhysicsMaterialInteraction& material = mPhysicsSystem->getMaterialInteraction(physics->getMaterial(0), mLight->getComponent<G2::Physics::PhysicsComponent>()->getMaterial(0));
	material.restitution = 0.7f;
	material.staticFriction = 0.75f;
	material.kineticFriction = 0.75f;
	

	mLight->addComponent<G2::Physics::PhysicsComponent>();
}

void 
PhyTestNewton::onKeyUp(G2::KeyCode keyCode)
{
	
}

void 
PhyTestNewton::onKeyDown(G2::KeyCode keyCode) {
	
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
PhyTestNewton::onViewportResize(int width, int height) 
{
	mEditorCamera.setViewport(width, height);
}

void 
PhyTestNewton::onMouseMove(glm::detail::tvec2<int> const& mouseCoords)
{

}

void
PhyTestNewton::onRenderFrame(G2::FrameInfo const& frameInfo) 
{
	// set exit state - the only mutable property!
	frameInfo.stopRenderingAfterThisFrame = mExitRendering;
}

void
PhyTestNewton::onMouseDown(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords) 
{
	if(button == G2::MOUSE_LEFT)
	{
		auto* transformComp = mEditorCamera.getComponent<G2::TransformComponent>();
		glm::vec4 pos = glm::inverse(transformComp->getWorldSpaceMatrix()) * glm::vec4(0.f,0.f,0.f,1.f);
		
		mFbxMeshes.push_back(mFBXImporter.import(ASSET_PATH + "Resources/unit-sphere.fbx"));
		auto* renderComp = mFbxMeshes.back()->getComponent<G2::RenderComponent>();
		auto* physics = mFbxMeshes.back()->addComponent<G2::Physics::PhysicsComponent>();
		physics->add(
			G2::Physics::CollisionShapeDescriptor::sphere(0.1), 
			G2::Physics::RigidBodyDescriptor()
				.setMass(0.1f)
				.setLinearVelocity(glm::vec3(mEditorCamera.getViewVec()) * 10.f),
			glm::vec3(pos)
		);//->trackAABBCollisions(true);
		renderComp->material.setSpecular(glm::vec4(1.f,1.f,1.f,1.f));
		renderComp->material.setShininess(16.f);
		renderComp->material.setAmbient(glm::vec4(0.5f,0.f,0.5f,1.f));
		renderComp->material.setDiffuse(glm::vec4(1.f,0.5f,1.f,1.f));
	}
}