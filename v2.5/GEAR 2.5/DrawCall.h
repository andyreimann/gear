#pragma once
#include "AABB.h"

#include <G2Core/Defines.h>

namespace G2 
{

	enum AABBCalculationMode
	{
		AUTOMATIC,	// default
		ANIMATED,	
		MANUAL,
	};
	/** This class defines all informations needed to invoke a draw call for rendering VertexArrayObjects and IndexArrayObjects.
	 * @created:	2014/06/28
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class DrawCall 
	{
		friend class RenderComponent;
		friend class RenderSystem;
		friend class TransformSystem;
		public:
			/** Constructs a new DrawCall object.
			 * The AABBCalculationMode defaults to AABBCalculationMode::AUTOMATIC.
			 * The DrawCall is enabled by default.
			 * The draw mode is set to TRIANGLES by default.
			 */
			DrawCall();
			/// Copy constructor.
			DrawCall(DrawCall const& rhs);
			/// Move constructor.
			DrawCall(DrawCall && rhs);
			/// Basic assignment operator.
			DrawCall& operator=(DrawCall const& rhs);
			/// Basic move operator.
			DrawCall& operator=(DrawCall && rhs);
			/** Sets the AABB calculation mode for the DrawCall.
			 * @param mode The mode to use..
			 * @note The default value is AABBCalculationMode::AUTOMATIC.
			 * @note AABBCalculationMode::AUTOMATIC means the engine will calculate the model space AABB before the RenderComponent is drawn the first time.
			 * @note AABBCalculationMode::ANIMATED means the engine will calculate the model space AABB before the RenderComponent is drawn the first time and every time an animation is performed for the RenderComponent.
			 * @note AABBCalculationMode::MANUAL means the engine will never calculate the model space AABB but will use the given one.
			 */
			DrawCall& setAABBCalculationMode(AABBCalculationMode mode);
			/** Sets the index of the VertexArrayObject inside of the array of VertexArrayObjects 
			 * of a RenderComponent to reference.
			 * @param index The index of the VertexArrayObject to reference.
			 * @note It will not be checked if the index is available. Make sure that the referenced VertexArrayObject exists when drawing the RenderComponent.
			 */
			DrawCall& setVaoIndex(unsigned int index) { mVaoIndex = (int)index; return *this; }
			/** Sets the index of the IndexArrayObject inside of the array of IndexArrayObject 
			 * of a RenderComponent to reference.
			 * @param index The index of the IndexArrayObject to reference. If set to -1, no IndexArrayObject is referenced by this DrawCall.
			 * @note It will not be checked if the index is available. Make sure that the referenced IndexArrayObject exists when drawing the RenderComponent.
			 */
			DrawCall& setIaoIndex(unsigned int index) { mIaoIndex = (int)index; return *this; }
			/** This function will force the engine to recalculate the model space AABB before the RenderComponent 
			 * is drawn the next time. The recalculation is done by using all vertex positions of the referenced VertexArrayObject.
			 * @note The recalculation is only done, if the configured AABBCalculationMode is set to a value distinct from AABBCalculationMode::MANUAL.
			 */
			DrawCall& forceAABBRecalculation() { mLastAutoRecalcFrame = -1; return *this; }
			/** Updates the draw mode to use when invoking this DrawCall.
			 * @param drawMode The new draw mode to use.
			 */
			DrawCall& setDrawMode(G2Core::DrawMode::Name drawMode) { mDrawMode = drawMode; return *this; }
			/** Enables this DrawCall.
			 */
			DrawCall& enable();
			/** Disables this DrawCall.
			 */
			DrawCall& disable();
			/** Enables or disables this DrawCall.
			 * @param enabled True if you want to enable this DrawCall, false otherwise.
			 */
			DrawCall& setEnabled(bool enabled);
			/** This function will set the model space AABB to the given one.
			* @param value The model space AABB.
			 * @note This model space AABB may be overwritten, if the configured AABBCalculationMode is set to a value distinct from AABBCalculationMode::MANUAL.
			*/
			DrawCall&  setModelSpaceAABB(G2::AABB const& value) { mModelSpaceAABB = value; return *this; }
			/** Returns the currently set model space AABB of this draw call.
			 * @return The currently set model space AABB of this draw call.
			 * @note This AABB may change depending on the configured AABBCalculationMode.
			 */
			AABB const& getModelSpaceAABB() const { return mModelSpaceAABB; }
			/** Returns the currently calculated world space AABB of this draw call.
			 * @return The currently calculated world space AABB of this draw call.
			 * @note This AABB may change depending on the TransformComponent of the Entity this DrawCall is attached to.
			 */
			AABB const& getWorldSpaceAABB() const { return mWorldSpaceAABB; }
			/** Returns the currently configured AABBCalculationMode.
			 * @return The currently configured AABBCalculationMode.
			 */
			AABBCalculationMode getAABBCalculationMode() const { return mAABBCalculationMode; }
			/** Returns the current state of the DrawCall.
			 * @return True if this DrawCall is enabled, false if not.
			 */
			bool isEnabled() const { return mEnabled; }
			/** Returns the referenced VertexArrayObject index.
			 * @return The referenced VertexArrayObject index.
			 */
			int getVaoIndex() const { return mVaoIndex; }
			/** Returns the referenced IndexArrayObject index.
			 * @return The referenced IndexArrayObject index or -1 if not IndexArrayObject is referenced by this DrawCall
			 */
			int getIaoIndex() const { return mIaoIndex; }
			/** Returns the ID of the Entity this DrawCall is attached to.
			 * @return The ID of the Entity this DrawCall is attached to or Entity::UNINITIALIZED_ENTITY_ID if the DrawCall is not attached to any Entity.
			 */
			unsigned int getEntityId() const { return mEntityId; }
			/** Returns the draw mode to use when invoking this DrawCall.
			 * @return The draw mode to use when invoking this DrawCall.
			 */
			G2Core::DrawMode::Name getDrawMode() const { return mDrawMode; }
			/** Returns whether the DrawCall was frustum culled from the last frustum culling check.
			 * @return True if the DrawCall was frustum culled from the last frustum culling check, false if not.
			 */
			bool isFrustumCulled() const { return mWasFrustumCulled; }
		private:
			/** Updates the index of this DrawCall in the list of DrawCalls this DrawCall belongs to.
			 * @param drawCallIndex The new index of this DrawCall. 
			 */
			void _setDrawCallIndex(int drawCallIndex) { mDrawCallIndex = drawCallIndex; }
			/** Updates the Entity-ID this DrawCall is attached to.
			 * @param entityId The Entity-ID this DrawCall is attached to.
			 */
			void _setEntityId(unsigned int entityId) { mEntityId = entityId; }
			
			AABBCalculationMode mAABBCalculationMode;   // The mode to use for calculating and updating the model space AABB
			AABB                mModelSpaceAABB;        // The model space AABB
			AABB                mWorldSpaceAABB;        // The current world space AABB
			long                mLastAutoRecalcFrame;   // The last frame, when this DrawCalls model space AABB was calculated from the engine.
			bool                mEnabled;               // simple flag to entirely enable/disable a draw call
			int					mDrawCallIndex;         // The index of the draw call
			int                 mVaoIndex;              // The index of the VertexArrayObject, this draw call references
			int                 mIaoIndex;				// The index of the IndexArrayObject, this draw call references or -1, if it does not reference any IndexBufferObject
			unsigned int        mEntityId;              // The Entity-ID the DrawCall is attached to - filled in automatically when adding a draw call to a RenderComponent
			G2Core::DrawMode::Name	mDrawMode;				// The draw mode to use when rendering
			bool				mWasFrustumCulled;		// Set from the engine while rendering and indicating whether this DrawCall was culled by the frustum in the last frame, this DrawCall was rendered.
	};
};