// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "MD5AnimationSystem.h"
#include "MD5AnimationComponent.h"
#include "RenderComponent.h"
//#define MD5_ASYNC_ANIMATION

#ifdef MD5_ASYNC_ANIMATION
	#include <future>
#endif

using namespace G2;


void
MD5AnimationSystem::runPhase(std::string const& name, FrameInfo const& frameInfo)
{
	if(name == "postUpdate") 
	{
		// update all animations
		
		for (int i = 0; i < components.size() ; ++i) 
		{
			MD5AnimationComponent& comp = components[i];

			if(!comp.animationState.animated)
			{
				continue;
			}

			if(mRenderSystem == nullptr) 
			{
				mRenderSystem = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>();
			}
			// check 
			auto* renderComponent = mRenderSystem->get(comp.getEntityId());
			if(renderComponent == nullptr)
			{
				continue; // no RenderComponent attached for some strange reason!
			}

			if(animate(&comp, frameInfo))
			{
				// mesh needs a pose update
				updatePose(&comp, renderComponent);
			}
		}
	}
}

bool
MD5AnimationSystem::animate(MD5AnimationComponent* component, FrameInfo const& frameInfo) 
{
	component->animationState.lastTime += frameInfo.timeSinceLastFrame;

	if (component->animationState.lastTime >= component->animationState.invFrameRate)
	{
		int numFrames = (int)(component->animationState.lastTime / component->animationState.invFrameRate);
		component->animationState.currentFrame+= numFrames;
		component->animationState.nextFrame = component->animationState.currentFrame + 1;
		component->animationState.lastTime -= numFrames * component->animationState.invFrameRate;

		if (component->animationState.currentFrame > component->animationState.maxFrame) 
		{
			component->animationState.currentFrame = component->animationState.startFrame; // start from front
		}
		if (component->animationState.nextFrame > component->animationState.maxFrame )
		{
			component->animationState.nextFrame = component->animationState.startFrame;
		}
		interpolateFrames(component, (float)component->animationState.lastTime);
		return true;
	}
	return false;
}

void
MD5AnimationSystem::interpolateFrames(MD5AnimationComponent* component, float interpolationFactor) 
{
	SkeletonFrame& skeletonFrameA = component->animationData.skeletonFrames.frames[component->animationState.currentFrame];
	SkeletonFrame& skeletonFrameB = component->animationData.skeletonFrames.frames[component->animationState.nextFrame];

	for(int i = 0; i < component->animationData.numJoints; ++i) {
		Joint& target = component->interpolatedSkeleton.joints[i];
		/* Copy parent index */
		Joint& jointA = skeletonFrameA.joints[i];
		Joint& jointB = skeletonFrameB.joints[i];
		jointB.parentIndex = jointA.parentIndex;

		/* Linear interpolation for position */
		target.position.x = jointA.position.x + interpolationFactor * (jointB.position.x - jointA.position.x);
		target.position.y = jointA.position.y + interpolationFactor * (jointB.position.y - jointA.position.y);
		target.position.z = jointA.position.z + interpolationFactor * (jointB.position.z - jointA.position.z);

		/* Spherical linear interpolation for orientation */
		target.orientation = glm::mix(jointA.orientation, jointB.orientation, (float)interpolationFactor);
	}
}

void
MD5AnimationSystem::updatePose(MD5AnimationComponent* animationComponent, RenderComponent* renderComponent) 
{
	
#ifdef MD5_ASYNC_ANIMATION
	// use shared_future because we copy the future object!
	std::vector<std::shared_future<void>> handles;
#endif


	size_t numMeshes = renderComponent->vaos.size();

	if(renderComponent->aabbAnimationRecalc && renderComponent->objectSpaceAABBs.size() != renderComponent->vaos.size())
	{
		renderComponent->objectSpaceAABBs.resize(renderComponent->vaos.size());
	}

	for (size_t c = 0; c < numMeshes; ++c) 
	{
		MeshAnimationData& meshAnimationData = animationComponent->animationData.meshAnimationData[c];
		unsigned int numVertices = renderComponent->vaos[c].getNumElements();
		if(renderComponent->aabbAnimationRecalc)
		{
			renderComponent->objectSpaceAABBs[c].clear();
		}

		if(meshAnimationData.vertexCache.size() == 0)
		{
			// setup cache
			meshAnimationData.vertexCache.resize(numVertices);
			meshAnimationData.normalCache.resize(numVertices);
		}

#ifdef MD5_ASYNC_ANIMATION
		auto handle = std::async(std::launch::async, [c, numVertices, &animationComponent, &renderComponent] {
			
			MeshAnimationData& meshAnimationData = animationComponent->animationData.meshAnimationData[c];
#endif
			/* Setup vertices */

			for(unsigned int i = 0; i < numVertices; ++i) 
			{


					// reset to zero before newly calculating a vertex position
					meshAnimationData.vertexCache[i] = glm::vec3(0.f,0.f,0.f);
					meshAnimationData.normalCache[i] = glm::vec3(0.f,0.f,0.f);

					/* Calculate final vertex to draw with weights */
					int numWeightIndices = meshAnimationData.vertexWeights[i].numWeightIndices;
					for (int j = 0; j < numWeightIndices; ++j) 
					{
						Weight const& weight = meshAnimationData.weights[meshAnimationData.vertexWeights[i].startWeightIndex + j];
						Joint const& joint = animationComponent->interpolatedSkeleton.joints[weight.jointIndex];

						/* Calculate transformed vertex for this weight */
						glm::vec3 wv, wn;
						wv = glm::rotate(joint.orientation, weight.position);
						//wv = joint.orientation.rotate(weight.position);
						wn = glm::rotate(joint.orientation, weight.normal);

						/* The sum of all weight->bias should be 1.0 */
						meshAnimationData.vertexCache[i] += (joint.position + wv) * weight.bias;
						meshAnimationData.normalCache[i] += wv * weight.bias;
						if(renderComponent->aabbAnimationRecalc)
						{
							renderComponent->objectSpaceAABBs[c].merge(meshAnimationData.vertexCache[i]);
						}
						//normal += wn * weight.bias;
					}
					//vertices[i] = vertex;
					//if(subMesh.normals.size() > 0) {
					//	subMesh.normals[i] = normal;
					//}
			}
#ifdef MD5_ASYNC_ANIMATION
		});
		handles.push_back(handle);
#else
		renderComponent->vaos[c].writeData(Semantics::POSITION, &meshAnimationData.vertexCache[0])
								.writeData(Semantics::NORMAL, &meshAnimationData.normalCache[0]);
		
#endif
	}
#ifdef MD5_ASYNC_ANIMATION
	for (int i = 0; i < handles.size(); ++i) 
	{
		// wait for every handle
		handles[i].get();

		// write cached data
		MeshAnimationData& meshAnimationData = animationComponent->animationData.meshAnimationData[i];
		
		renderComponent->vaos[i].writeData(Semantics::POSITION, &meshAnimationData.vertexCache[0])
								.writeData(Semantics::NORMAL, &meshAnimationData.normalCache[0]);
	}
#endif
}