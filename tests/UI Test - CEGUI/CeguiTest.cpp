#include "CeguiTest.h"
#include <G2/TransformComponent.h>
#include <G2/FBXAnimationComponent.h>

#include <G2/MatrixMultiplicator.h>
#include <G2/Logger.h>
#include <G2/TimeTracker.h>
#include <G2/Effect.h>

#include <G2Cegui/CeguiComponent.h>
#include <CEGUI/CEGUI.h>

//#define USE_LAYOUT_FILE

static std::string ASSET_PATH = "../../Assets/";

CeguiTest::CeguiTest(G2::SDLWindow& window)
	: mExitRendering(false),
	mWindow(window),
	mEditorCamera(&window)
{

	initCegui();


	srand(2006);

	mEditorCamera
		.pan(0.f,10.f)
		.rotate(25.f, 0.f)
		.moveView(-15.f)
		.getComponent<G2::CameraComponent>()->setAsRenderCamera();

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

	auto uiComponent = mLight->addComponent<G2::UI::CeguiComponent>();

	// create (load) the TaharezLook scheme file
	// (this auto-loads the TaharezLook looknfeel and imageset files)
	CEGUI::SchemeManager::getSingleton().createFromFile( "TaharezLook.scheme" );

	CEGUI::System::getSingleton().getDefaultGUIContext().
	getMouseCursor().setDefaultImage( "TaharezLook/MouseArrow" );

	CEGUI::System::getSingleton().getDefaultGUIContext().
	setDefaultTooltipType( "TaharezLook/Tooltip" );

#ifdef USE_LAYOUT_FILE
	// loading from xml-file

	mGUIWindows.push_back(G2::Entity());
	// load window
	auto* ceguiComponent = mGUIWindows.back().addComponent<G2::UI::CeguiComponent>("Taharez_Test.layout");
	ceguiComponent->attachToRootWindow();


	//CEGUI::Window* myRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile( "Taharez_Test.layout" );
	//CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow( myRoot );
	//CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow().removeChild(myRoot->getID())
#else
	mGUIWindows.push_back(G2::Entity());
	// load window and attach to already existing root window
	auto* ceguiComponent = mGUIWindows.back().addComponent<G2::UI::CeguiComponent>("TaharezLook/FrameWindow", "testWindow");
	ceguiComponent->attachToRootWindow();

	// cast to the desired type if functionality in CEGUI::Window is missing
	CEGUI::FrameWindow* fWnd = static_cast<CEGUI::FrameWindow*>(ceguiComponent->getWindow());


	// position a quarter of the way in from the top-left of parent.
	fWnd->setPosition( CEGUI::UVector2( CEGUI::UDim( 0.25f, 0.0f ), CEGUI::UDim( 0.25f, 0.0f ) ) );
	// set size to be half the size of the parent
	fWnd->setSize( CEGUI::USize( CEGUI::UDim( 0.5f, 0.0f ), CEGUI::UDim( 0.5f, 0.0f ) ) );

	fWnd->setText( "Hello World!" );

	// Create a button as a CeguiComponent
	mGUIWindows.push_back(G2::Entity());
	auto* button = mGUIWindows.back().addComponent<G2::UI::CeguiComponent>("TaharezLook/Button","TestButton");
	
	button->getWindow()->setPosition(CEGUI::UVector2(CEGUI::UDim(0.75,0),CEGUI::UDim(0.50,0)));
	button->getWindow()->setSize(CEGUI::USize(CEGUI::UDim(0,150),CEGUI::UDim(0,50)));
	button->getWindow()->setText("Jump!");
	fWnd->addChild(button->getWindow());  

#endif

	mGameConsole = std::shared_ptr<GameConsoleWindow>(new GameConsoleWindow());

	// ensure GUI is rendered after the scene!
	G2::ECSManager::getShared().updateInvokeOrder<G2::RenderSystem,G2::RenderComponent,G2::UI::CeguiSystem,G2::UI::CeguiComponent>();

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

void
CeguiTest::initCegui() 
{
	// init the CeguiSystem of gear to bootstrap the OpenGL renderer
	G2::ECSManager::getShared().getSystem<G2::UI::CeguiSystem,G2::UI::CeguiComponent>();

	/****************************************
	 * INIT CEGUI RESOURCE LOADING SYSTEM	*
	 ****************************************/
	// initialise the required dirs for the DefaultResourceProvider
	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
		(CEGUI::System::getSingleton().getResourceProvider());
	rp->setResourceGroupDirectory("schemes", ASSET_PATH + "Resources/cegui/schemes/");
	rp->setResourceGroupDirectory("imagesets", ASSET_PATH + "Resources/cegui/imagesets/");
	rp->setResourceGroupDirectory("fonts", ASSET_PATH + "Resources/cegui/fonts/");
	rp->setResourceGroupDirectory("layouts", ASSET_PATH + "Resources/cegui/layouts/");
	rp->setResourceGroupDirectory("looknfeels", ASSET_PATH + "Resources/cegui/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", ASSET_PATH + "Resources/cegui/lua_scripts/");

	// set the default resource groups to be used
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
}