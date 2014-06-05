// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "RenderSystem.h"
#include "VertexArrayObject.h"
#include "Material.h"
#include "ShaderCache.h"
#include "AABB.h"
#include "ShadowDescriptor.h"

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

			RenderComponent();

			RenderComponent(unsigned int numVertexArrayObjects);
			/** Move constructor to move a RenderComponent.
			 */
			RenderComponent(RenderComponent && rhs);
			/** Move assignment operator to move a RenderComponent.
			 */
			RenderComponent& operator=(RenderComponent && rhs);
			/** This function will return the UberShader. 
			* @return The current UberShader.
			*/
			std::shared_ptr<G2::Effect> const& getEffect() const { return mEffect; }
			/** This function will set the UberShader to the given value.
			* @param value The current UberShader.
			*/
			void setEffect(std::shared_ptr<G2::Effect> const& value);

			Material						material;		// The Material of the RenderComponent
			std::vector<VertexArrayObject>	vaos;			// The vertex array objects of the RenderComponent
			unsigned int					drawMode;		// The OpenGL draw mode to use when rendering
			bool							billboarding;	// The billboarding mode to use when rendering
			std::vector<AABB>				objectSpaceAABBs;// The object space axis aligned bounding box
			std::vector<AABB>				worldSpaceAABBs;// The world space axis aligned bounding box (object space aabb transformed)
			bool							aabbAnimationRecalc; // Flag indicating if the aabb should be recalculated with the animation
		private:
			ShaderCache& _getShaderCache() { return mShaderCache; }

			std::shared_ptr<G2::Effect>		mEffect;		// The UberShader of the RenderComponent (default is an empty UberShader!)
			ShaderCache						mShaderCache;	// The cache used for the Shader
	};
};