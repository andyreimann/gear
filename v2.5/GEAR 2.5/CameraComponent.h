// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "CameraSystem.h"
#include "Frustum.h"

#include <G2Core/BaseComponent.h>

#include <glm/glm.hpp>


namespace G2 
{
	
	/** This class encapsulates and handles the movement of a camera inside the Editor s
	 * render window for the current EditableScene.
	 * @created:	2014/01/19
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class CameraComponent : public BaseComponent<CameraSystem> 
	{
		friend class CameraSystem;
		public:
			/** This will create a new EditorCamera.
			 * @note Don't forget to provide a valid projection matrix with the 
			 * setProjectionMatrix function!
			 */
			CameraComponent(std::string const& name);
			/** Move constructor to move a CameraComponent.
			 */
			CameraComponent(CameraComponent && rhs);
			/** Move assignment operator to move a CameraComponent.
			 */
			CameraComponent& operator=(CameraComponent && rhs);
			/** This function will return the current projection matrix.
			 */
			glm::mat4 const& getProjectionMatrix() const { return mProjectionMatrix; }
			/** This function will set the projection matrix to the given value.
			* @param value The projection matrix.
			* @param width The width of the viewport
			* @param height The height of the viewport
			*/
			void setProjectionMatrix(glm::mat4 const& value, int width, int height);
			/** This function will return the ViewportWidth. 
			* @return The ViewportWidth.
			*/
			int const& getViewportWidth() const { return mViewportWidth; }
			/** This function will return the ViewportHeight. 
			* @return The ViewportHeight.
			*/
			int const& getViewportHeight() const { return mViewportHeight; }
			/** This function will return the rotation speed. 
			* @return The current rotation speed.
			*/
			float getRotationSpeed() const { return mRotationSpeed; }
			/** This function will set the rotation speed to the given value.
			* @param value The current rotation speed.
			*/
			void setRotationSpeed(float value) { mRotationSpeed = value; }
			/** This function will return the movement speed. 
			* @return The current movement speed.
			*/
			float getMoveSpeed() const { return mMoveSpeed; }
			/** This function will set the movement speed to the given value.
			* @param value The current movement speed.
			*/
			void setMoveSpeed(float value) { mMoveSpeed = value; }
			/** This function will make this CameraComponent the one to use for rendering.
			 */
			void setAsRenderCamera();
			/// This function will return the inverse camera rotation. 
			/// @return The inverse camera rotation.
			glm::mat4 const& getInverseCameraRotation() const { return mInverseCameraRotation; }
			/// This function will return the Frustum. 
			/// @return The Frustum of the CameraComponent.
			G2::Frustum const& getFrustum() const { return mFrustum; }
		private:
			/// This function will set the InverseCameraRotation to the given value.
			/// @param value The InverseCameraRotation.
			void _setInverseCameraRotation(glm::mat4 const& value) { mInverseCameraRotation = value; }
			/// Updates the Frustum with the given model view matrix.
			/// @param modelViewMatrix The model view matrix to update the frustum with.
			void _updateFrustum(glm::mat4 const& modelViewMatrix);
			std::string mName;					// The name of the Camera
			float	  mMoveSpeed;				// The speed of the camera movement
			float	  mRotationSpeed;			// The speed of the rotation
			
			glm::mat4 mProjectionMatrix;		// The current camera projection matrix
			glm::mat4 mInverseCameraRotation;	// The inverse camera rotation matrix needed for several effects
			int		  mViewportWidth;			// The width of the Viewport
			int		  mViewportHeight;			// The height of the Viewport
			Frustum   mFrustum;					// The Frustum of the Camera.
	};

};
