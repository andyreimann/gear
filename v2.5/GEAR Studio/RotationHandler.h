#pragma once
#include "EditorGrid.h"
#include "HandlerBase.h"

#include <G2Core/Entity.h>
#include <G2/EffectImporter.h>
#include <G2/FBXImporter.h>
#include <G2/RenderComponent.h>
#include <G2/CameraComponent.h>
#include <G2/TransformComponent.h>
#include <G2Core/FrameInfo.h>

#include <list>

class ManagedEntity;
/** This class is responsible for handling the rotation of meshes through the handlers of the Editor
 * @created:	2014/01/13
 * @author Andy Reimann <a.reimann@moorlands-grove.de>
 */
class RotationHandler : private G2S::HandlerBase
{
	public:
		/** This constructs a new TranslationHandler.
		 * @param xAxisHandlerId The Entity id of the handler for the X-Axis.
		 * @param yAxisHandlerId The Entity id of the handler for the Y-Axis.
		 * @param zAxisHandlerId The Entity id of the handler for the Z-Axis.
		 */
		RotationHandler(unsigned int xAxisHandlerId, unsigned int yAxisHandlerId, unsigned int zAxisHandlerId);

		~RotationHandler();
	private:

		enum State
		{
			ROTATION_NOT,
			ROTATION_X_AXIS,
			ROTATION_Y_AXIS,
			ROTATION_Z_AXIS,
		};
		/** Automatically called whenever this RotationHandler's handle becomes active/inactive.
		*/
		void handleActivityChanged() override;

		/** Called whenever an intersection occurred with an object assigned to the RenderLayer defined
		 * in EditorGeometryManager::gHandleLayer.
		*/
		void _onEditorHandleSelected(unsigned int entityId, G2::Intersection const& intersection);
		/** This is a callback function which will be invoked whenever a new ManagedEntity is selected/gains focus.
		* @param entity A pointer to the ManagedEntity which is selected.
		*/
		void _onManagedEntitySelected(ManagedEntity* entity);
		void _onMouseUp(G2::MouseButton button, glm::detail::tvec2<int> const& pos);
		void _onMouseMove(glm::detail::tvec2<int> const& pos);

		/** Calculates the intersection point with the current translation plane and the given Ray.
		 * @param ray The ray to intersect with the translation plane.
		 * @param result A pointer to a bool. If true, an intersection was detected, false otherwise.
		 * @return The intersection point.
		 */
		glm::vec3 _getPlaneIntersection(G2::Ray const& ray, bool* result) const;

		G2::CameraSystem*	mCameraSystem;
		G2::TransformSystem* mTransformSystem;
		G2::RenderSystem* mRenderSystem;

		ManagedEntity*	mEntity;				// The ManagedEntity which is currently selected

		glm::mat4		mEntityStartWSMatrix;
		glm::vec3		mHandlerStartPoint;		// The intersection point with the last selected handler
		glm::vec3		mEntityStartPosition;	// The position of the selected ManagedEntity at the time were a handler was selected.
		glm::quat		mEntityStartRotation;	// The rotation of the selected ManagedEntity at the time were a handler was selected.
		glm::vec3		mPickOffset;			// The offset between the intersection on the handle and the origin of the selected mesh
		glm::vec3		mRotationPlaneNormal;	// The normal vector of the current rotation plane
		float 			mRotationPlaneOffset;	// The offset to 0 of the current rotation plane
		State			mState;					// The current state of the RotationHandler
		unsigned int	mZAxisHandlerId;		// The Entity id of the handler for the X-Axis.
		unsigned int	mYAxisHandlerId;		// The Entity id of the handler for the Y-Axis.
		unsigned int	mXAxisHandlerId;		// The Entity id of the handler for the Z-Axis.
};