// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "BaseSystem.h"

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
			 * for rendering.
			 * @param entityId The ID of the Entity class instance, where the 
			 * CameraComponent is attached to.
			 */
			void setRenderCamera(unsigned int entityId);

			CameraComponent* getRenderCamera(); 

		private:
			unsigned int	mRenderCameraEntityId; // The entity id of the component containing the camera to use for rendering
	};
};