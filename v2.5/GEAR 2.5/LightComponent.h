#pragma once
#include "BaseComponent.h"
#include "LightSystem.h"
#include <glm/glm.hpp>

namespace G2 
{
	
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

		private:
			void _updateTransformedPosition(glm::vec4 const& pos);
			void _updateTransformedDirection(glm::vec3 const& pos);
			glm::vec4 const& _getUntransformedPosition() const { return mDefaultPosition; }
			glm::vec3 const& _getUntransformedDirection() const { return mDefaultDirection; }

			LightType::Name mType;		// the type of light
			bool	  mEnabled;			// Flag indicating whether the light is enabled or not.
			glm::vec4 mCachedPosition;	// The finally transformed position in world space
			glm::vec4 mDefaultPosition; // The not transformed position in model space
			glm::vec3 mDefaultDirection;// The not transformed direction in model space
			glm::vec3 mCachedDirection;	// The finally transformed direction in world space
	};
};