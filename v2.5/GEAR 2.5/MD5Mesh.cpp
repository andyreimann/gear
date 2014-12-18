// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "MD5Mesh.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "MD5AnimationComponent.h"
#include "NameComponent.h"

using namespace G2;

void 
MD5Mesh::Builder::createMeshData(G2::Entity* entity, std::vector<Builder::SubMesh> const& meshes, bool importNormals, bool importTexCoords)
{
	auto* renderComponent = entity->addComponent<RenderComponent>();
	renderComponent->allocateVertexArrays((unsigned int)meshes.size());
	renderComponent->allocateIndexArrays((unsigned int)meshes.size());

	for (int i = 0; i < meshes.size() ; ++i) 
	{
		Builder::SubMesh const& mesh = meshes[i];
		VertexArrayObject& vao = renderComponent->getVertexArray((unsigned int)i);
		vao.resizeElementCount((unsigned int)mesh.vertices.size());
		vao.writeData( G2Core::Semantics::POSITION, &mesh.vertices[0]);
		if(importNormals)
		{
			vao.writeData( G2Core::Semantics::NORMAL, &mesh.normals[0]);
		}
		if(importTexCoords)
		{
			vao.writeData( G2Core::Semantics::TEXCOORD_0, &mesh.uvs[0]);
		}
		IndexArrayObject& iao = renderComponent->getIndexArray((unsigned int)i);
		iao.writeIndices( &mesh.indices[0], (unsigned int)mesh.indices.size());
		renderComponent->addDrawCall(DrawCall()
			.setDrawMode(G2Core::DrawMode::TRIANGLES)
			.setEnabled(true)
			.setVaoIndex(i)
			.setIaoIndex(i)
			.setAABBCalculationMode(AABBCalculationMode::ANIMATED));
	}
}

G2::Entity
MD5Mesh::Builder::buildResource(bool importNormals, bool importTexCoords, bool importAnimations, G2::Entity* target)
{
	// create mesh
	G2::Entity meshLocal;
	G2::Entity* mesh = &meshLocal;
	if (target != nullptr)
	{
		mesh = target; // in case the user gave a valid pointer, we don't write into the local mesh
	}
	// create VAO from sub meshes
	createMeshData(mesh, subMeshes, importNormals, importTexCoords);

	// attach an animation component and configure it
	if(animationData.skeletonFrames.frames.size() > 0 && importAnimations) 
	{
		auto* animationComponent = mesh->addComponent<MD5AnimationComponent>(animationData.numJoints);
		animationComponent->animationData = animationData;

		// transfer vertex weights and weights to animation data
		animationComponent->animationData.meshAnimationData.resize(subMeshes.size());
		for (int i = 0; i < subMeshes.size() ; ++i) 
		{
			animationComponent->animationData.meshAnimationData[i].weights = subMeshes[i].weights;
			animationComponent->animationData.meshAnimationData[i].vertexWeights = subMeshes[i].vertexWeights;
		}
	
		animationComponent->animationState.startFrame = 0;
		animationComponent->animationState.nextFrame = 1;
		animationComponent->animationState.currentFrame = 0;
		animationComponent->animationState.lastTime = 0.0;
		animationComponent->animationState.invFrameRate = 1.0 / 24.0f;//animationData.frameRate;
		animationComponent->animationState.maxFrame = animationData.numFrames - 1;
		animationComponent->animationState.animMode = 1;
	}
	
	auto* nameComponent = mesh->addComponent<NameComponent>();
	nameComponent->name = name;
	return std::move(meshLocal);
}