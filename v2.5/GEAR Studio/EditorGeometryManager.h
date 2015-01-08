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

		~EditorGeometryManager();

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

		void _setTranslationMeshVisible(bool visible);

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
		G2::Entity mTransZMesh;
		G2::Entity mTransYMesh;
		G2::Entity mTransXMesh;							// The translation mesh for the X axis

		bool isTranslatingOnX;


};