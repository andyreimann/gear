#include "TexturingTest.h"

static std::string ASSET_PATH = "../../Assets/";

TexturingTest::TexturingTest(G2::SDL::Window& window)
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

	std::string phases[] = {"preUpdate","update","postUpdate","render","ui"};
	G2::ECSManager::getShared().setMainThreadPhases(phases, 5);

	G2::EventDistributer::onRenderFrame.hook(this, &TexturingTest::onRenderFrame);
	G2::EventDistributer::onViewportResize.hook(this, &TexturingTest::onViewportResize);
	G2::EventDistributer::onKeyUp.hook(this, &TexturingTest::onKeyUp);
	G2::EventDistributer::onKeyDown.hook(this, &TexturingTest::onKeyDown);
	G2::EventDistributer::onMouseMove.hook(this, &TexturingTest::onMouseMove);
	G2::EventDistributer::onMouseDown.hook(this, &TexturingTest::onMouseDown);

	mLight = mFbxImporter.import(ASSET_PATH + "Resources/monkey.fbx");

	
	auto* light = mLight->addComponent<G2::LightComponent>(G2::LightType::DIRECTIONAL);
	
	light->getType();
	light->diffuse = glm::vec4(1.f,1.f,1.f,1.f);
	light->linearAttenuation = 1.f;
	 
	auto* lightTransformation = mLight->addComponent<G2::TransformComponent>();
	lightTransformation->rotateAxis(-10.0f, glm::vec3(1.f,0.f,0.f));

	
	// generate animation
	//std::vector<G2::AnimationSample> samples;

	//
	//G2::AnimationSample sample;
	//sample.point = glm::vec3(0.f,0.f,0.f);
	//samples.push_back(sample);
	//sample.point = glm::vec3(0.f,4.f,5.f);
	//samples.push_back(sample);
	//sample.point = glm::vec3(5.f,4.f,5.f);
	//samples.push_back(sample);
	//sample.point = glm::vec3(5.f,0.f,0.f);
	//samples.push_back(sample);



	//G2::AnimationSample sample;
	//sample.point = glm::vec3(0.f,0.f,0.f);
	//sample.speed = 0.3;
	//samples.push_back(sample);
	//sample.point = glm::vec3(0.f,0.f,5.f);
	//sample.speed = 1.3;
	//samples.push_back(sample);
	//sample.point = glm::vec3(5.f,0.f,-2.f);
	//sample.speed = 0.3;
	//samples.push_back(sample);
	//sample.point = glm::vec3(5.f,0.f,5.f);
	//sample.speed = 0.7;
	//samples.push_back(sample);
	//
	//sample.point = glm::vec3(0.f,1.f,3.f);
	//sample.speed = 1.3;
	//samples.push_back(sample);
	//sample.point = glm::vec3(5.f,1.f,5.f);
	//sample.speed = 0.7;
	//samples.push_back(sample);
	//sample.point = glm::vec3(7.f,3.f,0.f);
	//sample.speed = 0.1;
	//samples.push_back(sample);
	//sample.point = glm::vec3(0.f,1.f,5.f);
	//sample.speed = 0.3;
	//samples.push_back(sample);

	//G2::AnimationDescription desc;
	//desc.loops = true;
	//desc.interpolationType = G2::SplineInterpolationType::CATMULL_ROM;
	//desc.animateOrientation = true;
	//mLight->addComponent<G2::SplineAnimation>(desc, samples);




	mEditor.start();

	float width = 1.f;
	float height = 1.f / 1920.f * 1080.f;
	
	// visualize normal min and mag filter using assassins creed HD image
	createTexturedPlane(glm::vec3(-3.f*width,-1.f,0.f), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR,  G2Core::FilterMode::LINEAR), "MIN: LINEAR, Mag: LINEAR");
	createTexturedPlane(glm::vec3(-2.f*width,-1.f,0.f), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::NEAREST, G2Core::FilterMode::NEAREST), "MIN: NEAREST, Mag: NEAREST");
	createTexturedPlane(glm::vec3(-1.f*width,-1.f,0.f), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR,  G2Core::FilterMode::NEAREST), "MIN: LINEAR, Mag: NEAREST");
	createTexturedPlane(glm::vec3( 0.f*width,-1.f,0.f), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::NEAREST, G2Core::FilterMode::LINEAR), "MIN: NEAREST, Mag: LINEAR");
	
	// visualize normal min and mag filter using assassins creed HD image using mipmaps
	createTexturedPlane(glm::vec3(-3.f*width,-1.f,-1.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR_MIPMAP_LINEAR,   G2Core::FilterMode::LINEAR), "MIN: LINEAR_MIPMAP_LINEAR, Mag: LINEAR");
	createTexturedPlane(glm::vec3(-2.f*width,-1.f,-1.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::NEAREST_MIPMAP_NEAREST, G2Core::FilterMode::NEAREST), "MIN: NEAREST_MIPMAP_NEAREST, Mag: NEAREST");
	createTexturedPlane(glm::vec3(-1.f*width,-1.f,-1.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR_MIPMAP_LINEAR,   G2Core::FilterMode::NEAREST), "MIN: LINEAR_MIPMAP_LINEAR, Mag: NEAREST");
	createTexturedPlane(glm::vec3( 0.f*width,-1.f,-1.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::NEAREST_MIPMAP_NEAREST, G2Core::FilterMode::LINEAR), "MIN: NEAREST_MIPMAP_NEAREST, Mag: LINEAR");
	createTexturedPlane(glm::vec3(-3.f*width,-1.f,-2.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR_MIPMAP_NEAREST,  G2Core::FilterMode::LINEAR), "MIN: LINEAR_MIPMAP_NEAREST, Mag: LINEAR");
	createTexturedPlane(glm::vec3(-2.f*width,-1.f,-2.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::NEAREST_MIPMAP_LINEAR,  G2Core::FilterMode::NEAREST), "MIN: NEAREST_MIPMAP_LINEAR, Mag: NEAREST");
	createTexturedPlane(glm::vec3(-1.f*width,-1.f,-2.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR_MIPMAP_NEAREST,  G2Core::FilterMode::NEAREST), "MIN: LINEAR_MIPMAP_NEAREST, Mag: NEAREST");
	createTexturedPlane(glm::vec3( 0.f*width,-1.f,-2.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::NEAREST_MIPMAP_LINEAR,  G2Core::FilterMode::LINEAR), "MIN: NEAREST_MIPMAP_LINEAR, Mag: LINEAR");

	// modify the internal format of the imported images
	//createTexturedPlane(glm::vec3(-3.f*width,-1.f,-3.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR, G2Core::FilterMode::LINEAR, false, G2Core::WrapMode::REPEAT, G2Core::WrapMode::REPEAT, G2Core::DataFormat::LUMINANCE), "Internal format: LUMINANCE"); // not supported by NSight
	createTexturedPlane(glm::vec3(-2.f*width, -1.f, -3.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR, G2Core::FilterMode::LINEAR, false, G2Core::WrapMode::REPEAT, G2Core::WrapMode::REPEAT, G2Core::DataFormat::RED), "Internal format: RED");
	createTexturedPlane(glm::vec3(-1.f*width, -1.f, -3.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR, G2Core::FilterMode::LINEAR, false, G2Core::WrapMode::REPEAT, G2Core::WrapMode::REPEAT, G2Core::DataFormat::RG), "Internal format: RG");
	//createTexturedPlane(glm::vec3( 0.f*width,-1.f,-3.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR, G2Core::FilterMode::LINEAR, false, G2Core::WrapMode::REPEAT, G2Core::WrapMode::REPEAT, G2Core::DataFormat::INTENSITY), "Internal format: INTENSITY"); // not supported by NSight
	
	// compressed images
	//createTexturedPlane(glm::vec3(-3.f*width,-1.f,-4.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR, G2Core::FilterMode::LINEAR, true, G2::WrapMode::REPEAT, G2::WrapMode::REPEAT, G2Core::DataFormat::LUMINANCE), "Compressed Internal format: LUMINANCE"); // not supported by NSight
	createTexturedPlane(glm::vec3(-2.f*width,-1.f,-4.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR, G2Core::FilterMode::LINEAR, true, G2Core::WrapMode::REPEAT, G2Core::WrapMode::REPEAT, G2Core::DataFormat::RED), "Compressed Internal format: RED");
	createTexturedPlane(glm::vec3(-1.f*width,-1.f,-4.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR, G2Core::FilterMode::LINEAR, true, G2Core::WrapMode::REPEAT, G2Core::WrapMode::REPEAT, G2Core::DataFormat::RG), "Compressed Internal format: RGB");
	createTexturedPlane(glm::vec3( 0.f*width,-1.f,-4.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR, G2Core::FilterMode::LINEAR, true, G2Core::WrapMode::REPEAT, G2Core::WrapMode::REPEAT, G2Core::DataFormat::RGB), "Compressed Internal format: RGB");
	
	// compressed images
	createTexturedPlane(glm::vec3(-3.f*width, -1.f, -5.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR, G2Core::FilterMode::LINEAR, false, G2Core::WrapMode::REPEAT, G2Core::WrapMode::REPEAT, G2Core::DataFormat::RGBA), "Internal format: RGBA");
	
	// after switch to external renderer we have temporarily dropped
	// support for compressed formats
	//createTexturedPlane(glm::vec3(-2.f*width, -1.f, -5.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR, G2Core::FilterMode::LINEAR, true, G2Core::WrapMode::REPEAT, G2Core::WrapMode::REPEAT, COMPRESSED_RGBA), "Compressed Internal format: GL_COMPRESSED_SIGNED_RED_RGTC1");
	//createTexturedPlane(glm::vec3(-1.f*width, -1.f, -5.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR, G2Core::FilterMode::LINEAR, false, G2Core::WrapMode::REPEAT, G2Core::WrapMode::REPEAT, GL_COMPRESSED_RG_RGTC2), "Compressed Internal format: GL_COMPRESSED_RG_RGTC2");
	//createTexturedPlane(glm::vec3(0.f*width, -1.f, -5.f*height), width, height, mTexImporter.import(ASSET_PATH + "Resources/ac-unity-hd.jpg", G2Core::FilterMode::LINEAR, G2Core::FilterMode::LINEAR, false, G2Core::WrapMode::REPEAT, G2Core::WrapMode::REPEAT, GL_COMPRESSED_SIGNED_RG_RGTC2), "Compressed Internal format: GL_COMPRESSED_SIGNED_RG_RGTC2");
}

TexturingTest::~TexturingTest(void)
{
	G2::EventDistributer::onRenderFrame.unHook(this, &TexturingTest::onRenderFrame);
	G2::EventDistributer::onViewportResize.unHook(this, &TexturingTest::onViewportResize);
	G2::EventDistributer::onKeyUp.unHook(this, &TexturingTest::onKeyUp);
	G2::EventDistributer::onKeyDown.unHook(this, &TexturingTest::onKeyDown);
	G2::EventDistributer::onMouseMove.unHook(this, &TexturingTest::onMouseMove);
	G2::EventDistributer::onMouseDown.unHook(this, &TexturingTest::onMouseDown);
}

void
TexturingTest::createTexturedPlane(glm::vec3 const& center, float width, float height, std::shared_ptr<G2::Texture2D> const& diffuseTex, std::string const& name) 
{
	mTexturedPlanes.push_back(G2::Entity());
	auto* plane = mTexturedPlanes.back().addComponent<G2::RenderComponent>();
	auto* nameComp = mTexturedPlanes.back().addComponent<G2::NameComponent>();
	nameComp->name = name;
	plane->allocateVertexArrays(1);
	plane->allocateIndexArrays(1);

	//plane->drawMode = GL_TRIANGLES;
	//// import and assign a texture
	plane->material.setTexture(G2::Sampler::DIFFUSE, diffuseTex); // only rendered/used in passes

	//// prepare vao
	//// assign vao to RenderComponent using move semantic
	G2::VertexArrayObject& vao = plane->getVertexArray(0);

	vao.resizeElementCount(4); 

	glm::vec3 planeGeometry[4];
	width *= 0.5f;
	height *= 0.5f;
	planeGeometry[0] = center + glm::vec3(-width,0.f,-height);
	planeGeometry[1] = center + glm::vec3(width,0.f,-height);
	planeGeometry[2] = center + glm::vec3(width,0.f,height);
	planeGeometry[3] = center + glm::vec3(-width,0.f,height);

	vao.writeData(G2Core::Semantics::POSITION, planeGeometry);

	glm::vec2 tex[4];
	tex[0] = glm::vec2(0.f,0.f);
	tex[1] = glm::vec2(1.f,0.f);
	tex[2] = glm::vec2(1.f,1.f);
	tex[3] = glm::vec2(0.f,1.f);
	vao.writeData(G2Core::Semantics::TEXCOORD_0, tex);

	// build indices
	G2::IndexArrayObject& iao = plane->getIndexArray(0);
	unsigned int indices[6] = {0, 3, 1, 1, 3, 2};
	iao.writeIndices(&indices[0], 6);

	// add draw call
	plane->addDrawCall(G2::DrawCall()
		.setDrawMode(G2Core::DrawMode::TRIANGLES)
		.setIaoIndex(0)
		.setVaoIndex(0));

	// load and assign texturing shader
	plane->setEffect(mEffectImporter.import(ASSET_PATH + "Shader/SimpleTex.g2fx"));
}

void
TexturingTest::onViewportResize(int width, int height) 
{
	mEditorCamera.setViewport(width, height);
}

void
TexturingTest::onKeyUp(G2::KeyCode keyCode) 
{

}

void
TexturingTest::onKeyDown(G2::KeyCode keyCode) 
{

}

void
TexturingTest::onMouseDown(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords) 
{

}

void
TexturingTest::onMouseMove(glm::detail::tvec2<int> const& mouseCoords) 
{

}

void
TexturingTest::onRenderFrame(G2::FrameInfo const&) 
{

}