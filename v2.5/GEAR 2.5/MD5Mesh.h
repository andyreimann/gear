// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "ResourceBuilder.h"
#include "MD5AnimationComponent.h"

#include <G2Core/Entity.h>

#include <vector>

namespace G2 
{
	namespace MD5Mesh
	{
			struct Builder : public ResourceBuilder<Builder, G2::Entity>
			{
				G2::Entity buildResource(bool importNormals, bool importTexCoords, bool importAnimations, G2::Entity* target);

				struct SubMesh 
				{
					std::vector<glm::vec3>			vertices;
					std::vector<unsigned int>		indices;
					std::vector<glm::vec3>			normals;
					std::vector<glm::vec3>			uvs;
					std::vector<Weight>				weights;
					std::vector<VertexWeights>		vertexWeights;
				};

				struct JointInfo 
				{
					char name[64];
					int parent;
					int flags;
					int startIndex;
				};

				struct BaseFrameJoint
				{ // WHERE USED?
					glm::vec3 pos;
					glm::quat orient;
				};	
				
				std::string			 name;
				Skeleton			 baseSkeleton; // build mesh vertex coords with this skeleton to get the default pose!
				AnimationData		 animationData; // comes from anim file!
				std::vector<SubMesh> subMeshes;
				void createMeshData(G2::Entity* entity, std::vector<Builder::SubMesh> const& meshes, bool importNormals, bool importTexCoords);
			};
	};
};