// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "RenderSystem.h"
#include "IndexArrayObject.h"
#include "Material.h"
#include "ShaderCache.h"
#include "AABB.h"
#include "ShadowDescriptor.h"
#include "RenderStatesGroup.h"
#include "DrawCall.h"

#include <G2Core/BaseComponent.h>
#include <G2Core/Event.h>

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
			/** Returns the VertexArrayObject at the given index.
			 * @param index The index to get the VertexArrayObject for.
			 * @note Make sure to not request an index, which does not exist!
			 */
			VertexArrayObject& getVertexArray(unsigned int index) { return mVaos[(size_t)index]; }
			/** Returns the IndexArrayObject at the given index.
			 * @param index The index to get the vertex array object for.
			 * @note Make sure to not request an index, which does not exist!
			 */
			IndexArrayObject& getIndexArray(unsigned int index) { return mIaos[(size_t)index]; }
			/** This function will calculate the tangent vectors and the binormal vectors for the VertexArrayObjects in the RenderComponents.
			 * @note It expects and supports only triangle vertex data!
			 * @param vertexSemantic The semantics to interpret as vertex positions.
			 * @param texCoordsSemantic The semantics to interpret as texture coordinates.
			 */
			void calculateBinormalsAndTangents(Semantics::Name vertexSemantic = Semantics::POSITION, Semantics::Name texCoordsSemantic = Semantics::TEXCOORD_0);
			/** This function will allocate the given number of VertexArrayObjects for the RenderComponent.
			 * If the given number is smaller than the current amount, some or all vertex array objects may be deleted.
			 * @param numVertexArrayObjects The number of vertex array objects to allocate.
			 */
			void allocateVertexArrays(unsigned int numVertexArrayObjects);
			/** This function will allocate the given number of IndexArrayObjects for the RenderComponent.
			 * If the given number is smaller than the current amount, some or all IndexArrayObjects may be deleted.
			 * @param numIndexArrays The number of IndexArrayObjects to allocate.
			 */
			void allocateIndexArrays(unsigned int numIndexArrays);
			/** This function will return the number of draw calls, the VertexArrayObject will invoke
			 * when the draw() function is called.
			 * @return The number of draw calls, the RenderComponent will invoke
			 * when the draw() function is called.
			 */
			unsigned int getNumDrawCalls() const;
			/** This function will return the PolygonOffsetUnits. 
			* @return The PolygonOffsetUnits.
			*/
			float const& getPolygonOffsetUnits() const { return mRenderStatesGroup->getRenderStates().getPolygonOffsetUnits(); }
			/** This function will set the PolygonOffsetUnits to the given value.
			* @param value The PolygonOffsetUnits.
			*/
			void setPolygonOffsetUnits(float const& value);
			/** This function will return the PolygonOffsetFactor. 
			* @return The PolygonOffsetFactor.
			*/
			float const& getPolygonOffsetFactor() const { return mRenderStatesGroup->getRenderStates().getPolygonOffsetFactor(); }
			/** This function will set the PolygonOffsetFactor to the given value.
			* @param value The PolygonOffsetFactor.
			*/
			void setPolygonOffsetFactor(float const& value);
			/** This function will return the PolygonDrawMode. 
			* @return The PolygonDrawMode.
			*/
			PolygonDrawMode::Name const& getPolygonDrawMode() const { return mRenderStatesGroup->getRenderStates().getPolygonDrawMode(); }
			/** This function will set the PolygonDrawMode to the given value.
			* @param value The PolygonDrawMode.
			*/
			void setPolygonDrawMode(PolygonDrawMode::Name const& value);
			/** This function will return the FaceCulling. 
			* @return The FaceCulling.
			*/
			FaceCulling::Name const& getFaceCulling() const { return mRenderStatesGroup->getRenderStates().getFaceCulling(); }
			/** This function will set the FaceCulling to the given value.
			* @param value The FaceCulling.
			*/
			void setFaceCulling(FaceCulling::Name const& value);
			/** This function will return the SourceFactor. 
			* @return The SourceFactor.
			*/
			BlendFactor::Name const& getSourceBlendFactor() const { return mRenderStatesGroup->getRenderStates().getSourceFactor(); }
			/** This function will set the SourceFactor to the given value.
			* @param value The SourceFactor.
			*/
			void setSourceBlendFactor(BlendFactor::Name const& value);
			/** This function will return the DestinationFactor. 
			* @return The DestinationFactor.
			*/
			BlendFactor::Name const& getDestinationBlendFactor() const { return mRenderStatesGroup->getRenderStates().getDestinationFactor(); }
			/** This function will set the DestinationFactor to the given value.
			* @param value The DestinationFactor.
			*/
			void setDestinationBlendFactor(BlendFactor::Name const& value);
			/** Schedules this RenderComponent in the RenderSystem to recalculate the model space
			 * AABBs for all DrawCalls using a distinct AABBCalculationMode than MANUAL.
			 * @note Use this function with caution
			 */
			void scheduleAABBRecalculation() const;
			/** Adds the given DrawCall to the end of the current array of DrawCalls.
			 * @param drawCall the DrawCall to add.
			 * @warning Due to performance reasons it is not checked if the RenderComponent can provide the referenced VertexArrayObject index 
			 * as well as the IndexArrayObject index.
			 */
			void addDrawCall(DrawCall const& drawCall);

			DrawCall& getDrawCall(int index) {return mDrawCalls[index]; }

			Material						material;		// The Material of the RenderComponent
			
			bool							billboarding;	// The billboarding mode to use when rendering
			
			Event<
				RenderComponent*,
				unsigned int,
				unsigned int,
				unsigned int&,
				glm::mat4 const&,
				std::shared_ptr<Shader> const&,
				bool&
			>	renderDrawCallEvent; // 1 = pointer to RenderComponent, 2 = VAO index, 3 = draw call index, 4 = draw mode, 5 = camera space matrix, 6 = bound shader, 7 = render draw call flag

			~RenderComponent();
		private:
			ShaderCache& _getShaderCache() { return mShaderCache; }
			/** This function will update the linkage to a RenderStatesGroup for the RenderComponent.
			 * It will also erase the linkage to a previous one.
			 * @param newGroup the new RenderStatesGroup the RenderComponent should be linked to.
			 * @note This function is normally only called from the RenderSystem.
			 */
			void _updateRenderStatesGroupLinkage(std::shared_ptr<RenderStatesGroup> newGroup);
			
			RenderSystem*					mRenderSystem;
			
			std::vector<VertexArrayObject>	mVaos;					// The VertexArrayObject array of the RenderComponent
			std::vector<IndexArrayObject>	mIaos;					// The IndexArrayObject array of the RenderComponent
			std::vector<DrawCall>			mDrawCalls;				// The DrawCall array of the RenderComponent
			
			std::shared_ptr<G2::Effect>		mEffect;				// The UberShader of the RenderComponent (default is an empty UberShader!)
			ShaderCache						mShaderCache;			// The cache used for the Shader
			std::shared_ptr<RenderStatesGroup> mRenderStatesGroup;	// The group of entities sharing the same RenderStates the RenderComponent belongs to (set and maintained by the RenderSystem!)
	};
};