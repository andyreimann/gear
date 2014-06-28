// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "MD5Mesh.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "MD5AnimationComponent.h"
#include "NameComponent.h"

using namespace G2;

MD5Mesh::MD5Mesh() 
{
}

MD5Mesh::MD5Mesh(MD5Mesh && rhs) 
{
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

MD5Mesh&
MD5Mesh::operator=(MD5Mesh && rhs) 
{
	return static_cast<MD5Mesh&>(G2::Entity::operator=(std::move(rhs)));
}

void 
MD5Mesh::createVAO(std::vector<Builder::SubMesh> const& meshes)
{
	auto* renderComponent = addComponent<RenderComponent>();
	renderComponent->allocateVertexArrays((unsigned int)meshes.size());
	renderComponent->allocateIndexArrays((unsigned int)meshes.size());

	for (int i = 0; i < meshes.size() ; ++i) 
	{
		Builder::SubMesh const& mesh = meshes[i];
		VertexArrayObject& vao = renderComponent->getVertexArray((unsigned int)i);
		vao.resizeElementCount((unsigned int)mesh.vertices.size());
		vao.writeData( Semantics::POSITION, (glm::vec3*)&mesh.vertices[0]);
		vao.writeData( Semantics::NORMAL, (glm::vec3*)&mesh.normals[0]);
		vao.writeData( Semantics::TEXCOORD_0, (glm::vec2*)&mesh.uvs[0]);
		IndexArrayObject& iao = renderComponent->getIndexArray((unsigned int)i);
		iao.writeIndices( &mesh.indices[0], (unsigned int)mesh.indices.size());
		renderComponent->addDrawCall(DrawCall()
			.setDrawMode(GL_TRIANGLES)
			.setEnabled(true)
			.setVaoIndex(i)
			.setIaoIndex(i)
			.setAABBCalculationMode(AABBCalculationMode::ANIMATED));
	}
}

std::shared_ptr<MD5Mesh>
MD5Mesh::Builder::buildResource() 
{
	// create new MD5Mesh
	std::shared_ptr<MD5Mesh> mesh = std::shared_ptr<MD5Mesh>(new MD5Mesh());

	// create VAO from sub meshes
	mesh->createVAO(subMeshes);

	// attach an animation component and configure it
	if(animationData.skeletonFrames.frames.size() > 0) 
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
	return mesh;
}