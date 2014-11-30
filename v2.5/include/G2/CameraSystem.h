// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include <G2Core/BaseSystem.h>

namespace G2 
{
	class CameraComponent;
	/** This class defines the CameraSystem for the basic movement handling.
	 * @created:	2014/02/02
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class CameraSystem : public BaseSystem<CameraSystem,CameraComponent> 
	{
		public:

			CameraSystem() : mRenderCameraEntityId(0) {}

			void runPhase(std::string const& name, FrameInfo const& frameInfo);
			/** This function will set the active CameraComponent to use 
			 * for rendering the main scene.
			 * @param entityId The ID of the Entity class instance, where the 
			 * CameraComponent is attached to.
			 * @note You have to set at least once a render camera in each application.
			 */
			void setRenderCamera(unsigned int entityId);
			/** Returns the currently used main render camera.
			 * @return The currently used main render camera or nullptr, if no render camera is defined.
			 */
			CameraComponent* getRenderCamera(); 

		private:
			unsigned int	mRenderCameraEntityId; // The entity id of the component containing the camera to use for rendering
	};
};