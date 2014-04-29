#include "TestScene.h"
#include <G2/TransformComponent.h>
#include <G2/FBXAnimationComponent.h>

#include <G2/MatrixMultiplicator.h>
#include <G2/Logger.h>
#include <G2/TimeTracker.h>
#include <G2/Effect.h>

TestScene::TestScene()
	: mMoveForward(false),
	mMoveBackward(false),
	mMoveLeft(false),
	mMoveRight(false),
	mMx(0),
	mMy(0),
	mExitRendering(false)
{
	srand(2006);

	//multMatrices();
	/*
	G2::MatrixMultiplicator mult;
	
	
	{
		G2::logger << "Multiply 10000 matrices with CL" << G2::endl;
		std::vector<glm::mat4> results;
		results.resize(10000);
		G2::TimeTracker timeTracker;
		for (int i = 0; i < 10000 ; ++i) 
		{
			glm::mat4 m1, m2, target;
			m1 = glm::translate(glm::vec3(i,3,i));
			m2 = glm::rotate(43.2f, glm::vec3(5,i,7));
			mult.mult(m1, m2, results[i]);
		}
		G2::logger << "Done in " << timeTracker.getMilliSeconds() <<"ms " << results.size() << "\n" << G2::endl;
	}
	{
		G2::logger << "Multiply 10000 matrices with CPU" << G2::endl;
		std::vector<glm::mat4> results;
		results.resize(10000);
		G2::TimeTracker timeTracker;
		for (int i = 0; i < 10000 ; ++i) 
		{
			glm::mat4 m1, m2, target;
			m1 = glm::translate(glm::vec3(i,3,i));
			m2 = glm::rotate(43.2f, glm::vec3(5,i,7));
			results[i] = m1 * m2;
		}
		G2::logger << "Done in " << timeTracker.getMilliSeconds() <<"ms " << results.size() << "\n" << G2::endl;
	}
	*/

	// create and activate the camera
	auto* cameraComponent = mCamera.addComponent<G2::CameraComponent>("Render Camera");
	cameraComponent->setAsRenderCamera();
	cameraComponent->setMoveSpeed(1.0f);
	cameraComponent->stepBackward();
	cameraComponent->setMoveSpeed(0.02f);
	
	// load default shader for the engine
	/*G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem,G2::RenderComponent>()
			->setDefaultShader(G2::UberShaderParser::parse("../../../Shader/Default.g2fx"));*/

	// new way of loading shader
	std::shared_ptr<G2::Effect> effect = mEffectImporter.import("../../../Shader/Default.g2fx");
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem,G2::RenderComponent>()
		->setDefaultEffect(effect);


	auto* renderComponent = mCube.addComponent<G2::RenderComponent>();

	float s = 0.5f;
	mCube = G2::AABB(glm::vec3(-s,-s,-s), glm::vec3(s,s,s));
	mCube.enableRendering();
	onViewportResize(512,512);


	G2::EventDistributer::onRenderFrame.hook(this, &TestScene::onRenderFrame);
	G2::EventDistributer::onViewportResize.hook(this, &TestScene::onViewportResize);
	G2::EventDistributer::onKeyUp.hook(this, &TestScene::onKeyUp);
	G2::EventDistributer::onKeyDown.hook(this, &TestScene::onKeyDown);
	G2::EventDistributer::onMouseMove.hook(this, &TestScene::onMouseMove);
	
	createPlane(glm::vec4(0.f,0.f,0.f,0.0), mTexImporter.import("../../../Resources/launch-button.jpg", G2::NEAREST, G2::NEAREST, false));
	
	//createPlane(glm::vec4(1.0,0.0,0.0,0.0), mTexImporter.import("../../../Resources/launch-button.jpg", G2::NEAREST, G2::NEAREST, true));
	
	//createPlane(glm::vec4(2.0,0.0,0.0,0.0), mTexImporter.import("../../../Resources/launch-button.jpg", G2::NEAREST, G2::LINEAR, false));
	
	//createPlane(glm::vec4(3.0,0.0,0.0,0.0), mTexImporter.import("../../../Resources/launch-button.jpg", G2::LINEAR, G2::LINEAR, false));
	
	//createPlane(glm::vec4(4.0,0.0,0.0,0.0), mTexImporter.import("../../../Resources/launch-button.jpg", G2::LINEAR, G2::NEAREST, false));
	
	std::vector<std::string> animFiles;
	animFiles.push_back("../../../Resources/boblampclean.md5anim");

	mSampleMesh = mMeshImporter.import("../../../Resources/boblampclean.md5mesh", animFiles);
	if(mSampleMesh.get()) 
	{
		auto* transformComponent = mSampleMesh->addComponent<G2::TransformComponent>();
		transformComponent->setScale(glm::vec3(0.01f,0.01f,0.01f));
	
		glm::quat rot = glm::angleAxis(-90.0f, glm::vec3(1.f,0.f,0.f));

		transformComponent->setRotation(rot);
		//transformComponent->setPosition(glm::vec3(0,1,0));
	}
	
	for (int k = 0; k < 0; ++k) 
	{
		for (int i = 0; i < 1; ++i) 
		{
		
			mFbxMeshes.push_back(mMeshImporter2.import("../../../Resources/humanoid.fbx"));
			if(mFbxMeshes.back().get()) 
			{
				auto* transformComponent = mFbxMeshes.back()->addComponent<G2::TransformComponent>();
				transformComponent->setScale(glm::vec3(0.01f,0.01f,0.01f));
				transformComponent->setPosition(glm::vec3(i*0.6,0,k*0.3));
			}
		}
	}
	
	//mSampleMesh3 = mMeshImporter2.import("../../Resources/humanoid.fbx");
	if(mSampleMesh3.get()) 
	{
		auto* transformComponent = mSampleMesh3->addComponent<G2::TransformComponent>();
		transformComponent->setScale(glm::vec3(0.01f,0.01f,0.01f));
		transformComponent->setPosition(glm::vec3(1,0,0));
		auto* animComponent = mSampleMesh3->getComponent<G2::FBXAnimationComponent>();
		animComponent->tempSetPoseIndex(0);
	}
}

TestScene::~TestScene()
{
	G2::EventDistributer::onRenderFrame.unHook(this, &TestScene::onRenderFrame);
	G2::EventDistributer::onViewportResize.unHook(this, &TestScene::onViewportResize);
	G2::EventDistributer::onKeyUp.unHook(this, &TestScene::onKeyUp);
	G2::EventDistributer::onKeyDown.unHook(this, &TestScene::onKeyDown);
	G2::EventDistributer::onMouseMove.unHook(this, &TestScene::onMouseMove);
}

void
TestScene::createPlane(glm::vec4 const& corner, std::shared_ptr<G2::Texture2D> const& diffuseTex) 
{
	mPlanes.push_back(GameObject());
	auto* plane = mPlanes.back().addComponent<G2::RenderComponent>();
	plane->drawMode = GL_TRIANGLES;
	// import and assign a texture
	plane->material.setTexture(G2::Sampler::DIFFUSE, diffuseTex);

	// prepare vao
	G2::VertexArrayObject vao;

	vao.resize(4);

	glm::vec4 planeGeometry[4];
	planeGeometry[0] = corner + glm::vec4(-5.f,0.f,-5.f,1.f);
	planeGeometry[1] = corner + glm::vec4(5.f,0.f,-5.f,1.f);
	planeGeometry[2] = corner + glm::vec4(5.f,0.f,5.f,1.f);
	planeGeometry[3] = corner + glm::vec4(-5.f,0.f,5.f,1.f);

	vao.writeData(G2::Semantics::POSITION, planeGeometry);

	// build tex coordinates
	glm::vec2 tex[6];
	tex[0] = glm::vec2(0.f,0.f);
	tex[1] = glm::vec2(0.f,1.f);
	tex[2] = glm::vec2(1.f,1.f);
	tex[3] = glm::vec2(1.f,0.f);
	vao.writeData(G2::Semantics::TEXCOORD_0, tex);

	// build indices - planes were rendered using indices as soon as we add some to the vao
	unsigned int indices[6] = {0, 1, 3, 3, 1, 2};
	vao.writeIndices(&indices[0], 6);

	// assign vao to RenderComponent using move semantic
	plane->vaos.push_back(std::move(vao));

	// load and assign texturing shader
	plane->setEffect(mEffectImporter.import("../../../Shader/MultipassProposal.g2fx"));
	
	mPlanes.back().addComponent<G2::TransformComponent>();
}

void 
TestScene::onKeyUp(G2::KeyCode keyCode)
{
	
	std::cout << "KEY: " << keyCode << std::endl;
	
	if(keyCode == G2::KC_Y)
	{
		
		if(mPlanes.size() > 0)
		{
			auto* tc = mPlanes[0].getComponent<G2::TransformComponent>();
			tc->translate(glm::vec3(0.1,0.0,0.1));
		}
	}

	if(keyCode == G2::KC_W) 
	{ 
		mMoveForward = false;
	}
	else if(keyCode == G2::KC_W) 
	{ 
		mMoveForward = false;
	}
	else if(keyCode == G2::KC_S) 
	{
		mMoveBackward = false;
	}
	else if(keyCode == G2::KC_A) 
	{ 
		mMoveLeft = false; 
	}
	else if(keyCode == G2::KC_D) 
	{ 
		mMoveRight = false; 
	}
	else if(keyCode == G2::KC_N)
	{ 
		if(mFbxMeshes.size() > 0)
		{
			mFbxMeshes.pop_back();
		}
	}
	else if(keyCode == G2::KC_J)
	{ 
		if(mMd5Meshes.size() > 0)
		{
			mMd5Meshes.pop_back();
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
	else if(keyCode == G2::KC_W)
	{ 
		mMoveForward = true; 
	}
	else if(keyCode == G2::KC_S) 
	{ 
		mMoveBackward = true; 
	}
	else if(keyCode == G2::KC_A) 
	{ 
		mMoveLeft = true; 
	}
	else if(keyCode == G2::KC_D)
	{ 
		mMoveRight = true; 
	}
	else if(keyCode == G2::KC_M)
	{ 
		mFbxMeshes.push_back(mMeshImporter2.import("../../../Resources/humanoid.fbx"));
		if(mFbxMeshes.back().get()) 
		{
			auto* transformComponent = mFbxMeshes.back()->addComponent<G2::TransformComponent>();
			transformComponent->setScale(glm::vec3(0.01f,0.01f,0.01f));
			transformComponent->setPosition(glm::vec3(0.0,0,mFbxMeshes.size()*0.3));
			auto* animComp = mFbxMeshes.back()->getComponent<G2::FBXAnimationComponent>();
			animComp->tempSetPoseIndex(mFbxMeshes.size()%3);
		}
	}
	else if(keyCode == G2::KC_K)
	{ 
		std::vector<std::string> animFiles;
		animFiles.push_back("../../../Resources/boblampclean.md5anim");

		mMd5Meshes.push_back(mMeshImporter.import("../../../Resources/boblampclean.md5mesh", animFiles));
		if(mMd5Meshes.back().get()) 
		{
			
			int row = mMd5Meshes.size() % 10;
			int col = mMd5Meshes.size() / 10;

			auto* transformComponent = mMd5Meshes.back()->addComponent<G2::TransformComponent>();
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
	}
}

void
TestScene::onViewportResize(int width, int height) 
{
	if(height == 0)
	{ 
		height = 1; 
	}
	auto* camera = mCamera.getComponent<G2::CameraComponent>();
	camera->setProjectionMatrix(glm::perspective(80.0f, width / (float)height, 0.001f, 10.0f));
}

void 
TestScene::onMouseMove(glm::detail::tvec2<int> const& mouseCoords)
{
		int dx = mouseCoords.x - mMx;
		int dy = mouseCoords.y - mMy;

		mMx = mouseCoords.x;
		mMy = mouseCoords.y;

		auto* camera = mCamera.getComponent<G2::CameraComponent>();
		camera->rotate(dy*camera->getRotationSpeed(), 
							 dx*camera->getRotationSpeed());
}

void
TestScene::onRenderFrame(G2::FrameInfo const& frameInfo) 
{
	// set exit state - the only mutable property!
	frameInfo.stopRenderingAfterThisFrame = mExitRendering;

	auto* camera = mCamera.getComponent<G2::CameraComponent>();

	// make movement independent from framerate
	camera->setMoveSpeed((float)frameInfo.timeSinceLastFrame);

	if(mMoveForward) 
	{ 
		camera->stepForward(); 
	}
	if(mMoveBackward) 
	{ 
		camera->stepBackward(); 
	}
	if(mMoveLeft) 
	{ 
		camera->stepLeft(); 
	}
	if(mMoveRight) 
	{ 
		camera->stepRight();
	}

	for (int i = 0; i < mFbxMeshes.size() ; ++i) 
	{
		auto* tcomp = mFbxMeshes[i].get()->getComponent<G2::TransformComponent>();
		tcomp->rotate(glm::angleAxis(10.0f*(float)frameInfo.timeSinceLastFrame, glm::vec3(0.f,1.f,0.f)));
	}
}