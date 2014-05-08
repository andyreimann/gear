// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "BaseSystem.h"


#include <memory>
#include <vector>

namespace G2 
{
	class RenderComponent;
	class CameraComponent;
	class Effect;
	class TransformComponent;
	class LightComponent;
	class Material;
	class Shader;
	class Pass;
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

		private:

			void render(CameraComponent* camera, RenderComponent* component, std::shared_ptr<Shader>& boundShader);
			
			void uploadMatrices(std::shared_ptr<Shader>& shader, TransformComponent* transformation, CameraComponent* camera);
			void uploadLight(std::shared_ptr<Shader>& shader, LightComponent* light, CameraComponent* camera, int index);
			void uploadMaterial(std::shared_ptr<Shader>& shader, Material* material);
			std::shared_ptr<Shader> getRenderShader(RenderComponent* component);
			std::shared_ptr<Shader> getPassRenderShader(RenderComponent* component, Pass const* pass) const;

			std::shared_ptr<G2::Effect>						defaultEffect;		// The default UberShader to use for rendering
	};
};