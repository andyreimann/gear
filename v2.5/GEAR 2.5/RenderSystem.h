// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once

#include "EffectImporter.h"
#include "AABB.h"
#include "RenderStatesGroup.h"
#include "ZSorter.h"

#include <G2Core/BaseSystem.h>
#include <G2Core/Entity.h>

#include <memory>
#include <glm/glm.hpp>
#include <unordered_set>
#include <map>

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
	typedef std::vector<std::shared_ptr<RenderTarget>> PostProcessingRenderTargets;
	typedef std::vector<std::shared_ptr<RenderStatesGroup>> RenderStatesGroups;
	
	/** This class defines the whole render pipeline of the GEAR engine.
	 * It render all registered RenderComponents with their settings and all passes.
	 * The rendering takes place in the 'render' phase.
	 * @created:	2014/01/30
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class RenderSystem : public BaseSystem<RenderSystem,RenderComponent> 
	{
			friend class RenderComponent;
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
			/** This function will add the given Effect to the end of the post processing pipeline
			 * @param effect The Effect to add.
			 */
			void addPostProcessingEffect(std::shared_ptr<Effect> effect);
			/** Schedules the RenderComponent registered under the given entity id to recalculate it's AABB within the next "update" phase. 
			 * @param entityId The entity id to recalculate the RenderComponent for.
			 * @note Not thread safe.
			 */
			void scheduleAABBRecalculation(unsigned int entityId);
			/** This function will update the transparency mode for the given entity ids RenderComponent.
			 * @note Transparent RenderComponents are treated different in terms of rendering.
			 * They are rendered depending on their distance to the render camera to reduce rendering artifacts.
			 * Use this function to register and unregister a RenderComponent from being treated as transparent.
			 * @warning The engine itself will register/unregister the RenderComponent objects when setting the material properties or adding/removing vertex array objects,
			 * thus this function should normally not be called unless you exactly know what you do!
			 */
			void updateTransparencyMode(unsigned int entityId, bool transparent);
			/** This function will return the clear color to use for clearing the render frame buffer. 
			* @return The clear color.
			*/
			glm::vec4 const& getClearColor() const { return mClearColor; }
			/** This function will set the clear color to use for clearing the render frame buffer. 
			* @param value The clear color.
			*/
			void setClearColor(glm::vec4 const& value) { mClearColor = value; }

			/** This function performs an intersection of a ray and all G2::RenderComponent objects.
			* @param ray The ray which is used for the intersection calculation.
			* @return A pair of an intersection point, if some intersection was found and a boolean indicating if an intersection was found. The boolean is true, if the intersection point is a real intersection point and false if not.
			* @TODO we have to return a struct here containing the entity-id, drawCall, vao index, ... of the entity the intersection comes from.
			*/
			std::pair<glm::vec4, bool> intersect(G2::Ray const& ray);


			~RenderSystem();
		private:
			/** This function will perform the complete forward rendering pipeline.
			 * This does not include rendering the passes of the RenderComponent objects Effects!
			 */
			void _renderForward(
				CameraComponent* mainCamera,
				glm::mat4 const& cameraSpaceMatrix,
				glm::mat4 const& inverseCameraRotation,
				TransformSystem* transformSystem,
				LightSystem* lightSystem
			);
			/** This function will perform the complete deferred rendering pipeline.
			 * This does not include rendering the passes of the RenderComponent objects Effects!
			 */
			void _renderDeferred(
				CameraComponent* mainCamera,
				glm::mat4 const& cameraSpaceMatrix,
				glm::mat4 const& inverseCameraRotation,
				TransformSystem* transformSystem,
				LightSystem* lightSystem
			);
			/** This function will perform the complete pass rendering pipeline.
			 */
			void _renderPasses(
				CameraComponent* mainCamera,
				glm::vec3 const& cameraPosition,
				glm::mat4 const& cameraSpaceMatrix,
				glm::mat4 const& inverseCameraRotation,
				TransformSystem* transformSystem,
				LightSystem* lightSystem
			);
			/** This function will render all components with the given parameters.
			 * It will also perform frustum culling before rendering.
			 * It is used generic for rendering the RenderComponent objects in passes, deferred rendering and normal rendering.
			 * @note This function will take care of rendering transparent vertex arrays in a proper way.
			 */
			void _renderAllComponents(
				Frustum const* frustum,
				glm::mat4 const& projectionMatrix,
				glm::mat4 const& cameraSpaceMatrix,
				glm::mat4 const& inverseCameraRotation,
				glm::vec4 const& cameraPosition,
				TransformSystem* transformSystem,
				LightSystem* lightSystem,
				G2::Pass const* pass = nullptr,
				unsigned int entityIdToSkip = Entity::UNINITIALIZED_ENTITY_ID
			);
			/** This function will perform the rendering for one single RenderComponent.
			 * @note If vertexArrayObjectIndex is -1, all vertex array objects are draw, otherwise, only the given index is drawn
			 */
			void _render(
				glm::mat4 const& projectionMatrix, 
				glm::mat4 const& cameraSpaceMatrix, 
				glm::mat4 const& inverseCameraRotation,
				RenderComponent* component,
				std::shared_ptr<Shader>& boundShader,
				TransformSystem* transformSystem,
				LightSystem* lightSystem,
				int drawCallToDraw = -1
			);
			/** This function will upload the matrices to the given shader.
			 */
			void _uploadMatrices(
				std::shared_ptr<Shader>& shader, 
				TransformComponent* transformation, 
				glm::mat4 const& projectionMatrix, 
				glm::mat4 const& cameraSpaceMatrix,
				glm::mat4 const& inverseCameraRotation,
				bool billboarding
			);
			/** This function will upload the lights to the given shader.
			 */
			void _uploadLight(std::shared_ptr<Shader>& shader, LightComponent* light, glm::mat4 const& cameraSpaceMatrix, int index);
			/** This function will upload the material to the given shader.
			 */
			void _uploadMaterial(std::shared_ptr<Shader>& shader, Material* material);
			/** This function will recalculate the model space AABBs for the given RenderComponent.
			 */
			void _recalculateModelSpaceAABB(RenderComponent* component, TransformSystem* transformSystem);
			
			void _recalculateWorldSpaceAABB(RenderComponent* component, int drawCallIndex, TransformSystem* transformSystem);
			
			std::shared_ptr<Shader> _getRenderShader(RenderComponent* component);
			std::shared_ptr<Shader> _getPassRenderShader(RenderComponent* component, Pass const* pass) const;

			/** This function performs the frustum culling for all vertex array objects
			 * contained in the RenderComponent.
			 * @param comp The RenderComponent to perform frustum culling for.
			 * @param frustum The frustum to cull against.
			 * @return True if at least one vertex array object was not culled, false if all were culled.
			 * @note This function will cache the result for every vertex array object inside of the RenderComponent in the mVaosFrustumCulled member!
			 */
			bool _performFrustumCulling(RenderComponent* comp, Frustum const* frustum);
			/** This function is called automatically whenever the viewport of the render window performed a resize.
			 */
			void _onViewportResize(int w, int h);
			/** This function is called from a RenderComponent if a resize for the vertex array objects occurs.
			 * @param entityId The entity id of the RenderComponent
			 * @param sizeDifference The difference in size of the vertex array objects.
			 */
			void _onDrawCallResize(unsigned int entityId, int sizeDifference);
			/** This function is called from the BaseSystem whenever a component was removed from the BaseSystem.
			 * @param entityId The ID of the Entity, that was removed.
			 * @note The BaseSystem components are locked when this function is called, so further modifications are not permitted!
			 */
			virtual void onComponentAdded(unsigned int entityId) override;
			/** This function is called from the BaseSystem whenever a component was removed from the BaseSystem.
			 * @param entityId The ID of the Entity, that was removed.
			 * @note The BaseSystem components are locked when this function is called, so further modifications are not permitted!
			 */
			virtual void onComponentRemoved(unsigned int entityId) override;
			/** This function will add the given RenderComponent to a RenderStatesGroup containing the given RenderStates.
			 * @param component The RenderComponent to add to the RenderStatesGroup
			 * @param newRenderStates The RenderStates of the RenderStatesGroup, the RenderComponent should be added to.
			 */
			void _updateRenderStatesGroup(RenderComponent* component, RenderStates* newRenderStates);
			/** This function will delete all RenderStatesGroups, which do not contain any RenderComponent entity id anymore.
			 */
			void _deleteEmptyRenderStatesGroups();
			/** Updates the uniform variables requested by the shader of the given post processing Effect
			 * to the last reported state.
			 * @param effect The post processing Effect to set the uniforms on.
			 */
			void _updatePostProcessingUniforms(std::shared_ptr<Effect> const& effect) const;

			void _intersect(
				std::pair<glm::vec4, bool>& closestMatch,
				glm::mat4 const& worldSpaceMatrix,
				G2::Ray const& worldSpaceRay, 
				G2::Ray const& modelSpaceRay, 
				glm::vec3 const& p1, 
				glm::vec3 const& p2, 
				glm::vec3 const& p3);
			
			glm::vec4										mClearColor;			// The clear color to use for rendering
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
			
			std::shared_ptr<RenderTarget>					mSceneRenderTarget;				// The render target of the initial scene when entering the post processing chain (only set if at least one post processing Effect is added).
			PostProcessingRenderTargets						mPostProcessingRenderTargets;	// The render target of the post processing pipeline
			int												mCurrentPostProcessingRenderTargetIndex;

			std::list<unsigned int>							mRecalcAABBEntityIds;
			std::unordered_set<unsigned int>				mTransparentEntityIds;			// Entity ids of the RenderComponent objects to treat as transparent while rendering
			
			std::vector<std::pair<unsigned int,int>>		mZSortedTransparentEntityIdsToDrawCall;	// Entity ids of the RenderComponent objects to treat as transparent while rendering -> sorted by their distance to the camera.

			RenderStatesGroups								mRenderSortedComponents;		// All components sorted by render states

			glm::vec2										mLastWindowSize;				// The last known window size
	};
};

