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

	setFocusPolicy(Qt::FocusPolicy::ClickFocus);

	initKeyMap();
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
	int key = event->key();
	auto keyIter = mKeyMap.find((Qt::Key)key);
	if(keyIter != mKeyMap.end())
	{
		key = keyIter->second;
	}
	G2::EventDistributer::onKeyDown(static_cast<G2::KeyCode>(key));
}

void GLContext::keyReleaseEvent(QKeyEvent* event) 
{
	int key = event->key();
	auto keyIter = mKeyMap.find((Qt::Key)key);
	if(keyIter != mKeyMap.end())
	{
		key = keyIter->second;
	}
	G2::EventDistributer::onKeyUp(static_cast<G2::KeyCode>(key));
}

void
GLContext::initKeyMap() 
{
	mKeyMap.insert(std::make_pair(Qt::Key_Return,G2::KC_RETURN));
	mKeyMap.insert(std::make_pair(Qt::Key_Escape,G2::KC_ESCAPE));
	
	mKeyMap.insert(std::make_pair(Qt::Key_Backspace,G2::KC_BACKSPACE));
	mKeyMap.insert(std::make_pair(Qt::Key_Tab,G2::KC_TAB));
	mKeyMap.insert(std::make_pair(Qt::Key_Space,G2::KC_SPACE));
	mKeyMap.insert(std::make_pair(Qt::Key_Exclam,G2::KC_EXCLAM));
	mKeyMap.insert(std::make_pair(Qt::Key_QuoteDbl,G2::KC_QUOTEDBL));
	mKeyMap.insert(std::make_pair(Qt::Key_Percent,G2::KC_PERCENT));
	mKeyMap.insert(std::make_pair(Qt::Key_Dollar,G2::KC_DOLLAR));
	mKeyMap.insert(std::make_pair(Qt::Key_Ampersand,G2::KC_AMPERSAND));
	mKeyMap.insert(std::make_pair(Qt::Key_QuoteLeft,G2::KC_QUOTE));
	mKeyMap.insert(std::make_pair(Qt::Key_ParenLeft,G2::KC_LEFTPAREN));
	mKeyMap.insert(std::make_pair(Qt::Key_ParenRight,G2::KC_RIGHTPAREN));
	mKeyMap.insert(std::make_pair(Qt::Key_Asterisk,G2::KC_ASTERISK));
	mKeyMap.insert(std::make_pair(Qt::Key_Plus,G2::KC_PLUS));
	mKeyMap.insert(std::make_pair(Qt::Key_Comma,G2::KC_COMMA));
	mKeyMap.insert(std::make_pair(Qt::Key_Minus,G2::KC_MINUS));
	mKeyMap.insert(std::make_pair(Qt::Key_Period,G2::KC_PERIOD));
	mKeyMap.insert(std::make_pair(Qt::Key_Slash,G2::KC_SLASH));
	mKeyMap.insert(std::make_pair(Qt::Key_F1,G2::KC_F1));
	mKeyMap.insert(std::make_pair(Qt::Key_F2,G2::KC_F2));
	mKeyMap.insert(std::make_pair(Qt::Key_F3,G2::KC_F3));
	mKeyMap.insert(std::make_pair(Qt::Key_F4,G2::KC_F4));
	mKeyMap.insert(std::make_pair(Qt::Key_F5,G2::KC_F5));
	mKeyMap.insert(std::make_pair(Qt::Key_F6,G2::KC_F6));
	mKeyMap.insert(std::make_pair(Qt::Key_F7,G2::KC_F7));
	mKeyMap.insert(std::make_pair(Qt::Key_F8,G2::KC_F8));
	mKeyMap.insert(std::make_pair(Qt::Key_F9,G2::KC_F9));
	mKeyMap.insert(std::make_pair(Qt::Key_F10,G2::KC_F10));
	mKeyMap.insert(std::make_pair(Qt::Key_F11,G2::KC_F11));
	mKeyMap.insert(std::make_pair(Qt::Key_F12,G2::KC_F12));
	mKeyMap.insert(std::make_pair(Qt::Key_Left,G2::KC_LEFT));
	mKeyMap.insert(std::make_pair(Qt::Key_Right,G2::KC_RIGHT));
	mKeyMap.insert(std::make_pair(Qt::Key_Up,G2::KC_UP));
	mKeyMap.insert(std::make_pair(Qt::Key_Down,G2::KC_DOWN));
}

GLContext::~GLContext() 
{
}