#pragma once
#include "EditorGrid.h"

#include <G2Core/Entity.h>
#include <G2/EffectImporter.h>
#include <G2/FBXImporter.h>
#include <G2/RenderComponent.h>
#include <G2/CameraComponent.h>
#include <G2/TransformComponent.h>
#include <G2Core/FrameInfo.h>

#include <list>

class ManagedEntity;
/** This class is responsible for the editorial content the user may interact with.
 * @created:	2014/01/05
 * @author Andy Reimann <a.reimann@moorlands-grove.de>
 */
class EditorGeometryManager 
{
	public:
		/// This constructs a new EditorGeometryManager.
		EditorGeometryManager();
		/** Returns the entity id of the translation handle mesh for the X-Axis.
		* @return The entity id of the translation handle mesh for the X-Axis.
		*/
		unsigned int getXTranslationHandleId() const { return mTransXMesh.getId(); };
		/** Returns the entity id of the translation handle mesh for the Y-Axis.
		* @return The entity id of the translation handle mesh for the Y-Axis.
		*/
		unsigned int getYTranslationHandleId() const { return mTransYMesh.getId(); };
		/** Returns the entity id of the translation handle mesh for the Z-Axis.
		* @return The entity id of the translation handle mesh for the Z-Axis.
		*/
		unsigned int getZTranslationHandleId() const { return mTransZMesh.getId(); };
		/** Returns the entity id of the scale handle mesh for the X-Axis.
		* @return The entity id of the scale handle mesh for the X-Axis.
		*/
		unsigned int getXScaleHandleId() const { return mScaleXMesh.getId(); };
		/** Returns the entity id of the scale handle mesh for the Y-Axis.
		* @return The entity id of the scale handle mesh for the Y-Axis.
		*/
		unsigned int getYScaleHandleId() const { return mScaleYMesh.getId(); };
		/** Returns the entity id of the scale handle mesh for the Z-Axis.
		* @return The entity id of the scale handle mesh for the Z-Axis.
		*/
		unsigned int getZScaleHandleId() const { return mScaleZMesh.getId(); };
		/** Returns the entity id of the rotation handle mesh for the X-Axis.
		* @return The entity id of the rotation handle mesh for the X-Axis.
		*/
		unsigned int getXRotationHandleId() const { return mRotateXMesh.getId(); };
		/** Returns the entity id of the rotation handle mesh for the Y-Axis.
		* @return The entity id of the rotation handle mesh for the Y-Axis.
		*/
		unsigned int getYRotationHandleId() const { return mRotateYMesh.getId(); };
		/** Returns the entity id of the rotation handle mesh for the Z-Axis.
		* @return The entity id of the rotation handle mesh for the Z-Axis.
		*/
		unsigned int getZRotationHandleId() const { return mRotateZMesh.getId(); };
		/** Returns the entity id of the transformation handle mesh.
		* @return The entity id of the transformation handle mesh.
		*/
		unsigned int getTransformationHandleId() const { return mTransAnchor.getId(); };
		/** Returns the solid effect file of the editor.
		 * @note This function is supposed to be used as a quick debug functionality.
		 */
		std::shared_ptr<G2::Effect> _getSolidFx() const { return mSolidFx; }

		~EditorGeometryManager();

		/** Adds a global geometry with the given data.
		 * @param name The name to register the global geometry.
		 * @param vertices the vertex data.
		 * @param num The number of vertices contained in the vertices array.
		 * @param ambient The ambient color to use.
		 * @note The geometry will always use the solid effect of the editor.
		 */
		static void addGlobalGeometry(std::string const& name, glm::vec3* vertices, unsigned int num, glm::vec4 ambient);
		/** Clears all global geometry registered under the given name.
		 * @param name The name of the geomety to clear.
		 */
		static void clearGlobalGeometry(std::string const& name);

		static G2Core::RenderLayer::Name gHandleLayer;
		static G2Core::RenderLayer::RenderLayerMask gEditorGeometryLayers;
	private:
		/** Called every frame by the GEAR engine.
		 */
		void _updateEditorGrid(G2::FrameInfo const& frame);
		/** This is a callback function which will be invoked whenever a new ManagedEntity is selected/gains focus.
		* @param entity A pointer to the ManagedEntity which is selected.
		*/
		void _onManagedEntitySelected(ManagedEntity* entity);

		void _updateGridPosition();

		void _initTranslationHandles();
		void _initScaleHandles();
		void _initRotationHandles();
		void _initHandleSettings(G2::RenderComponent* rc, G2::TransformComponent* tc, glm::vec4 const& ambientColor, float degrees, glm::vec3 const& axis);

		G2::RenderSystem*	mRenderSystem;
		G2::CameraSystem*	mCameraSystem;
		G2::TransformSystem* mTransformSystem;

		G2::EffectImporter  mEffectImporter;			// The effect importer used for the editor geometry
		G2::FBXImporter mFbxImporter;					// The effect importer used for the editor geometry
		std::shared_ptr<G2::Effect>	mSolidFx;			// The effect for solid ambient lighting
		
		std::list<EditorGrid>	mGrids;					// The different grids sorted by their unit size
		EditorGrid*				mActiveGrid;			// The currently active grid
		
		G2::Entity mSelectedEntityAABB;					// The AABB visualization of the currently selected entity

		G2::Entity mTransAnchor;
		
		G2::Entity mTransZMesh;							// The translation mesh for the Z axis
		G2::Entity mTransYMesh;							// The translation mesh for the Y axis
		G2::Entity mTransXMesh;							// The translation mesh for the X axis

		G2::Entity mScaleZMesh;							// The scale mesh for the Z axis
		G2::Entity mScaleYMesh;							// The scale mesh for the Y axis
		G2::Entity mScaleXMesh;							// The scale mesh for the X axis

		G2::Entity mRotateZMesh;						// The rotation mesh for the Z axis
		G2::Entity mRotateYMesh;						// The rotation mesh for the Y axis
		G2::Entity mRotateXMesh;						// The rotation mesh for the X axis

		static std::map<std::string,std::list<G2::Entity>> gGeometry; // An anonymous geometry container containing any kind of geometry with fast access possibilities
};