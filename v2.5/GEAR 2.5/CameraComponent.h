// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "BaseComponent.h"
#include "CameraSystem.h"

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
		friend class RenderSystem;
		public:
			/** This will create a new EditorCamera.
			 * @note Don't forget to provide a valid projection matrix with the 
			 * setProjectionMatrix function!
			 */
			CameraComponent(std::string const& name);
			/** This function will return the current camera space matrix (or view/eye space matrix).
			 */
			glm::mat4 const& getCameraSpaceMatrix() const { return mCameraSpaceMatrix; }
			/** This function will return the current normal matrix. 
			* @return The current normal matrix (inverse transpose modelview matrix).
			* @warning: This function is currently not used. Make sure to always use 
			* uniform scaling to calculate normal vector correctly in eye-space!
			*/
			glm::mat3 const& getNormalMatrix() const { return mNormalMatrix; }

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
			/** This function will perform one step into the view vector direction
			 * of the Camera.
			 * @note The step size is calculated with the current movement speed.
			 */
			void stepForward();
			/** This function will perform one step into the negative view vector direction
			 * of the Camera.
			 * @note The step size is calculated with the current movement speed.
			 */
			void stepBackward();
			/** This function will perform one strafe step into the negative strafe vector direction
			 * of the Camera.
			 * @note The step size is calculated with the current movement speed.
			 */
			void stepLeft();
			/** This function will perform one strafe step into the strafe vector direction
			 * of the Camera.
			 * @note The step size is calculated with the current movement speed.
			 */
			void stepRight();
			/** This function will update the rotation of the camera.
			 * @param degreesX The degrees to rotate around the X-Axis.
			 * @param degreesY The degrees to rotate around the Y-Axis.
			 */
			void rotate(float degreesX, float degreesY);
			/** This function will return the camera view vector. 
			* @return The current camera view vector.
			*/
			glm::vec3 const& getViewVector() const { return mViewVector; }
			/** This function will return the camera up vector. 
			* @return The current camera up vector.
			*/
			glm::vec3 const& getUpVector() const { return mUpVector; }
			/** This function will return the camera strafe vector. 
			* @return The current camera strafe vector.
			*/
			glm::vec3 const& getStrafeVector() const { return mStrafeVector; }
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
			/** This function will update the current modelview matrix
			 * of the EditorCamera. It is only called internally when
			 * the transformation was updated.
			 * @return True if the modelview matrix has changed.
			 */
			bool updateModelView();
		private:

			
			float	  mMoveSpeed;			// The speed of the camera movement
			float	  mRotationSpeed;		// The speed of the rotation
			
			glm::vec3 mTranslation;			// The translation of the camera relative to [0,0,0] as a vector
			
			glm::vec3 mUpVector;			// The current camera up vector
			glm::vec3 mViewVector;			// The current camera view vector
			glm::vec3 mStrafeVector;		// The current camera strafe vector

			glm::mat4 mRotation;			// The current rotation matrix of the camera

			glm::mat4 mProjectionMatrix;	// The current camera projection matrix
			int		  mViewportWidth;		// The width of the Viewport
			int		  mViewportHeight;		// The height of the Viewport
			glm::mat4 mCameraSpaceMatrix;	// The current camera modelview matrix

			/** TODO: This normal matrix isn't the one, we need for normal transformation
			 * Normal matrix calculation: (modelview^-1)^T, actually: (view^-1)^T
			 */

			glm::mat3 mNormalMatrix;		// The current camera normal matrix
			bool	  mChanged;				// Indicates whether sth. has changed since the last updateModelView call
	};

};
