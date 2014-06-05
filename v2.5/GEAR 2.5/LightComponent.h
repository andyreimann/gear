#pragma once
#include "LightSystem.h"
#include "ShadowDescriptor.h"
#include "AABB.h"
#include "LightEffectState.h"

#include <G2Core/BaseComponent.h>

#include <glm/glm.hpp>
#include <memory>

namespace G2 
{
	class Pass;
	class CameraComponent;
	
	namespace LightType {
		enum Name {
			POSITIONAL,
			SPOT,
			DIRECTIONAL,
		};
	};
	/// This class defines the model for a light.
	/// This can be either a positional or a directional light.
	/// The transformation of a light is managed by a TransformComponent.
	/// If the LightComponent is attached to an Entity, which also has a 
	/// TransformComponent attached, this transformation is used to measure 
	/// the position/orientation.
	/// @created:	2014/04/29
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class LightComponent : public BaseComponent<LightSystem> 
	{
			friend class LightSystem;
			friend class RenderSystem;

		public:
			LightComponent(LightType::Name type);
			/// Changes the direction of the Light and sets the light 
			/// to type DIRECTIONAL.
			LightComponent& setupColor(glm::vec4 const& ambient, glm::vec4 const& diffuse, glm::vec4 const& specular);
			
			LightType::Name const& getType() const { return mType; }
			
			bool isEnabled() const { return mEnabled; }
			
			LightComponent& setEnabled(bool enabled) { mEnabled = enabled; return *this; }
			
			glm::vec4 const& getTransformedPosition() const { return mCachedPosition; }
			
			glm::vec3 const& getTransformedDirection() const { return mCachedDirection; }
			
			glm::vec4	ambient;			// default: (1.0f,1.0f,1.0f,1.0f)
			glm::vec4	diffuse;			// default: (1.0f,1.0f,1.0f,1.0f)
			glm::vec4	specular;			// default: (1.0f,1.0f,1.0f,1.0f)
			float		cutOffDegrees;		// only needed for spot lights. Represents the maximum angle between the light direction and the light to pixel vector for pixels that are under the influence of the spot light.
			float		attenuation;			// The constant attenuation of the Light. Default: 1.0
			float		linearAttenuation;		// The linear attenuation of the Light. Default: 0.0
			float		exponentialAttenuation; // The exponential attenuation of the Light. Default: 0.0
			
			/** Configures the shadow mapping used by this LightComponent.
			 * @param shadowDescriptor The descriptor to use for setting up the shadows.
			 * @return True if the configuration of the shadow information was done, false if not.
			 * @note This function may add a RenderComponent to the Entity this LightComponent 
			 * is attached to if not already existing. Also it will override any assigned effect
			 * of that RenderComponent.
			 * @warning Use this function only if you not plan to implement your own shadow algorithm on top!
			 */
			bool configureShadows(ShadowDescriptor const& shadowDescriptor);
			/** This function will return the LightEffectState. 
			 * @return The LightEffectState.
			 */
			std::shared_ptr<LightEffectState> const& getLightEffectState() const { return mLightEffectState; }

		private:
			/** This function is called from the RenderSystem whenever a Pass
			 * will be rendered, which is attached to a LightComponent.
			 * @param pass The Pass, which will be rendered.
			 * @param mainCamera The main render camera.
			 */
			void _prePassRendering(Pass const* pass, CameraComponent const* mainCamera);
			
			/** This function is called from the RenderSystem whenever a Pass iteration
			 * will be rendered, which is attached to a LightComponent.
			 * @param pass The Pass, which will be rendered.
			 * @param iterationIndex The index of iteration of the pass.
			 * @param mainCamera The main render camera.
			 * @param mainCameraSpaceMatrix The camera space matrix of the main camera.
			 * @param passProjectionMatrix A modifiable reference to the pass projection matrix initialized with the main cameras projection matrix.
			 * @param passCameraSpaceMatrix A modifiable reference to the pass camera space matrix to use.
			 * @param worldAABB The AABB containing all objects in the world.
			 */
			void _prePassIterationRendering(
				Pass const* pass, 
				int iterationIndex, 
				CameraComponent const* mainCamera, 
				glm::mat4 const& mainCameraSpaceMatrix,
				glm::mat4& passProjectionMatrix,
				glm::mat4& passCameraSpaceMatrix,
				AABB const& worldAABB
			);
			
			
			
			void setupSplitDistance(float zNear, float zFar);
			void setupFrustumPoints(int pass, int width, int height, float fovY, glm::mat4 modelView, glm::mat4 const& invCameraTransformation, glm::mat4 const& invCameraRotation);
		




			/** This function is called from within the LightSystem to update the transformed world position of the LightComponent.
			 * @param pos The world coordinate position of the LightComponent.
			 */
			void _updateTransformedPosition(glm::vec4 const& pos);
			/** This function is called from within the LightSystem to update the transformed world direction of the LightComponent.
			 * @param pos The world coordinate direction of the LightComponent.
			 */
			void _updateTransformedDirection(glm::vec3 const& pos);
			glm::vec4 const& _getUntransformedPosition() const { return mDefaultPosition; }
			glm::vec3 const& _getUntransformedDirection() const { return mDefaultDirection; }

			LightType::Name		mType;				// the type of light
			bool				mEnabled;			// Flag indicating whether the light is enabled or not.
			glm::vec4			mCachedPosition;	// The finally transformed position in world space
			glm::vec4			mDefaultPosition;	// The not transformed position in model space
			glm::vec3			mDefaultDirection;	// The not transformed direction in model space
			glm::vec3			mCachedDirection;	// The finally transformed direction in world space
			ShadowDescriptor	mShadowDescriptor;	// The descriptor to use for the shadowing stage for the LightComponent
			std::shared_ptr<LightEffectState> mLightEffectState;
	};
};