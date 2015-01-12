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
/** This class is responsible for handling the general transformation of meshes through the handlers of the Editor
 * @created:	2014/01/12
 * @author Andy Reimann <a.reimann@moorlands-grove.de>
 */
class TransformationHandler 
{
	public:
		/** This constructs a new TransformationHandler.
		 * @param transformationAnchorId The Entity id of the transformation anchor every specific transformator is connected to.
		 */
		TransformationHandler(unsigned int transformationAnchorId);

		~TransformationHandler();
	private:

		/** This is a callback function which will be invoked whenever a new ManagedEntity is selected/gains focus.
		* @param entity A pointer to the ManagedEntity which is selected.
		*/
		void _onManagedEntitySelected(ManagedEntity* entity);
		/** Called every frame by the GEAR engine.
		*/
		void _onRenderFrame(G2::FrameInfo const& frame);

		void _update();

		G2::TransformSystem* mTransformSystem;

		ManagedEntity*	mEntity;				// The ManagedEntity which is currently selected
		
		unsigned int	mTransformationAnchorId;		// The Entity id of the transformation anchor every specific transformator is connected to.
};