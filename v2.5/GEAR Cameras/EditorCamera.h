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

			CAMERADLL_API void setViewport(int width, int height);
			/// This function will return the camera view vector. 
			/// @return The camera view vector.
			CAMERADLL_API glm::vec4 const& getViewVec() const { return mView; }

			CAMERADLL_API EditorCamera& rotate(float xDegrees, float yDegrees);
			CAMERADLL_API EditorCamera& pan(float strafe, float up);
			CAMERADLL_API EditorCamera& moveView(float units);



		private:
		
			CAMERADLL_API void onMouseMove(glm::detail::tvec2<int> const& mouseCoords);
			CAMERADLL_API void onMouseDown(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords);
			CAMERADLL_API void onMouseUp(G2::MouseButton button, glm::detail::tvec2<int> const& mouseCoords);
			CAMERADLL_API void onMouseWheel(int y);
			CAMERADLL_API void onKeyUp(G2::KeyCode keyCode);
			CAMERADLL_API void onKeyDown(G2::KeyCode keyCode);

			G2::AbstractWindow*			mWindow;
			glm::vec4				mView;
			glm::vec4				mUp;
			glm::vec4				mStrafe;

			float					mRotY;
			float					mRotX;

			float					mSpeedBoost;
			float					mViewPlaneTranslationSpeed;
			float					mRotationSpeed;

			glm::detail::tvec2<int> mMouseCoords;
		
			TranslationMode			mTranslationMode;
			RotationMode			mRotationMode;
	};
};