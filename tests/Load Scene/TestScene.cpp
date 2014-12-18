#include "TestScene.h"
#include <G2/TransformComponent.h>
#include <G2/FBXAnimationComponent.h>

#include <G2/MatrixMultiplicator.h>
#include <G2/Logger.h>
#include <G2/TimeTracker.h>
#include <G2/Effect.h>
#include <G2BulletPhysics/PhysicsComponent.h>

static std::string ASSET_PATH = "../../Assets/";

TestScene::TestScene(G2::SDL::Window& window)
	: mExitRendering(false),
	mWindow(window),
	mEditorCamera(&window),
	mEditor(&window, ASSET_PATH + "G2Editor/"),
	mEditorOn(false)
{
	srand(2006);

	mEditorCamera
		.translate(0.f,10.f)
		.rotate(25.f, 0.f)
		.zoom(-15.f)
		.getComponent<G2::CameraComponent>()->setAsRenderCamera();

	// new way of loading shader
	std::shared_ptr<G2::Effect> effect = mEffectImporter.import(ASSET_PATH + "Shader/Default_CG.g2fx");
	//std::shared_ptr<G2::Effect> effect = mEffectImporter.import(ASSET_PATH + "Shader/EffectFileImporterTest.g2fx");
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem,G2::RenderComponent>()
		->setDefaultEffect(effect);

	/*auto* renderComponent = mCube.addComponent<G2::RenderComponent>();

	float s = 0.5f;
	mCube = G2::AABB(glm::vec3(-s,-s,-s), glm::vec3(s,s,s));
	mCube.enableRendering();
	*/
	G2::EventDistributer::onRenderFrame.hook(this, &TestScene::onRenderFrame);
	G2::EventDistributer::onViewportResize.hook(this, &TestScene::onViewportResize);
	G2::EventDistributer::onKeyUp.hook(this, &TestScene::onKeyUp);
	G2::EventDistributer::onKeyDown.hook(this, &TestScene::onKeyDown);
	G2::EventDistributer::onMouseMove.hook(this, &TestScene::onMouseMove);
	G2::EventDistributer::onMouseDown.hook(this, &TestScene::onMouseDown);
	
	//createPlane(glm::vec4(0.f,0.f,0.f,0.0), mTexImporter.import(ASSET_PATH + "Resources/launch-button.jpg", G2::NEAREST, G2::NEAREST, false));
	//createPlane(glm::vec4(1.0,0.0,0.0,0.0), mTexImporter.import(RESOURCE_PATH + "Resources/launch-button.jpg", G2::NEAREST, G2::NEAREST, true));
	//createPlane(glm::vec4(2.0,0.0,0.0,0.0), mTexImporter.import(RESOURCE_PATH + "Resources/launch-button.jpg", G2::NEAREST, G2::LINEAR, false));
	//createPlane(glm::vec4(3.0,0.0,0.0,0.0), mTexImporter.import(RESOURCE_PATH + "Resources/launch-button.jpg", G2::LINEAR, G2::LINEAR, false));
	//createPlane(glm::vec4(4.0,0.0,0.0,0.0), mTexImporter.import(RESOURCE_PATH + "Resources/launch-button.jpg", G2::LINEAR, G2::NEAREST, false));
	
	createWalls();

	mLight = mMeshImporter2.import(ASSET_PATH + "Resources/unit-sphere.fbx");

	
	auto* light = mLight.addComponent<G2::LightComponent>(G2::LightType::DIRECTIONAL);
	
	//light->configureShadows(G2::ShadowDescriptor::cascadedShadowMaps(3,ASSET_PATH + "Shader/CSM.g2fx"));

	mLightType = light->getType();
	light->diffuse = glm::vec4(0.3,0.6,0.f,0.f);
	light->specular = glm::vec4(1.f,1.f,1.f,0.f);
	light->linearAttenuation = 1.f;
	
	//auto* physics = mLight->addComponent<G2::Physics::PhysicsComponent>(
	//	G2::Physics::CollisionShapeDescriptor::sphere(0.1), 
	//	G2::Physics::RigidBodyDescriptor().setMass(10.f)
	//					.setFriction(0.75f)
	//					.setRestitution(0.5f)
	//);
	
	auto* lightTransformation = mLight.addComponent<G2::TransformComponent>();
	//lightTransformation->setPosition(glm::vec3(0.123f,0.7f,1.0123f));
	lightTransformation->rotateAxis(-90.0f, glm::vec3(1.f,0.f,0.f));
	//lightTransformation->setScale(glm::vec3(0.1f,0.1f,0.1f));
	//lightTransformation->setRotation(glm::angleAxis(30.0f, glm::vec3(1.f,0.f,0.f)));

	std::vector<std::string> animFiles;
	animFiles.push_back(ASSET_PATH + "Resources/boblampclean.md5anim");

	//mSampleMesh = mMeshImporter.import(ASSET_PATH + "Resources/boblampclean.md5mesh", animFiles);
	

	auto* renderComp = mSampleMesh.addComponent<G2::RenderComponent>();
	renderComp->material.setShininess(128.f);
	auto* transformComponent = mSampleMesh.addComponent<G2::TransformComponent>();
	transformComponent->setScale(glm::vec3(0.01f,0.01f,0.01f));
	
	glm::quat rot = glm::angleAxis(-90.0f, glm::vec3(1.f,0.f,0.f));

	//transformComponent->setRotation(rot);
	//transformComponent->setPosition(glm::vec3(0,1,0));
	
	
	for (int k = 0; k < 0; ++k) 
	{ 
		for (int i = 0; i < 1; ++i) 
		{
			mFbxMeshes.push_back(mMeshImporter2.import(ASSET_PATH + "Resources/humanoid.fbx"));
				auto* transformComponent = mFbxMeshes.back().addComponent<G2::TransformComponent>();
				transformComponent->setScale(glm::vec3(0.01f,0.01f,0.01f));
				transformComponent->setPosition(glm::vec3(i*0.6,0,k*0.3));
		}
	}
	
	//mSampleMesh3 = mMeshImporter2.import("../../Resources/humanoid.fbx");
	//if(mSampleMesh3.get()) 
	//{
	//	auto* transformComponent = mSampleMesh3.addComponent<G2::TransformComponent>();
	//	transformComponent->setScale(glm::vec3(0.01f,0.01f,0.01f));
	//	transformComponent->setPosition(glm::vec3(1,0,0));
	//	auto* animComponent = mSampleMesh3.getComponent<G2::FBXAnimationComponent>();
	//	animComponent->tempSetPoseIndex(0);
	//}

	for(int i = 0; i < 0; ++i)
	{
		
		for(int x = 0; x < 5; ++x)
		{
		
			for(int y = 0; y < 5; ++y)
			{
				
				mBalls.push_back(mMeshImporter2.import(ASSET_PATH + "Resources/unit-sphere.fbx"));
				auto* renderComp = mBalls.back().getComponent<G2::RenderComponent>();
				mBalls.back().addComponent<G2::Physics::PhysicsComponent>(
					G2::Physics::CollisionShapeDescriptor::sphere(0.1), 
					G2::Physics::RigidBodyDescriptor()
						.setMass(0.1f)
						.setFriction(0.75f)
						.setRestitution(0.5f),
					glm::vec3(-4.0 + (x * 0.8f), 100 + i, -4.0 + (y * 0.8f))
				);//->trackAABBCollisions(true);
				renderComp->material.setSpecular(glm::vec4(1.f,1.f,1.f,1.f));
				renderComp->material.setShininess(16.f);
				renderComp->material.setAmbient(glm::vec4(0.5f,0.f,0.5f,1.f));
				renderComp->material.setDiffuse(glm::vec4(1.f,0.5f,1.f,1.f));
			}
	
		}
	
	}
	//createWaterSurface();
	
	// setup the post processing pipeline
	//std::shared_ptr<G2::Effect> postProcess = mEffectImporter.import(ASSET_PATH + "Shader/BlurShader.g2fx");
	//G2::ECSManager::getShared()
	//	.getSystem<G2::RenderSystem,G2::RenderComponent>()
	//	->addPostProcessingEffect(postProcess);
	//postProcess = mEffectImporter.import(ASSET_PATH + "Shader/RedDistortion.g2fx");
	//G2::ECSManager::getShared()
	//	.getSystem<G2::RenderSystem,G2::RenderComponent>()
	//	->addPostProcessingEffect(postProcess);

}

TestScene::~TestScene()
{
	mEditorCamera.~EditorCamera();
	G2::EventDistributer::onRenderFrame.unHook(this, &TestScene::onRenderFrame);
	G2::EventDistributer::onViewportResize.unHook(this, &TestScene::onViewportResize);
	G2::EventDistributer::onKeyUp.unHook(this, &TestScene::onKeyUp);
	G2::EventDistributer::onKeyDown.unHook(this, &TestScene::onKeyDown);
	G2::EventDistributer::onMouseMove.unHook(this, &TestScene::onMouseMove);
	G2::EventDistributer::onMouseDown.unHook(this, &TestScene::onMouseDown);
}

void
TestScene::createPlane(glm::vec4 const& corner, std::shared_ptr<G2::Texture2D> const& diffuseTex, glm::quat const& rot) 
{
	//mPlanes.push_back(GameObject());
	//auto* plane = mPlanes.back().addComponent<G2::RenderComponent>();
	//plane->allocateVertexArrays(1);
	//plane->drawMode = GL_TRIANGLES;
	//// import and assign a texture
	//plane->material.setTexture(G2::Sampler::NORMAL, diffuseTex); // only rendered/used in passes

	//// prepare vao
	//// assign vao to RenderComponent using move semantic
	//G2::VertexArrayObject& vao = plane->getVertexArray(0);

	//vao.resizeElementCount(4);

	//glm::vec4 planeGeometry[4];
	//planeGeometry[0] = corner + glm::vec4(-5.f,0.f,-5.f,1.f);
	//planeGeometry[1] = corner + glm::vec4(5.f,0.f,-5.f,1.f);
	//planeGeometry[2] = corner + glm::vec4(5.f,0.f,5.f,1.f);
	//planeGeometry[3] = corner + glm::vec4(-5.f,0.f,5.f,1.f);

	//vao.writeData(G2::Semantics::POSITION, planeGeometry);

	//// build tex coordinates
	//glm::vec2 tex[6];
	//tex[0] = glm::vec2(0.f,0.f);
	//tex[1] = glm::vec2(0.f,1.f);
	//tex[2] = glm::vec2(1.f,1.f);
	//tex[3] = glm::vec2(1.f,0.f);
	//vao.writeData(G2::Semantics::TEXCOORD_0, tex);

	//// build indices - planes were rendered using indices as soon as we add some to the vao
	//unsigned int indices[6] = {0, 1, 3, 3, 1, 2};
	//vao.writeIndices(&indices[0], 6);

	//// load and assign texturing shader
	//plane->setEffect(mEffectImporter.import(ASSET_PATH + "Shader/Test.g2fx"));
	//
	//auto* transformation = mPlanes.back().addComponent<G2::TransformComponent>();
	//transformation->setRotation(rot);
}

void 
TestScene::createWalls()
{	
	float rot = 5.f;
	mWalls.push_back(mMeshImporter2.import(ASSET_PATH + "Resources/unit-cube.fbx"));
	auto* transformation = mWalls.back().addComponent<G2::TransformComponent>();
	transformation->setScale(glm::vec3(5.f, 0.5f, 5.f));
	transformation->rotateX(10.f);
	transformation->setPosition(glm::vec3(0.f, -20.f, 0.f));
	//transformation->rotateX(45.f);
	transformation->updateWorldSpaceMatrix(0);
	auto* renderComp = mWalls.back().getComponent<G2::RenderComponent>();
	renderComp->calculateBinormalsAndTangents();
	renderComp->material.setSpecular(glm::vec4(1.f,0.f,0.f,1.f));
	renderComp->material.setShininess(128.f);
	renderComp->material.setAmbient(glm::vec4(0.5f,0.f,0.f,1.f));
	renderComp->material.setDiffuse(glm::vec4(1.f,1.f,1.f,1.f));
	
	renderComp->setEffect(mEffectImporter.import(ASSET_PATH + "Shader/NormalMapping.g2fx"));
	renderComp->material.setTexture(G2::Sampler::NORMAL, mTexImporter.import(ASSET_PATH + "Resources/normalmap.png", G2Core::DataFormat::Internal::R32G32B32A32_F, G2Core::FilterMode::NEAREST, G2Core::FilterMode::NEAREST, false));

	mWalls.back().addComponent<G2::Physics::PhysicsComponent>(
		G2::Physics::CollisionShapeDescriptor::box(glm::vec3(5.f, 0.5f, 5.f)),
		G2::Physics::RigidBodyDescriptor().setMass(0.f).setFriction(0.5f).setRestitution(0.7f),
		glm::vec3(0.f, -20.f, 0.f)
	);
	
	mWalls.push_back(mMeshImporter2.import(ASSET_PATH + "Resources/monkey.fbx"));
	transformation = mWalls.back().addComponent<G2::TransformComponent>();
	transformation->setScale(glm::vec3(1.f, 1.f, 1.f));
	transformation->setPosition(glm::vec3(0.f, -15.f, 0.f));
	//transformation->rotateX(45.f);
	transformation->updateWorldSpaceMatrix(0);
	renderComp = mWalls.back().addComponent<G2::RenderComponent>();
	renderComp->material.setSpecular(glm::vec4(0.f,1.f,0.f,1.f));
	renderComp->material.setShininess(128.f);
	renderComp->material.setAmbient(glm::vec4(0.2f,0.2f,0.13f,0.3));
	renderComp->material.setDiffuse(glm::vec4(1.f,0.23f,0.f,0.3));
	renderComp->setEffect(mEffectImporter.import(ASSET_PATH + "Shader/Glass.g2fx"));
	
	mWalls.push_back(mMeshImporter2.import(ASSET_PATH + "Resources/unit-sphere.fbx"));
	transformation = mWalls.back().addComponent<G2::TransformComponent>();
	transformation->setScale(glm::vec3(3.f, 3.f, 3.f));
	transformation->setPosition(glm::vec3(0.f, -17.5f, 0.f));
	//transformation->rotateX(45.f);
	transformation->updateWorldSpaceMatrix(0);
	renderComp = mWalls.back().addComponent<G2::RenderComponent>();
	renderComp->material.setSpecular(glm::vec4(1.f,1.f,1.f,1.f));
	renderComp->material.setShininess(255.f);
	renderComp->material.setAmbient(glm::vec4(0.0f,0.0f,0.0f,1.0));
	renderComp->material.setDiffuse(glm::vec4(1.f,1.f,1.f,1.0));
	renderComp->setEffect(mEffectImporter.import(ASSET_PATH + "Shader/Glass.g2fx"));

	
	//renderComp->setEffect(mEffectImporter.import(ASSET_PATH + "Shader/GeometryProposal.g2fx"));

	/*
	mReflecting = mMeshImporter2.import(ASSET_PATH + "Resources/unit-sphere.fbx");
	transformation = mReflecting->addComponent<G2::TransformComponent>();
	transformation->setPosition(glm::vec3(2.f,0.f,2.f));

	renderComp = mReflecting->addComponent<G2::RenderComponent>();
	renderComp->material.setSpecular(glm::vec4(1.f,0.f,0.f,1.f));
	renderComp->material.setShininess(128.f);
	renderComp->material.setAmbient(glm::vec4(0.5f,0.5f,0.5f,1.f));
	renderComp->material.setDiffuse(glm::vec4(1.f,1.f,1.f,1.f));
	renderComp->setEffect(mEffectImporter.import(ASSET_PATH + "Shader/CubeReflection.g2fx"));*/
}

void 
TestScene::onKeyUp(G2::KeyCode keyCode)
{
	
	std::cout << "KEY: " << keyCode << std::endl;
	
	if(keyCode == G2::KC_Y && !mEditorOn)
	{
		
		if(mPlanes.size() > 0)
		{
			auto* tc = mPlanes[0].getComponent<G2::TransformComponent>();
			tc->translate(glm::vec3(0.1,0.0,0.1));
		}
	}
	if(keyCode == G2::KC_G && !mEditorOn)
	{
		mWalls.back().getComponent<G2::RenderComponent>()->setPolygonDrawMode(G2Core::PolygonDrawMode::LINE);
	}
	if(keyCode == G2::KC_H && !mEditorOn)
	{
		mWalls.back().getComponent<G2::RenderComponent>()->setPolygonDrawMode(G2Core::PolygonDrawMode::FILL);
	}
	else if(keyCode == G2::KC_U && !mEditorOn) 
	{ 
		
		auto* light = mLight.getComponent<G2::LightComponent>();
		mLightType = light->getType()+1;

		mLight = mMeshImporter2.import(ASSET_PATH + "Resources/unit-sphere.fbx");
		
		if(mLightType > G2::LightType::DIRECTIONAL)
		{
			mLightType = G2::LightType::POSITIONAL;
		}
		if(mLightType == G2::LightType::POSITIONAL)
		{
			light = mLight.addComponent<G2::LightComponent>(G2::LightType::POSITIONAL);
			mLight.addComponent<G2::RenderComponent>()->setEffect(
				mEffectImporter.import(ASSET_PATH + "Shader/PointLightShadowMapping.g2fx")
			);
		}
		else if(mLightType == G2::LightType::SPOT)
		{
			light = mLight.addComponent<G2::LightComponent>(G2::LightType::SPOT);
			light->cutOffDegrees = 40.f;
		}
		else if(mLightType == G2::LightType::DIRECTIONAL)
		{
			light = mLight.addComponent<G2::LightComponent>(G2::LightType::DIRECTIONAL);
			auto* lightTransformation = mLight.addComponent<G2::TransformComponent>();
			lightTransformation->rotateAxis(40.0f, glm::vec3(1.f,0.f,1.f));
		}

		
	
	
		auto* physics = mLight.addComponent<G2::Physics::PhysicsComponent>(
			G2::Physics::CollisionShapeDescriptor::box(glm::vec3(0.1f,0.1f,0.1f)), 
			G2::Physics::RigidBodyDescriptor().setMass(10.f)
		);



		light->diffuse = glm::vec4(0.5,0.6,0.4f,0.f);
		light->specular = glm::vec4(1.f,1.f,1.f,0.f);
		//light->linearAttenuation = 1.f;
	
		auto* lightTransformation = mLight.addComponent<G2::TransformComponent>();
		lightTransformation->setPosition(glm::vec3(0.123f,0.7f,1.0123f));
		//lightTransformation->setPosition(glm::vec3(0.f,1.5f,0.f));
		//lightTransformation->setScale(glm::vec3(0.1f,0.1f,0.1f));
	}
	else if(keyCode == G2::KC_N && !mEditorOn)
	{ 
		if(mFbxMeshes.size() > 0)
		{
			mFbxMeshes.pop_back();
		}
	}
	else if(keyCode == G2::KC_J && !mEditorOn)
	{ 
		if(mMd5Meshes.size() > 0)
		{
			mMd5Meshes.pop_back();
		}
	}
	else if(keyCode == G2::KC_Q && !mEditorOn)
	{ 
		for(int i = 0; i < 1; ++i)
		{
			mBalls.push_back(mMeshImporter2.import(ASSET_PATH + "Resources/unit-sphere.fbx"));
			auto* renderComp = mBalls.back().getComponent<G2::RenderComponent>();
			auto* physics = mBalls.back().addComponent<G2::Physics::PhysicsComponent>(
				G2::Physics::CollisionShapeDescriptor::sphere(0.1), 
				G2::Physics::RigidBodyDescriptor().setMass(0.1f).setFriction(0.75f).setRestitution(0.5f),
				glm::vec3((rand() % 1000) / 1000.0f*(0.1f),3+(i*2),(rand() % 1000) / 1000.0f*(0.1f))
			);
			renderComp->material.setSpecular(glm::vec4(1.f,1.f,1.f,1.f));
			renderComp->material.setShininess(16.f);
			renderComp->material.setAmbient(glm::vec4(0.5f,0.f,0.5f,1.f));
			renderComp->material.setDiffuse(glm::vec4(1.f,0.5f,1.f,1.f));
		}
	}

	

	//std::cout << "Key: " << (int)keyCode << std::endl;
}

void 
TestScene::onKeyDown(G2::KeyCode keyCode) {
	
	if(keyCode == G2::KC_ESCAPE)
	{ 
		mExitRendering = true; 
	}
	if(keyCode == G2::KC_I && !mEditorOn)
	{ 
		mWalls.pop_back();
	}

	

	else if(keyCode == G2::KC_UP && !mEditorOn)
	{ 
		auto* trans = mLight.getComponent<G2::TransformComponent>();
		trans->translate(glm::vec3(0.f,0.f,-0.1f));
	}
	else if(keyCode == G2::KC_DOWN && !mEditorOn) 
	{ 
		auto* trans = mLight.getComponent<G2::TransformComponent>();
		trans->translate(glm::vec3(0.f,0.f,0.1f));
	}
	else if(keyCode == G2::KC_LEFT && !mEditorOn) 
	{ 
		auto* trans = mLight.getComponent<G2::TransformComponent>();
		trans->translate(glm::vec3(-0.1f,0.f,0.f));
	}
	else if(keyCode == G2::KC_RIGHT && !mEditorOn)
	{ 
		auto* trans = mLight.getComponent<G2::TransformComponent>();
		trans->translate(glm::vec3(0.1f,0.f,0.f));
	}
	else if(keyCode == G2::KC_M && !mEditorOn)
	{ 
		mFbxMeshes.push_back(mMeshImporter2.import(ASSET_PATH + "Resources/humanoid.fbx"));

			auto* transformComponent = mFbxMeshes.back().addComponent<G2::TransformComponent>();
			transformComponent->setScale(glm::vec3(0.01f,0.01f,0.01f));
			transformComponent->setPosition(glm::vec3(0.0,0,mFbxMeshes.size()*0.3));
			auto* animComp = mFbxMeshes.back().getComponent<G2::FBXAnimationComponent>();
			animComp->tempSetPoseIndex(mFbxMeshes.size()%3);
	}
	else if(keyCode == G2::KC_K && !mEditorOn)
	{ 
		std::vector<std::string> animFiles;
		animFiles.push_back(ASSET_PATH + "Resources/boblampclean.md5anim");

		mMd5Meshes.push_back(mMeshImporter.import(ASSET_PATH + "Resources/boblampclean.md5mesh", true, true, true, animFiles));
			
			int row = mMd5Meshes.size() % 10;
			int col = mMd5Meshes.size() / 10;

			auto* transformComponent = mMd5Meshes.back().addComponent<G2::TransformComponent>();
			transformComponent->setScale(glm::vec3(0.01f,0.01f,0.01f));
			transformComponent->rotateAxis(-90.0f, glm::vec3(1.f,0.f,0.f));
			transformComponent->setPosition(glm::vec3(col*0.3,0,row*0.3));

			if(mPlanes.size() > 0)
			{
				transformComponent->setParent(mPlanes[0].getComponent<G2::TransformComponent>());
				// provoke circular dependency check fail
				mPlanes[0].getComponent<G2::TransformComponent>()->setParent(transformComponent);
			}
	}
	else if(keyCode == G2::KC_F && !mEditorOn)
	{ 
		generateGeometryForFrusta();
	}
	else if(keyCode == G2::KC_SPACE)
	{ 
		mEditorOn = !mEditorOn;
		if(mEditorOn)
		{
			mEditorCamera.pause();
			mEditor.start();
		}
		else
		{
			mEditorCamera.unpause();
			mEditor.stop();
		}
	}
}

void
TestScene::onViewportResize(int width, int height) 
{
	mEditorCamera.setViewport(width, height);
}

void 
TestScene::onMouseMove(glm::detail::tvec2<int> const& mouseCoords)
{

}

void
TestScene::onRenderFrame(G2::FrameInfo const& frameInfo) 
{
	// set exit state - the only mutable property!
	frameInfo.stopRenderingAfterThisFrame = mExitRendering;

	for (int i = 0; i < mFbxMeshes.size() ; ++i) 
	{
		auto* tcomp = mFbxMeshes[i].getComponent<G2::TransformComponent>();
		if(tcomp != nullptr)
		{
			tcomp->rotate(glm::angleAxis(10.0f*(float)frameInfo.timeSinceLastFrame, glm::vec3(0.f,1.f,0.f)));
		}
	}
		
		glm::quat rot = glm::angleAxis(-(frameInfo.frame%90)/1.f, glm::vec3(1.f,0.f,0.f));
		
		auto* transformComponent = mSampleMesh.addComponent<G2::TransformComponent>();
		transformComponent->setRotation(rot);
		//transformComponent->setPosition(glm::vec3(0,1,0));
}

void
TestScene::createWaterSurface() 
{
	//auto* water = mWaterSurface.addComponent<G2::RenderComponent>();
	//water->allocateVertexArrays(1);
	//water->drawMode = GL_TRIANGLES;
	//// prepare vao
	//G2::VertexArrayObject& vao = water->getVertexArray(0);

	//vao.resizeElementCount(4);

	//glm::vec4 planeGeometry[4];
	//planeGeometry[0] = glm::vec4(-5.f,0.f,-5.f,1.f);
	//planeGeometry[1] = glm::vec4(5.f,0.f,-5.f,1.f);
	//planeGeometry[2] = glm::vec4(5.f,0.f,5.f,1.f);
	//planeGeometry[3] = glm::vec4(-5.f,0.f,5.f,1.f);

	//vao.writeData(G2::Semantics::POSITION, planeGeometry);

	//// build tex coordinates
	//glm::vec2 tex[4];
	//tex[0] = glm::vec2(0.f,0.f);
	//tex[1] = glm::vec2(0.f,1.f);
	//tex[2] = glm::vec2(1.f,1.f);
	//tex[3] = glm::vec2(1.f,0.f);
	//vao.writeData(G2::Semantics::TEXCOORD_0, tex);

	//// build tex coordinates
	//glm::vec3 normal[4];
	//normal[0] = glm::vec3(0.f,1.f,0.f);
	//normal[1] = glm::vec3(0.f,1.f,0.f);
	//normal[2] = glm::vec3(0.f,1.f,0.f);
	//normal[3] = glm::vec3(0.f,1.f,0.f);
	//vao.writeData(G2::Semantics::NORMAL, normal);

	//// build indices - planes were rendered using indices as soon as we add some to the vao
	//unsigned int indices[6] = {0, 1, 3, 3, 1, 2};
	//vao.writeIndices(&indices[0], 6);

	//// load and assign shader
	//water->setEffect(mEffectImporter.import(ASSET_PATH + "Shader/Water.g2fx"));

	//water->material
	//	.setAmbient(glm::vec4(0.2,0.2,0.2,0.2))
	//	.setDiffuse(glm::vec4(0.2,0.3,0.4,0.3))
	//	.setSpecular(glm::vec4(0.2,0.3,0.4,0.2))
	//	.setShininess(128.f);
	//
	//auto* transformation = mWaterSurface.addComponent<G2::TransformComponent>();
	//transformation->translate(glm::vec3(0.f,-3.5f,0.f));
}

void
TestScene::onMouseDown(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords) 
{
	if(mEditorOn)
	{
		return;
	}
	if(button == G2::MOUSE_LEFT)
	{
		auto* transformComp = mEditorCamera.getComponent<G2::TransformComponent>();
		glm::vec4 pos = glm::inverse(transformComp->getWorldSpaceMatrix()) * glm::vec4(0.f,0.f,0.f,1.f);
		
		mBalls.push_back(mMeshImporter2.import(ASSET_PATH + "Resources/unit-sphere.fbx"));
		auto* renderComp = mBalls.back().getComponent<G2::RenderComponent>();
		mBalls.back().addComponent<G2::Physics::PhysicsComponent>(
			G2::Physics::CollisionShapeDescriptor::sphere(0.1), 
			G2::Physics::RigidBodyDescriptor()
				.setMass(0.1f)
				.setFriction(0.75f)
				.setRestitution(0.5f)
				.setLinearVelocity(glm::vec3(mEditorCamera.getViewVec()) * 10.f),
			glm::vec3(pos)
		);//->trackAABBCollisions(true);
		renderComp->material.setSpecular(glm::vec4(1.f,1.f,1.f,1.f));
		renderComp->material.setShininess(16.f);
		renderComp->material.setAmbient(glm::vec4(0.5f,0.f,0.5f,1.f));
		renderComp->material.setDiffuse(glm::vec4(1.f,0.5f,1.f,1.f));
	}
}

void
TestScene::generateGeometryForFrusta() 
{
	
	auto* light = mLight.getComponent<G2::LightComponent>();

	//for( int i = 0; i < light->getShadowDescriptor().numCascades; ++i)
	{
		//mPlanes.push_back(GameObject());
	
		//auto* renderComp = mPlanes.back().addComponent<G2::RenderComponent>();
		//renderComp->drawMode = GL_TRIANGLES;
		//// prepare vao
		//G2::VertexArrayObject vao;

		//vao.resize(24);

		//glm::vec4 geometry[24];
		//// right frustum plane
		//geometry[0] = light->getShadowDescriptor().frusta[i].getCornerPoints()[0];
		//geometry[1] = light->getShadowDescriptor().frusta[i].getCornerPoints()[4];
		//geometry[2] = light->getShadowDescriptor().frusta[i].getCornerPoints()[5];
		//geometry[3] = light->getShadowDescriptor().frusta[i].getCornerPoints()[0];
		//geometry[4] = light->getShadowDescriptor().frusta[i].getCornerPoints()[5];
		//geometry[5] = light->getShadowDescriptor().frusta[i].getCornerPoints()[1];
		//// left frustum plane
		//geometry[6] = light->getShadowDescriptor().frusta[i].getCornerPoints()[3];
		//geometry[7] = light->getShadowDescriptor().frusta[i].getCornerPoints()[6];
		//geometry[8] = light->getShadowDescriptor().frusta[i].getCornerPoints()[7];
		//geometry[9] = light->getShadowDescriptor().frusta[i].getCornerPoints()[3];
		//geometry[10] = light->getShadowDescriptor().frusta[i].getCornerPoints()[2];
		//geometry[11] = light->getShadowDescriptor().frusta[i].getCornerPoints()[6];
		//// top frustum plane
		//geometry[12] = light->getShadowDescriptor().frusta[i].getCornerPoints()[1];
		//geometry[13] = light->getShadowDescriptor().frusta[i].getCornerPoints()[2];
		//geometry[14] = light->getShadowDescriptor().frusta[i].getCornerPoints()[6];
		//geometry[15] = light->getShadowDescriptor().frusta[i].getCornerPoints()[1];
		//geometry[16] = light->getShadowDescriptor().frusta[i].getCornerPoints()[6];
		//geometry[17] = light->getShadowDescriptor().frusta[i].getCornerPoints()[5];
		//// bottom frustum plane
		//geometry[18] = light->getShadowDescriptor().frusta[i].getCornerPoints()[0];
		//geometry[19] = light->getShadowDescriptor().frusta[i].getCornerPoints()[7];
		//geometry[20] = light->getShadowDescriptor().frusta[i].getCornerPoints()[3];
		//geometry[21] = light->getShadowDescriptor().frusta[i].getCornerPoints()[0];
		//geometry[22] = light->getShadowDescriptor().frusta[i].getCornerPoints()[4];
		//geometry[23] = light->getShadowDescriptor().frusta[i].getCornerPoints()[7];

		//vao.writeData(G2::Semantics::POSITION, geometry);

		//// assign vao to RenderComponent using move semantic
		//renderComp->vaos.push_back(std::move(vao));

		//if(i == 0)
		//{
		//	renderComp->material
		//		.setAmbient(glm::vec4(1.0,0.0,0.0,0.2))
		//		.setDiffuse(glm::vec4(1.0,0.0,0.0,0.2));
		//}
		//else if(i == 1)
		//{
		//	renderComp->material
		//		.setAmbient(glm::vec4(0.0,1.0,0.0,0.2))
		//		.setDiffuse(glm::vec4(0.0,1.0,0.0,0.2));
		//}
		//else if(i == 2)
		//{
		//	renderComp->material
		//		.setAmbient(glm::vec4(0.0,0.0,1.0,0.2))
		//		.setDiffuse(glm::vec4(0.0,0.0,1.0,0.2));
		//}



		
		//mPlanes.push_back(GameObject());
	
		//renderComp = mPlanes.back().addComponent<G2::RenderComponent>();
		//renderComp->drawMode = GL_TRIANGLES;
		//// prepare vao
		//G2::VertexArrayObject vao2;

		//vao2.resize(24);
		//// right frustum plane
		//geometry[0] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[0];
		//geometry[1] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[4];
		//geometry[2] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[5];
		//geometry[3] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[0];
		//geometry[4] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[5];
		//geometry[5] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[1];
		//// left frustum plane
		//geometry[6] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[3];
		//geometry[7] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[6];
		//geometry[8] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[7];
		//geometry[9] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[3];
		//geometry[10] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[2];
		//geometry[11] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[6];
		//// top frustum plane
		////geometry[12] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[1];
		////geometry[13] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[2];
		////geometry[14] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[6];
		////geometry[15] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[1];
		////geometry[16] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[6];
		////geometry[17] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[5];
		//geometry[12] = glm::vec4();
		//geometry[13] = glm::vec4();
		//geometry[14] = glm::vec4();
		//geometry[15] = glm::vec4();
		//geometry[16] = glm::vec4();
		//geometry[17] = glm::vec4();
		//// bottom frustum plane
		//geometry[18] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[0];
		//geometry[19] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[7];
		//geometry[20] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[3];
		//geometry[21] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[0];
		//geometry[22] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[4];
		//geometry[23] = light->getShadowDescriptor().orthoFrusta[i].getCornerPoints()[7];

		//vao2.writeData(G2::Semantics::POSITION, geometry);

		//// assign vao to RenderComponent using move semantic
		//renderComp->vaos.push_back(std::move(vao2));

		//if(i == 0)
		//{
		//	renderComp->material
		//		.setAmbient(glm::vec4(1.0,0.0,0.0,0.2))
		//		.setDiffuse(glm::vec4(1.0,0.0,0.0,0.2));
		//}
		//else if(i == 1)
		//{
		//	renderComp->material
		//		.setAmbient(glm::vec4(0.0,1.0,0.0,0.2))
		//		.setDiffuse(glm::vec4(0.0,1.0,0.0,0.2));
		//}
		//else if(i == 2)
		//{
		//	renderComp->material
		//		.setAmbient(glm::vec4(0.0,0.0,1.0,0.2))
		//		.setDiffuse(glm::vec4(0.0,0.0,1.0,0.2));
		//}
	}
}