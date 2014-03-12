// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "CameraSystem.h"
#include "CameraComponent.h"

#include <iostream>

using namespace G2;

void
CameraSystem::runPhase(std::string const& name, FrameInfo const& frameInfo) 
{
	// nothing
	if(name == "postUpdate") 
	{
		// we only support first camera up to now
		/*
		
		if(camera->updateModelView()) {
			// transformations have changed this frame -> upload matrices to default shader
			
			defaultShader->setUniformMatrix("matrices.modelViewMatrix", camera->getModelviewMatrix());
			defaultShader->setUniformMatrix("matrices.normalMatrix", camera->getNormalMatrix());
		}
		*/
		//for(auto i = 0; i < components.size(); ++i) {
			// do sth.
		//}
	}
}

CameraComponent*
CameraSystem::getRenderCamera() 
{
	return get(mRenderCameraEntityId);
}

void
CameraSystem::setRenderCamera(unsigned int entityId) 
{
	mRenderCameraEntityId = entityId;
}