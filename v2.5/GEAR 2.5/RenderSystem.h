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
	/** This class defines the whole render pipeline of the GEAR engine.
	 * It render all registered RenderComponents with their settings.
	 * The rendering takes place in the 'render' phase.
	 * @created:	2014/01/30
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class RenderSystem : public BaseSystem<RenderSystem,RenderComponent> 
	{
		public:
			void runPhase(std::string const& name, FrameInfo const& frameInfo);
			/** This function will return the DefaultShader. 
			* @return The current DefaultShader.
			*/
			std::shared_ptr<G2::Effect> const& getDefaultEffect() const { return defaultEffect; }
			/** This function will set the DefaultShader to the given value.
			* @param value The current DefaultShader.
			*/
			void setDefaultEffect(std::shared_ptr<G2::Effect> const& value) { defaultEffect = value; }
			/// This function will return the EngineEffectImporter. 
			/// @return The EngineEffectImporter.
			G2::EffectImporter& getEngineEffectImporter() { return mEngineEffectImporter; }
		private:

			void renderPasses(
				CameraComponent* mainCamera,
				glm::vec3 const& cameraPosition,
				glm::mat4 const& cameraSpaceMatrix,
				glm::mat4 const& inverseCameraRotation,
				TransformSystem* transformSystem,
				LightSystem* lightSystem
			);

			void render(
				glm::mat4 const& projectionMatrix, 
				glm::mat4 const& cameraSpaceMatrix, 
				glm::mat4 const& inverseCameraRotation,
				RenderComponent* component, 
				std::shared_ptr<Shader>& boundShader,
				TransformSystem* transformSystem,
				LightSystem* lightSystem,
				Frustum const* frustum
			);
			
			void uploadMatrices(
				std::shared_ptr<Shader>& shader, 
				TransformComponent* transformation, 
				glm::mat4 const& projectionMatrix, 
				glm::mat4 const& cameraSpaceMatrix,
				glm::mat4 const& inverseCameraRotation,
				bool billboarding
			);
			void uploadLight(std::shared_ptr<Shader>& shader, LightComponent* light, glm::mat4 const& cameraSpaceMatrix, int index);
			void uploadMaterial(std::shared_ptr<Shader>& shader, Material* material);
			
			void initializeAABB(RenderComponent* component, TransformSystem* transformSystem);
			
			std::shared_ptr<Shader> getRenderShader(RenderComponent* component);
			std::shared_ptr<Shader> getPassRenderShader(RenderComponent* component, Pass const* pass) const;

			std::shared_ptr<G2::Effect>						defaultEffect;			// The default UberShader to use for rendering
			EffectImporter									mEngineEffectImporter;	// The effect importer used when importing default engine effects

			AABB											mWorldAABB;

	};
};