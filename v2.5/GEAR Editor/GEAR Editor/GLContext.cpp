#include "GLContext.h"
#include <QtGui/QMouseEvent>
#include <QTimer>


static std::string ASSET_PATH = "../../../../../../tests/bin/Assets/";

GLContext::GLContext(QWidget *parent) 
	: QGLWidget(parent), 
	mEditorCamera(this),
	G2::AbstractWindow("GEAR Editor", 1024, 768, false) 
{
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(10);
}

void GLContext::initializeGL() 
{
	G2_init();

	setMouseTracking(true);
	
	mEditorCamera
		.pan(0.f,5.f)
		.rotate(25.f, 0.f)
		.moveView(-15.f)
		.getComponent<G2::CameraComponent>()->setAsRenderCamera();
	mEditorCamera.setInternals(70.f,0.01f,1000.f);
	
	mMouseButtonMapping.insert(std::make_pair(Qt::LeftButton,G2::MOUSE_LEFT));
	mMouseButtonMapping.insert(std::make_pair(Qt::MiddleButton,G2::MOUSE_MIDDLE));
	mMouseButtonMapping.insert(std::make_pair(Qt::RightButton,G2::MOUSE_RIGHT));

	
	std::shared_ptr<G2::Effect> effect = mEffectImporter.import(ASSET_PATH + "Shader/Default.g2fx");
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem,G2::RenderComponent>()
		->setDefaultEffect(effect);
	G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem,G2::RenderComponent>()
		->setClearColor(glm::vec4(0.f,0.f,0.f,1.f));

	mLight = mFbxImporter.import(ASSET_PATH + "Resources/unit-sphere.fbx");

	
	auto* light = mLight->addComponent<G2::LightComponent>(G2::LightType::DIRECTIONAL);
	
	light->getType();
	light->diffuse = glm::vec4(1.f,1.f,1.f,1.f);
	light->linearAttenuation = 1.f;
	 
	auto* lightTransformation = mLight->addComponent<G2::TransformComponent>();
	lightTransformation->rotateAxis(-10.0f, glm::vec3(1.f,0.f,0.f));
}

void GLContext::resizeGL(int w, int h) 
{
	mWidth = w;
	mHeight = h;
	GLDEBUG( glViewport(0, 0, mWidth, mHeight) );
	G2::EventDistributer::onViewportResize(mWidth, mHeight);
	
	mEditorCamera.setViewport(mWidth, mHeight);
}

void GLContext::paintGL() 
{
	G2_singleFrame(*this, mFrameInfo);
}

void GLContext::mousePressEvent(QMouseEvent *event)
{
	G2::EventDistributer::onMouseDown(mMouseButtonMapping[event->button()], glm::detail::tvec2<int>(event->x(), event->y()));
}

void GLContext::mouseReleaseEvent(QMouseEvent *event)
{
	G2::EventDistributer::onMouseUp(mMouseButtonMapping[event->button()], glm::detail::tvec2<int>(event->x(), event->y()));
}

void GLContext::mouseMoveEvent(QMouseEvent *event) 
{
	glm::detail::tvec2<int> mousePos(event->x(), event->y());
	G2::EventDistributer::onMouseMove(mousePos);
}

void
GLContext::wheelEvent(QWheelEvent* event) 
{
	QPoint numPixels = event->pixelDelta();
	QPoint numDegrees = event->angleDelta() / 8;

	if (!numPixels.isNull()) 
	{
		//scrollWithPixels(numPixels);
		G2::EventDistributer::onMouseWheel(numPixels.y());
	} 
	else if (!numDegrees.isNull()) 
	{
		QPoint numSteps = numDegrees / 15;
		//scrollWithDegrees(numSteps);
		G2::EventDistributer::onMouseWheel(numSteps.y());
	}

	event->accept();
}

void GLContext::keyPressEvent(QKeyEvent* event) 
{
	switch(event->key()) 
	{
		case Qt::Key_Escape:
			close();
			break;
		default:
			event->ignore();
			break;
	}
}


GLContext::~GLContext() 
{
}