#include "SplineAnimationTest.h"

#include <G2/CatmullRomCurve.h>

static std::string ASSET_PATH = "../../Assets/";

SplineAnimationTest::SplineAnimationTest(G2::SDL::Window& window)
	: mExitRendering(false),
	mWindow(window),
	mEditorCamera(&window),
	mEditor(&window, ASSET_PATH + "G2Editor/")
{
	mEditorCamera
		.pan(0.f,10.f)
		.rotate(25.f, 0.f)
		.moveView(-15.f)
		.getComponent<G2::CameraComponent>()->setAsRenderCamera();
	mEditorCamera.setInternals(70.f,0.01f,1000.f);
	
	// new way of loading shader
	std::shared_ptr<G2::Effect> effect = mEffectImporter.import(ASSET_PATH + "Shader/Default.g2fx");
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem,G2::RenderComponent>()
		->setDefaultEffect(effect);
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem,G2::RenderComponent>()
		->setClearColor(glm::vec4(0.f,0.f,0.f,1.f));

	G2::EventDistributer::onRenderFrame.hook(this, &SplineAnimationTest::onRenderFrame);
	G2::EventDistributer::onViewportResize.hook(this, &SplineAnimationTest::onViewportResize);
	G2::EventDistributer::onKeyUp.hook(this, &SplineAnimationTest::onKeyUp);
	G2::EventDistributer::onKeyDown.hook(this, &SplineAnimationTest::onKeyDown);
	G2::EventDistributer::onMouseMove.hook(this, &SplineAnimationTest::onMouseMove);
	G2::EventDistributer::onMouseDown.hook(this, &SplineAnimationTest::onMouseDown);

	mLight = mFbxImporter.import(ASSET_PATH + "Resources/unit-sphere.fbx");

	
	auto* light = mLight->addComponent<G2::LightComponent>(G2::LightType::DIRECTIONAL);
	
	light->getType();
	light->diffuse = glm::vec4(1.f,1.f,1.f,1.f);
	light->linearAttenuation = 1.f;
	 
	auto* lightTransformation = mLight->addComponent<G2::TransformComponent>();
	lightTransformation->rotateAxis(-10.0f, glm::vec3(1.f,0.f,0.f));

	mEditor.start();
	
	createSpline(G2::SplineInterpolationType::CATMULL_ROM, "Catmull Rom Spline", 0.f);
	createSpline(G2::SplineInterpolationType::BASIS_SPLINE, "Basis Spline", -4.f);
	createSpline(G2::SplineInterpolationType::HERMITE_SPLINE, "Hermite Spline", -8.f);
}

SplineAnimationTest::~SplineAnimationTest(void)
{
	G2::EventDistributer::onRenderFrame.unHook(this, &SplineAnimationTest::onRenderFrame);
	G2::EventDistributer::onViewportResize.unHook(this, &SplineAnimationTest::onViewportResize);
	G2::EventDistributer::onKeyUp.unHook(this, &SplineAnimationTest::onKeyUp);
	G2::EventDistributer::onKeyDown.unHook(this, &SplineAnimationTest::onKeyDown);
	G2::EventDistributer::onMouseMove.unHook(this, &SplineAnimationTest::onMouseMove);
	G2::EventDistributer::onMouseDown.unHook(this, &SplineAnimationTest::onMouseDown);
}

void
SplineAnimationTest::createSpline(G2::SplineInterpolationType::Name type, std::string const& name, float zOffset) 
{
	mSplines.push_back(mFbxImporter.import(ASSET_PATH + "Resources/monkey.fbx"));


	G2::InterpolationDescription desc;
	desc.animatePosition = true;
	desc.loops = true;

	std::vector<G2::CurveSample> curveSamples;
	
	G2::CurveSample sample;
	sample.point = glm::vec3(-5.0f,-2.5f,-10.f+zOffset);
	curveSamples.push_back(sample);
	sample.point = glm::vec3( 5.0f, 2.5f,-10.f+zOffset);
	curveSamples.push_back(sample);
	sample.point = glm::vec3( 7.5f, 0.0f,-12.f+zOffset);
	curveSamples.push_back(sample);
	sample.point = glm::vec3(5.f,-2.5f,-10.f+zOffset);
	curveSamples.push_back(sample);
	sample.point = glm::vec3(-5.0f, 2.5f,-10.f+zOffset);
	curveSamples.push_back(sample);
	sample.point = glm::vec3(-7.5f, 0.0f,-8.f+zOffset);
	curveSamples.push_back(sample);


	auto* splineAnimation = mSplines.back()->addComponent<G2::SplineAnimation>(std::shared_ptr<G2::Curve>(new G2::CatmullRomCurve(desc, curveSamples)));
	
	
	std::vector<glm::vec3> geometry = splineAnimation->debugSamplePath(0.05f);

	//mSplineVis.push_back(G2::Entity());
	//auto* renderComponent = mSplineVis.back().addComponent<G2::RenderComponent>();
	//auto* nameComp = mSplineVis.back().addComponent<G2::NameComponent>();
	//nameComp->name = name;
	//renderComponent->allocateVertexArrays(1);	
	//G2::VertexArrayObject& vao = renderComponent->getVertexArray(0);
	//vao.resizeElementCount((unsigned int)geometry.size());
	//vao.writeData(G2::Semantics::POSITION, &geometry[0]);
	//
	//renderComponent->addDrawCall(G2::DrawCall()
	//	.setDrawMode(GL_LINE_STRIP)
	//	.setVaoIndex(renderComponent->getNumVertexArrays()-1)
	//	.setEnabled(true)
	//);
	//renderComponent->addDrawCall(G2::DrawCall()
	//	.setDrawMode(GL_POINTS)
	//	.setVaoIndex(renderComponent->getNumVertexArrays()-1)
	//	.setEnabled(true)
	//);
}

void
SplineAnimationTest::onViewportResize(int width, int height) 
{
	mEditorCamera.setViewport(width, height);
}

void
SplineAnimationTest::onKeyUp(G2::KeyCode keyCode) 
{

}

void
SplineAnimationTest::onKeyDown(G2::KeyCode keyCode) 
{

}

void
SplineAnimationTest::onMouseDown(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords) 
{

}

void
SplineAnimationTest::onMouseMove(glm::detail::tvec2<int> const& mouseCoords) 
{

}

void
SplineAnimationTest::onRenderFrame(G2::FrameInfo const&) 
{

}