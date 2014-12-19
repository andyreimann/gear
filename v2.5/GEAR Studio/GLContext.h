#pragma once
#include <G2/GEAR.h>
#include <QtOpenGL/QGLWidget>
#include <G2/AbstractWindow.h>
#include <G2Cameras/EditorCamera.h>

#include <unordered_map>

/** This is the OpenGL Context class of the GEAR Editor rendersurface.
* @created	2014/12/09
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class GLContext : public QGLWidget, public G2::AbstractWindow
{
	Q_OBJECT // must include this if you use Qt signals/slots

	public:
		GLContext(QWidget *parent = 0);
		/** Loads a default scene which is only needed for developing
		 * purpose. Later this class shouldn't load any resources.
		 */
		void loadDefaultScene();

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
		void initKeyMap();

		G2::FrameInfo				mFrameInfo;
		G2::Entity					mLight;
		G2::EffectImporter			mEffectImporter;
		//G2::FBXImporter				mFbxImporter;
		G2Cameras::EditorCamera		mEditorCamera;
		std::unordered_map<Qt::MouseButton,G2::MouseButton> mMouseButtonMapping;
		std::unordered_map<Qt::Key,int>						mKeyMap;
};

