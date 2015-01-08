#include "GLContext.h"
#include "Project.h"
#include "GEARStudioEvents.h"
#include "EditorGeometryManager.h"
#include "TranslationHandler.h"

#include <QtGui/QMouseEvent>
#include <QtCore/QTimer>

#include <G2/RenderComponent.h>
#include <G2Core/ECSManager.h>
#include <G2Core/EventDistributer.h>


static std::string ASSET_PATH = "../../tests/bin/Assets/";

GLContext::GLContext(QWidget *parent) 
	: QGLWidget(parent), 
	mEditorCamera(this),
	G2::AbstractWindow("GEAR Editor", 1024, 768, false),
	mProject(nullptr),
	mEditorGeometryManager(nullptr),
	mEntity(nullptr)
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

void GLContext::initializeGL() 
{
	// Initialize the GEAR Gfx device - this cannot hapen earlier
	G2_gfxDevice()->init(nullptr);

	setMouseTracking(true);
	
	mMouseButtonMapping.insert(std::make_pair(Qt::LeftButton,G2::MOUSE_LEFT));
	mMouseButtonMapping.insert(std::make_pair(Qt::MiddleButton,G2::MOUSE_MIDDLE));
	mMouseButtonMapping.insert(std::make_pair(Qt::RightButton,G2::MOUSE_RIGHT));
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
	G2_singleFrame(*this, mFrameInfo);
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
}

void
GLContext::setEditorGeometryManager(std::shared_ptr<EditorGeometryManager> const& editorGeometryManager)
{
	mEditorGeometryManager = editorGeometryManager;

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