#include "GLContext.h"
#include "Project.h"
#include "GEARStudioEvents.h"
#include "EditorGeometryManager.h"
#include "TranslationHandler.h"
#include "TransformationHandler.h"
#include "QmlOverlay.h"

#include <QtGui/QMouseEvent>
#include <QtCore/QTimer>
#include <QQmlEngine>
#include <QtQml/qqml.h>
#include <QQmlComponent>


#include <G2/RenderComponent.h>
#include <G2Core/ECSManager.h>
#include <G2Core/EventDistributer.h>

// QML TEST
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOffscreenSurface>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQuickRenderControl>
#include <QtOpenGL/QGLContext>

static std::string ASSET_PATH = "../../tests/bin/Assets/";

GLContext::GLContext(QWidget *parent) 
#ifndef GEAR_QT_5_4
	: QGLWidget(parent), 
#else
	: QOpenGLWidget(parent),
#endif
	mEditorCamera(this),
	G2::AbstractWindow("GEAR Editor", 1024, 768, false),
	mProject(nullptr),
	mEditorGeometryManager(nullptr),
	mEntity(nullptr),
	m_rootItem(0),
	m_fbo(0),
	m_program(0),
	m_vbo(0),
	m_quickInitialized(false),
	m_quickReady(false),
	gearInitialized(false)
{
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(10); 

	setFocusPolicy(Qt::FocusPolicy::ClickFocus);

	initKeyMap();

	GEARStudioEvents::onProjectOpened.hook(this, &GLContext::_onProjectOpened);
	GEARStudioEvents::onManagedEntitySelected.hook(this, &GLContext::_onManagedEntitySelected);
	G2::EventDistributer::onKeyDown.hook(this, &GLContext::_onFocusEntity);
}

GLContext::~GLContext()
{
	GEARStudioEvents::onProjectOpened.unHookAll(this);
	GEARStudioEvents::onManagedEntitySelected.unHookAll(this);
	G2::EventDistributer::onKeyDown.unHookAll(this);
}

void GLContext::initQuick()
{
	//QSurfaceFormat format;
	////// Qt Quick may need a depth and stencil buffer. Always make sure these are available.
	//format.setDepthBufferSize(16);
	//format.setStencilBufferSize(8);
	////setFormat(format);

	///*m_context = new QOpenGLContext;
	//m_context->setFormat(format);
	//m_context->create();*/

	//m_offscreenSurface = new QOffscreenSurface;
	//// Pass m_context->format(), not format. Format does not specify and color buffer
	//// sizes, while the context, that has just been created, reports a format that has
	//// these values filled in. Pass this to the offscreen surface to make sure it will be
	//// compatible with the context's configuration.
	//m_offscreenSurface->setFormat(format);
	//m_offscreenSurface->create();

	//m_renderControl = new QQuickRenderControl(this);

	//// Create a QQuickWindow that is associated with out render control. Note that this
	//// window never gets created or shown, meaning that it will never get an underlying
	//// native (platform) window.
	//m_quickWindow = new QQuickWindow(m_renderControl);

	//// Create a QML engine.
	//m_qmlEngine = new QQmlEngine;
	//if (!m_qmlEngine->incubationController())
	//	m_qmlEngine->setIncubationController(m_quickWindow->incubationController());

	//// When Quick says there is a need to render, we will not render immediately. Instead,
	//// a timer with a small interval is used to get better performance.
	//m_updateTimer.setSingleShot(true);
	//m_updateTimer.setInterval(5);
	//connect(&m_updateTimer, &QTimer::timeout, this, &GLContext::updateQuick);

	// Now hook up the signals. For simplicy we don't differentiate between
	// renderRequested (only render is needed, no sync) and sceneChanged (polish and sync
	// is needed too).
	//connect(m_quickWindow, &QQuickWindow::sceneGraphInitialized, this, &GLContext::createFbo);
	//connect(m_quickWindow, &QQuickWindow::sceneGraphInvalidated, this, &GLContext::destroyFbo);
	//connect(m_renderControl, &QQuickRenderControl::renderRequested, this, &GLContext::requestUpdate);
	//connect(m_renderControl, &QQuickRenderControl::sceneChanged, this, &GLContext::requestUpdate);
}

void GLContext::updateQuick()
{
}

void GLContext::run()
{
	//disconnect(m_qmlComponent, SIGNAL(statusChanged(QQmlComponent::Status)), this, SLOT(run()));

	//if (m_qmlComponent->isError()) {
	//	QList<QQmlError> errorList = m_qmlComponent->errors();
	//	foreach(const QQmlError &error, errorList)
	//		qWarning() << error.url() << error.line() << error;
	//	return;
	//}

	//QObject *rootObject = m_qmlComponent->create();
	//if (m_qmlComponent->isError()) {
	//	QList<QQmlError> errorList = m_qmlComponent->errors();
	//	foreach(const QQmlError &error, errorList)
	//		qWarning() << error.url() << error.line() << error;
	//	return;
	//}

	//m_rootItem = qobject_cast<QQuickItem *>(rootObject);
	//if (!m_rootItem) {
	//	qWarning("run: Not a QQuickItem");
	//	delete rootObject;
	//	return;
	//}

	//// The root item is ready. Associate it with the window.
	//m_rootItem->setParentItem(m_quickWindow->contentItem());

	//// Update item and rendering related geometries.
	//updateSizes();

	//// Initialize the render control and our OpenGL resources.
	//context()->makeCurrent(m_offscreenSurface);
	//m_renderControl->initialize(context());

	//static const char *vertexShaderSource =
	//	"attribute highp vec4 vertex;\n"
	//	"attribute lowp vec2 coord;\n"
	//	"varying lowp vec2 v_coord;\n"
	//	"uniform highp mat4 matrix;\n"
	//	"void main() {\n"
	//	"   v_coord = coord;\n"
	//	"   gl_Position = matrix * vertex;\n"
	//	"}\n";
	//static const char *fragmentShaderSource =
	//	"varying lowp vec2 v_coord;\n"
	//	"uniform sampler2D sampler;\n"
	//	"void main() {\n"
	//	"   gl_FragColor = vec4(texture2D(sampler, v_coord).rgb, 1.0);\n"
	//	"}\n";
	//m_program = new QOpenGLShaderProgram;
	//m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	//m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	//m_program->bindAttributeLocation("vertex", 0);
	//m_program->bindAttributeLocation("coord", 1);
	//m_program->link();
	//m_matrixLoc = m_program->uniformLocation("matrix");

	//m_vao = new QOpenGLVertexArrayObject;
	//m_vao->create();
	//m_vao->bind();

	//m_vbo = new QOpenGLBuffer;
	//m_vbo->create();
	//m_vbo->bind();

	//GLfloat v[] = {
	//	-0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5,
	//	0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
	//	-0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, 0.5, -0.5,
	//	0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5,

	//	0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, -0.5,
	//	0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5,
	//	-0.5, 0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5,
	//	-0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, 0.5,

	//	0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5,
	//	-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5,
	//	-0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5,
	//	0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5
	//};
	//GLfloat texCoords[] = {
	//	0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	//	1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	//	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	//	0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

	//	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	//	0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	//	1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

	//	0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	//	1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	//	1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
	//};

	//const int vertexCount = 36;
	//m_vbo->allocate(sizeof(GLfloat) * vertexCount * 5);
	//m_vbo->write(0, v, sizeof(GLfloat) * vertexCount * 3);
	//m_vbo->write(sizeof(GLfloat) * vertexCount * 3, texCoords, sizeof(GLfloat) * vertexCount * 2);
	//m_vbo->release();

	//if (m_vao->isCreated())
	//	setupVertexAttribs();

	//// Must unbind before changing the current context. Hence the absence of
	//// QOpenGLVertexArrayObject::Binder here.
	//m_vao->release();

	//context()->doneCurrent();
	m_quickInitialized = true;
}

void GLContext::createFbo()
{
	// The scene graph has been initialized. It is now time to create an FBO and associate
	// it with the QQuickWindow.
	//m_fbo = new QOpenGLFramebufferObject(size(), QOpenGLFramebufferObject::CombinedDepthStencil);
	//m_quickWindow->setRenderTarget(m_fbo);
}

void GLContext::destroyFbo()
{
	//delete m_fbo;
	//m_fbo = 0;
}

void GLContext::requestUpdate()
{
	//if (!m_updateTimer.isActive())
	//	m_updateTimer.start();
}

void GLContext::setupVertexAttribs()
{
	//m_vbo->bind();
	//m_program->enableAttributeArray(0);
	//m_program->enableAttributeArray(1);
	//context()->functions()->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//context()->functions()->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
	//	(const void *)(36 * 3 * sizeof(GLfloat)));
	//m_vbo->release();
}

void GLContext::startQuick(const QString &filename)
{
	//m_qmlComponent = new QQmlComponent(m_qmlEngine, QUrl(filename));
	//if (m_qmlComponent->isLoading())
	//	connect(m_qmlComponent, &QQmlComponent::statusChanged, this, &GLContext::run);
	//else
	//	run();
}

void GLContext::updateSizes()
{
	//// Behave like SizeRootObjectToView.
	//m_rootItem->setWidth(width());
	//m_rootItem->setHeight(height());

	//m_quickWindow->setGeometry(0, 0, width(), height());

	//m_proj.setToIdentity();
	//m_proj.perspective(45, width() / float(height()), 0.01f, 100.0f);
}

void GLContext::initializeGL() 
{
	// Initialize the GEAR Gfx device - this cannot hapen earlier
	G2_gfxDevice()->init(nullptr);

	setMouseTracking(true);
	
	mMouseButtonMapping.insert(std::make_pair(Qt::LeftButton,G2::MOUSE_LEFT));
	mMouseButtonMapping.insert(std::make_pair(Qt::MiddleButton,G2::MOUSE_MIDDLE));
	mMouseButtonMapping.insert(std::make_pair(Qt::RightButton,G2::MOUSE_RIGHT));

	// QML TEST
	// http://doc.qt.io/qt-5/qtquick-rendercontrol-window-cpp.html
	//initQuick();

	//if (!m_quickInitialized)
	//	startQuick(QStringLiteral("qrc:/rendercontrol/demo.qml"));
}

void GLContext::resizeGL(int w, int h) 
{
	mWidth = w;
	mHeight = h;
	//GLDEBUG( glViewport(0, 0, mWidth, mHeight) );
	G2::EventDistributer::onViewportResize(mWidth, mHeight);
	
	mEditorCamera.setViewport(mWidth, mHeight);
}

void GLContext::paintGL() 
{
	if (gearInitialized)
	{
		G2_singleFrame(*this, mFrameInfo);
	}
}

void GLContext::mousePressEvent(QMouseEvent *event)
{
	G2::EventDistributer::onMouseDown(mMouseButtonMapping[event->button()], glm::detail::tvec2<int>(event->x(), event->y()));

	// intersect scene
	if (mProject != nullptr && mProject->getCurrentScene().get() != nullptr && mMouseButtonMapping[event->button()] == G2::MOUSE_LEFT)
	{
		auto* transform = mEditorCamera.getComponent<G2::TransformComponent>();
		auto* camera = mEditorCamera.getComponent<G2::CameraComponent>();
		
		G2::Ray mouseRay = G2::Ray::createScreenProjectionRay(
			event->x(), 
			event->y(),
			transform->getWorldSpaceMatrix(),
			camera->getProjectionMatrix(),
			glm::detail::tvec4<int>(0, 0, camera->getViewportWidth(), camera->getViewportHeight()));
		 
		G2::Intersection intersection = G2::ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>()->intersect(mouseRay, EditorGeometryManager::gHandleLayer);

		if (intersection.getState() != G2::IntersectionState::NO_INTERSECTION)
		{
			GEARStudioEvents::onEditorHandleSelected(intersection.getEntityId(), intersection);
		}
		else
		{
			// no editor handle was clicked
			intersection = G2::ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>()->intersect(mouseRay, ~EditorGeometryManager::gHandleLayer);

			if (intersection.getState() != G2::IntersectionState::NO_INTERSECTION)
			{
				auto* managedEntity = mProject->getCurrentScene()->get(intersection.getEntityId());
				if (managedEntity != nullptr)
				{
					GEARStudioEvents::onManagedEntitySelected(managedEntity);
				}
			}
		}
	}
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

void GLContext::_onProjectOpened(Project* project)
{
	mProject = project;
}

void GLContext::initKeyMap() 
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
	mKeyMap.insert(std::make_pair(Qt::Key_Down, G2::KC_DOWN));
	mKeyMap.insert(std::make_pair(Qt::Key_A, G2::KC_A));
	mKeyMap.insert(std::make_pair(Qt::Key_B, G2::KC_B));
	mKeyMap.insert(std::make_pair(Qt::Key_C, G2::KC_C));
	mKeyMap.insert(std::make_pair(Qt::Key_D, G2::KC_D));
	mKeyMap.insert(std::make_pair(Qt::Key_E, G2::KC_E));
	mKeyMap.insert(std::make_pair(Qt::Key_F, G2::KC_F));
	mKeyMap.insert(std::make_pair(Qt::Key_G, G2::KC_G));
	mKeyMap.insert(std::make_pair(Qt::Key_H, G2::KC_H));
	mKeyMap.insert(std::make_pair(Qt::Key_I, G2::KC_I));
	mKeyMap.insert(std::make_pair(Qt::Key_J, G2::KC_J));
	mKeyMap.insert(std::make_pair(Qt::Key_K, G2::KC_K));
	mKeyMap.insert(std::make_pair(Qt::Key_L, G2::KC_L));
	mKeyMap.insert(std::make_pair(Qt::Key_M, G2::KC_M));
	mKeyMap.insert(std::make_pair(Qt::Key_N, G2::KC_N));
	mKeyMap.insert(std::make_pair(Qt::Key_O, G2::KC_O));
	mKeyMap.insert(std::make_pair(Qt::Key_P, G2::KC_P));
	mKeyMap.insert(std::make_pair(Qt::Key_Q, G2::KC_Q));
	mKeyMap.insert(std::make_pair(Qt::Key_R, G2::KC_R));
	mKeyMap.insert(std::make_pair(Qt::Key_S, G2::KC_S));
	mKeyMap.insert(std::make_pair(Qt::Key_T, G2::KC_T));
	mKeyMap.insert(std::make_pair(Qt::Key_U, G2::KC_U));
	mKeyMap.insert(std::make_pair(Qt::Key_V, G2::KC_V));
	mKeyMap.insert(std::make_pair(Qt::Key_W, G2::KC_W));
	mKeyMap.insert(std::make_pair(Qt::Key_X, G2::KC_X));
	mKeyMap.insert(std::make_pair(Qt::Key_Y, G2::KC_Y));
	mKeyMap.insert(std::make_pair(Qt::Key_Z, G2::KC_Z));
}

void
GLContext::createEditorCamera()
{
	// create a new EditorCamera
	mEditorCamera = G2Cameras::EditorCamera(this);
	// set it as the main camera
	mEditorCamera
		.translate(0.f, 5.f)
		.rotate(25.f, 0.f)
		.zoom(-15.f)
		.getComponent<G2::CameraComponent>()->setAsRenderCamera();
	mEditorCamera.setInternals(70.f, 0.01f, 300.f);
	gearInitialized = true;
}

void
GLContext::setEditorGeometryManager(std::shared_ptr<EditorGeometryManager> const& editorGeometryManager)
{
	mEditorGeometryManager = editorGeometryManager;

	mTransformationHandler = std::shared_ptr<TransformationHandler>(new TransformationHandler(
		mEditorGeometryManager->getTransformationHandleId()
	));
	mTranslationHandler = std::shared_ptr<TranslationHandler>(new TranslationHandler(
		mEditorGeometryManager->getXTranslationHandleId(),
		mEditorGeometryManager->getYTranslationHandleId(),
		mEditorGeometryManager->getZTranslationHandleId()
	));
}

void
GLContext::_onManagedEntitySelected(ManagedEntity* entity)
{
	mEntity = entity;
}

void
GLContext::_onFocusEntity(G2::KeyCode keyCode)
{
	if (keyCode == G2::KC_F)
	{
		// check preconditions for focus...
		if (mEntity != nullptr && mEntity->hasComponent<G2::RenderComponent>())
		{
			G2::AABB fullAABB;
			glm::vec4 pos(0.f,0.f,0.f,1.f);
			glm::mat4 worldSpaceMat;
			if (mEntity->hasComponent<G2::TransformComponent>())
			{
				auto* tc = mEntity->getComponent<G2::TransformComponent>();
				worldSpaceMat = tc->getWorldSpaceMatrix();
				pos = worldSpaceMat * pos;
			}
			auto* rc = mEntity->getComponent<G2::RenderComponent>();
			for (unsigned int i = 0; i < rc->getNumDrawCalls(); ++i)
			{
				G2::AABB const& aabb = rc->getDrawCall(i).getModelSpaceAABB();
				fullAABB.merge(aabb);
			}
			auto* camTc = mEditorCamera.getComponent<G2::TransformComponent>();
			glm::vec4 aabbCenter = worldSpaceMat * glm::vec4(fullAABB.getCenter(), 1.f);
			glm::vec4 halfExtends = worldSpaceMat * glm::vec4(fullAABB.getHalfExtends(), 0.f);
			camTc->setPosition(glm::vec3(-aabbCenter + (glm::length(halfExtends) * mEditorCamera.getViewVec())));
		}
	}
}