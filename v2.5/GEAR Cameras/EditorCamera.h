#pragma once
#include "Defines.h"

#include <G2Core/Entity.h>
#include <G2Core/MouseButton.h>
#include <G2Core/KeyCodes.h>
#include <glm/glm.hpp>

namespace G2
{
	class AbstractWindow;
}
namespace G2Cameras
{
	enum TranslationMode
	{
		NO_TRANSLATION,
		XZ_PLANE,
		VIEW_PLANE,
	};
	enum RotationMode
	{
		NO_ROTATION,
		AROUND_LOCATION,
	};
	/// This class implements a camera simulating a behavior used in an editor.
	/// @created:	2014/05/16
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class EditorCamera : public G2::Entity
	{
		public:
			/// This constructs a new EditorCamera.
			CAMERADLL_API EditorCamera(G2::AbstractWindow* window);
			/// Basic move constructor.
			CAMERADLL_API EditorCamera(EditorCamera && rhs);
			/// Basic move operator.
			CAMERADLL_API EditorCamera& operator=(EditorCamera && rhs);
			/// normal destructor
			CAMERADLL_API ~EditorCamera();
			/** Updates the viewport of the camera.
			 * @param width The width of the viewport.
			 * @param height The height of the viewport.
			 */
			CAMERADLL_API void setViewport(int width, int height);
			/** Updates the cameras perspective projection matrix.
			 * @param fovY The cameras opening angle.
			 * @param zNear The near clip plane distance.
			 * @param zFar The far clip plane distance.
			 */
			CAMERADLL_API void setInternals(float fovY, float zNear, float zFar);

			/// This function will return the camera view vector. 
			/// @return The camera view vector.
			CAMERADLL_API glm::vec4 const& getViewVec() const { return mView; }
			/** Rotates the camera around it's position.
			 * @param xDegrees The degrees to rotate around the local X-Axis of the camera.
			 * @param yDegrees The degrees to rotate around the local Y-Axis of the camera.
			 */
			CAMERADLL_API EditorCamera& rotate(float xDegrees, float yDegrees);
			/** Translates the camera on it's current view plane.
			 * @param strafe The translation on the local X-Axis.
			 * @param up The translation on the local Y-Axis.
			 */
			CAMERADLL_API EditorCamera& translate(float strafe, float up);
			/** Zooms into the cameras view vector.
			 * @param units the translation to move the camera into the direction of it's view vector.
			 */
			CAMERADLL_API EditorCamera& zoom(float units);
			/** Pauses the cameras internal event processing.
			 */
			CAMERADLL_API EditorCamera& pause();
			/** Unpauses the cameras internal event processing.
			*/
			CAMERADLL_API EditorCamera& unpause();
		private:
		
			CAMERADLL_API void onMouseMove(glm::detail::tvec2<int> const& mouseCoords);
			CAMERADLL_API void onMouseDown(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords);
			CAMERADLL_API void onMouseUp(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords);
			CAMERADLL_API void onMouseWheel(int y);
			CAMERADLL_API void onKeyUp(G2::KeyCode keyCode);
			CAMERADLL_API void onKeyDown(G2::KeyCode keyCode);

			bool					mPaused;
			G2::AbstractWindow*		mWindow;
			glm::vec4				mView;
			glm::vec4				mUp;
			glm::vec4				mStrafe;

			float					mRotY;
			float					mRotX;
			
			float					mFovY;
			float					mZNear;
			float					mZFar;

			float					mSpeedBoost;
			float					mViewPlaneTranslationSpeed;
			float					mRotationSpeed;

			glm::detail::tvec2<int> mMouseCoords;
		
			TranslationMode			mTranslationMode;
			RotationMode			mRotationMode;
	};
};