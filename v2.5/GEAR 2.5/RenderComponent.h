// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "RenderSystem.h"
#include "VertexArrayObject.h"
#include "Material.h"
#include "ShaderCache.h"

#include <G2Core/BaseComponent.h>

#include <vector>

namespace G2 
{
	/** This class defines an object to render.
	 * It holds all the needed rendering informations like the geometry, material and more.
	 * @created:	2014/02/01
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class RenderComponent : public BaseComponent<RenderSystem> 
	{
		friend class RenderSystem;

		public:

			RenderComponent() : billboarding(false) {}

			RenderComponent(unsigned int numVertexArrayObjects) : billboarding(false)
			{
				vaos.resize(numVertexArrayObjects);
			}

			Material						material;		// The Material of the RenderComponent
			std::vector<VertexArrayObject>	vaos;			// The vertex array objects of the RenderComponent
			unsigned int					drawMode;		// The OpenGL draw mode to use when rendering
			bool							billboarding;	// The billboarding mode to use when rendering
			/** This function will return the UberShader. 
			* @return The current UberShader.
			*/
			std::shared_ptr<G2::Effect> const& getEffect() const { return effect; }
			/** This function will set the UberShader to the given value.
			* @param value The current UberShader.
			*/
			void setEffect(std::shared_ptr<G2::Effect> const& value) 
			{ 
				effect = value; 
				shaderCache = ShaderCache(); // invalidate
			}
			
		private:
			std::shared_ptr<G2::Effect>		effect;		// The UberShader of the RenderComponent (default is an empty UberShader!)
			ShaderCache						shaderCache;	// The cache used for the Shader
	};
};