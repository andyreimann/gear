#pragma once

//#define GEAR_QT_5_4

#include <G2/GEAR.h>
#ifndef GEAR_QT_5_4
	#include <QtOpenGL/QGLWidget>
#else
	#include <QOpenGLWidget>
#endif
#include <G2/AbstractWindow.h>
#include <G2Cameras/EditorCamera.h>

#include <unordered_map>

#include <QTimer>
#include <QMatrix4x4>
QT_FORWARD_DECLARE_CLASS(QOpenGLContext)
QT_FORWARD_DECLARE_CLASS(QOpenGLFramebufferObject)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLBuffer)
QT_FORWARD_DECLARE_CLASS(QOpenGLVertexArrayObject)
QT_FORWARD_DECLARE_CLASS(QOffscreenSurface)
QT_FORWARD_DECLARE_CLASS(QQuickRenderControl)
QT_FORWARD_DECLARE_CLASS(QQuickWindow)
QT_FORWARD_DECLARE_CLASS(QQmlEngine)
QT_FORWARD_DECLARE_CLASS(QQmlComponent)
QT_FORWARD_DECLARE_CLASS(QQuickItem)

class EditorGeometryManager;
class Project;
class TransformationHandler;
class TranslationHandler;
class ManagedEntity;
/** This is the OpenGL Context class of the GEAR Editor render surface.
* @created	2014/12/09
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
#ifndef GEAR_QT_5_4
class GLContext : public QGLWidget, public G2::AbstractWindow
#else
class GLContext : public QOpenGLWidget, public G2::AbstractWindow
#endif
{
	Q_OBJECT // must include this if you use Qt signals/slots

	public:
		GLContext(QWidget *parent = 0);

		/** Initializes the editor camera.
		 */
		void createEditorCamera();
		/** Sets the EditorGeometryManager instance holding all necessary editor geometry.
		 * @param editorGeometryManager The EditorGeometryManager instance.
		 */
		void setEditorGeometryManager(std::shared_ptr<EditorGeometryManager> const& editorGeometryManager);
		~GLContext();
	protected:
		void initializeGL();
		void resizeGL(int w, int h);
		void paintGL();
		
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void wheelEvent(QWheelEvent* event);
		
		void keyPressEvent(QKeyEvent *event);
		void keyReleaseEvent(QKeyEvent *event);

		
		virtual void processEvents(int frame) { }
		virtual void swapBuffer() { }
		virtual void setHideMouseMode(bool mode) { }

	private:
		/** This is a callback function which will be invoked whenever a new Project was opened.
		* @param project A pointer to the Project which was opened.
		*/
		void _onProjectOpened(Project* project);
		/** This is a callback function which will be invoked whenever a new ManagedEntity is selected/gains focus.
		* @param entity A pointer to the ManagedEntity which is selected.
		*/
		void _onManagedEntitySelected(ManagedEntity* entity);

		void _onFocusEntity(G2::KeyCode keyCode);

		void initKeyMap();

		std::shared_ptr<EditorGeometryManager>	mEditorGeometryManager; // The EditorGeometryManager holding all necessary editor geometry
		Project*								mProject;			// A pointer to the current project
		G2::FrameInfo							mFrameInfo;
		G2::Entity								mLight;
		G2::EffectImporter						mEffectImporter;
		//G2::FBXImporter						mFbxImporter;
		G2Cameras::EditorCamera					mEditorCamera;
		std::unordered_map<Qt::MouseButton,G2::MouseButton> mMouseButtonMapping;
		std::unordered_map<Qt::Key, int>		mKeyMap;

		std::shared_ptr<TransformationHandler>	mTransformationHandler;
		std::shared_ptr<TranslationHandler>		mTranslationHandler;

		ManagedEntity*							mEntity;

		// QML TEST
		void initQuick();
		QTimer									m_updateTimer;
		bool									m_quickInitialized;
		bool									m_quickReady;
		QOffscreenSurface*						m_offscreenSurface;
		QQuickRenderControl*					m_renderControl;
		QQuickWindow*							m_quickWindow;
		QQmlEngine*								m_qmlEngine;
		QQmlComponent*							m_qmlComponent;
		QQuickItem*								m_rootItem;
		QOpenGLFramebufferObject*				m_fbo;
		QOpenGLShaderProgram*					m_program;
		QOpenGLBuffer*							m_vbo;
		QOpenGLVertexArrayObject*				m_vao;
		int m_matrixLoc;
		bool gearInitialized;
		QMatrix4x4 m_proj;
		void startQuick(const QString &filename);
		void setupVertexAttribs();
		void updateSizes();
	private slots:
		void updateQuick();
		void run();
		void createFbo();
		void destroyFbo();
		void requestUpdate();
	
};

