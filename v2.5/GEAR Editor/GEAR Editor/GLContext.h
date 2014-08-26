#pragma once
#include <G2/GEAR.h>
#include <QGLWidget>
#include <G2/AbstractWindow.h>
#include <G2Cameras/EditorCamera.h>

#include <unordered_map>

class GLContext : public QGLWidget, public G2::AbstractWindow
{
	Q_OBJECT // must include this if you use Qt signals/slots

	public:
		GLContext(QWidget *parent = 0);
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

		
		virtual void processEvents(int frame) { }
		virtual void swapBuffer() { }
		virtual void setHideMouseMode(bool mode) { }

		G2::FrameInfo				mFrameInfo;
		std::shared_ptr<G2::FBXMesh> mLight;
		G2::EffectImporter			mEffectImporter;
		G2::FBXImporter				mFbxImporter;
		G2Cameras::EditorCamera		mEditorCamera;
		std::unordered_map<Qt::MouseButton,G2::MouseButton> mMouseButtonMapping;
};

