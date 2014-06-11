// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once

#include "EffectImporter.h"
#include "AABB.h"

#include <G2Core/BaseSystem.h>

#include <memory>
#include <glm/glm.hpp>

namespace G2 
{
	namespace RenderType
	{
		enum Name {
			FORWARD_RENDERING,
			DEFERRED_SHADING,
		};
		/** This function will parse the given string to the appropriate
		 * RenderTargetType enum value.
		 * @param name The name to parse.
		 * @return The parsed RenderTargetType enum value.
		 */
		RenderTargetType::Name getRenderTargetType(std::string const& name);
	};

	namespace DeferredShadingPass
	{
		enum Name {
			ATTRIBUTES_PASS,
			SHADING_PASS,
		};
		/** This function will parse the given string to the appropriate
		 * RenderTargetType enum value.
		 * @param name The name to parse.
		 * @return The parsed RenderTargetType enum value.
		 */
		RenderTargetType::Name getRenderTargetType(std::string const& name);
	};
	class CameraComponent;
	class RenderComponent;
	class Effect;
	class TransformComponent;
	class TransformSystem;
	class LightComponent;
	class LightSystem;
	class Material;
	class Shader;
	class Pass;
	class Frustum;
	class MultipleRenderTarget;

	typedef std::vector<std::shared_ptr<G2::Effect>> PostProcessingPipeline;
	typedef std::vector<std::shared_ptr<RenderTarget>> PostProcessingPingPongRenderTargets;
	/** This class defines the whole render pipeline of the GEAR engine.
	 * It render all registered RenderComponents with their settings.
	 * The rendering takes place in the 'render' phase.
	 * @created:	2014/01/30
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class RenderSystem : public BaseSystem<RenderSystem,RenderComponent> 
	{
		public:

			RenderSystem();

			void runPhase(std::string const& name, FrameInfo const& frameInfo);
			/** This function will return the DefaultShader. 
			* @return The current DefaultShader.
			*/
			std::shared_ptr<Effect> const& getDefaultEffect() const { return defaultEffect; }
			/** This function will set the DefaultShader to the given value.
			* @param value The current DefaultShader.
			*/
			void setDefaultEffect(std::shared_ptr<Effect> const& value) { defaultEffect = value; }
			/** This function will return the EngineEffectImporter. 
			 * @return The EngineEffectImporter.
			 */
			EffectImporter& getEngineEffectImporter() { return mEngineEffectImporter; }
			/** This function will add the given Effect to the end of the post processing pipeling
			 * @param effect The Effect to add.
			 */
			void addPostProcessingEffect(std::shared_ptr<Effect> effect);
			/** Schedules the RenderComponent registered under the given entity id to recalculate it's AABB within the next "update" phase. 
			 * @param entityId The entity id to recalculate the RenderComponent for.
			 * @note Not thread safe.
			 */
			void scheduleAABBRecalculation(unsigned int entityId);

			~RenderSystem();
		private:
			
			void _renderForward(
				CameraComponent* mainCamera,
				glm::mat4 const& cameraSpaceMatrix,
				glm::mat4 const& inverseCameraRotation,
				TransformSystem* transformSystem,
				LightSystem* lightSystem
			);

			void _renderDeferred(
				CameraComponent* mainCamera,
				glm::mat4 const& cameraSpaceMatrix,
				glm::mat4 const& inverseCameraRotation,
				TransformSystem* transformSystem,
				LightSystem* lightSystem
			);

			void _renderPasses(
				CameraComponent* mainCamera,
				glm::vec3 const& cameraPosition,
				glm::mat4 const& cameraSpaceMatrix,
				glm::mat4 const& inverseCameraRotation,
				TransformSystem* transformSystem,
				LightSystem* lightSystem
			);

			void _render(
				glm::mat4 const& projectionMatrix, 
				glm::mat4 const& cameraSpaceMatrix, 
				glm::mat4 const& inverseCameraRotation,
				RenderComponent* component, 
				std::shared_ptr<Shader>& boundShader,
				TransformSystem* transformSystem,
				LightSystem* lightSystem
			);
			
			void _uploadMatrices(
				std::shared_ptr<Shader>& shader, 
				TransformComponent* transformation, 
				glm::mat4 const& projectionMatrix, 
				glm::mat4 const& cameraSpaceMatrix,
				glm::mat4 const& inverseCameraRotation,
				bool billboarding
			);
			void _uploadLight(std::shared_ptr<Shader>& shader, LightComponent* light, glm::mat4 const& cameraSpaceMatrix, int index);
			void _uploadMaterial(std::shared_ptr<Shader>& shader, Material* material);
			/** This function will recalculate the object space AABBs for the given RenderComponent.
			 */
			void _recalculateAABB(RenderComponent* component, TransformSystem* transformSystem);
			
			std::shared_ptr<Shader> _getRenderShader(RenderComponent* component);
			std::shared_ptr<Shader> _getPassRenderShader(RenderComponent* component, Pass const* pass) const;

			void _onViewportResize(int w, int h);
			/** This function performs the frustum culling for all vertex array objects
			 * contained in the RenderComponent.
			 * @param comp The RenderComponent to perform frustum culling for.
			 * @param frustum The frustum to cull against.
			 * @return True if at least one vertex array object was not culled, false if all were culled.
			 * @note This function will cache the result for every vertex array object inside of the RenderComponent in the mVaosFrustumCulled member!
			 */
			bool _performFrustumCulling(RenderComponent* comp, Frustum const* frustum);


			RenderType::Name								mRenderType;
			std::shared_ptr<G2::Effect>						defaultEffect;			// The default UberShader to use for rendering
			EffectImporter									mEngineEffectImporter;	// The effect importer used when importing default engine effects
			AABB											mWorldAABB;
			DeferredShadingPass::Name						mDeferredShadingPass;
			std::shared_ptr<G2::Effect>						mGBufferEffect;
			std::shared_ptr<G2::Effect>						mShadingEffect;
			std::shared_ptr<MultipleRenderTarget>			mDeferredShadingTarget;
			VertexArrayObject								mFullScreenQuad;
			PostProcessingPipeline							mPostProcessingEffects;			// A vector of shader to use for post processing
			PostProcessingPingPongRenderTargets				mPostProcessingRenderTargets;	// The render target of the post processing pipeline
			int												mCurrentPostProcessingRenderTargetIndex;
			
			std::list<unsigned int>							mRecalcAABBEntityIds;
	};
};