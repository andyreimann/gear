// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "RenderSystem.h"
#include "VertexArrayObject.h"
#include "Material.h"
#include "ShaderCache.h"
#include "AABB.h"
#include "ShadowDescriptor.h"
#include "RenderStatesGroup.h"

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
			/** Returns the number of vertex array objects, this RenderComponent maintains.
			 * @return The number of vertex array objects, this RenderComponent maintains.
			 */
			unsigned int getNumVertexArrays() const { return (unsigned int)mVaos.size(); }
			/** Returns the vertex array object at the given index.
			 * @param index The index to get the vertex array object for.
			 * @note Make sure to not request an index, which does not exist!
			 */
			VertexArrayObject& getVertexArray(unsigned int index) { return mVaos[(size_t)index]; }
			/** This function will allocate the given number of vertex array objects for the RenderComponent.
			 * If the given number is smaller than the current amount, some or all vertex array objects may be deleted.
			 * @param numVertexArrayObjects The number of vertex array objects to allocate.
			 * @note A call to this function will register the RenderComponent to update it's axis aligned bounding boxes in the next "update" phase (even if no resize occured, because size already fits).
			 */
			void allocateVertexArrays(unsigned int numVertexArrayObjects);

			Material						material;		// The Material of the RenderComponent
			
			unsigned int					drawMode;		// The OpenGL draw mode to use when rendering
			bool							billboarding;	// The billboarding mode to use when rendering
			std::vector<AABB>				objectSpaceAABBs;// The object space axis aligned bounding box
			std::vector<AABB>				worldSpaceAABBs;// The world space axis aligned bounding box (object space aabb transformed)
			bool							aabbAnimationRecalc; // Flag indicating if the aabb should be recalculated with the animation

			~RenderComponent();
		private:
			ShaderCache& _getShaderCache() { return mShaderCache; }
			/** This function will update the linkage to a RenderStatesGroup for the RenderComponent.
			 * It will also erase the linbkage to a previous one.
			 * @param newGroup the new RenderStatesGroup the RenderComponent should be linked to.
			 * @note This function is normally only called from the RenderSystem.
			 */
			void _updateRenderStatesGroupLinkage(std::shared_ptr<RenderStatesGroup> newGroup);
			
			std::vector<bool>				mVaosFrustumCulled;		// The frustum culling flag of the vertex array objects of the RenderComponent
			std::vector<VertexArrayObject>	mVaos;					// The vertex array objects of the RenderComponent
			std::shared_ptr<G2::Effect>		mEffect;				// The UberShader of the RenderComponent (default is an empty UberShader!)
			ShaderCache						mShaderCache;			// The cache used for the Shader
			std::shared_ptr<RenderStatesGroup> mRenderStatesGroup;	// The group of entities sharing the same RenderStates the RenderComponent belongs to (set and maintained by the RenderSystem!)
	};
};