// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "ResourceBuilder.h"
#include "Entity.h"
#include "MD5AnimationComponent.h"

#include <vector>

namespace G2 
{
	/** This class holds all the data needed to visualize a MD5Mesh.
	 * Since it is a derivation of the Entity, it takes advantage of the
	 * features of the ECS.
	 * Instances are usually created using the MD5Importer class.
	 * @created:	2014/02/18
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class MD5Mesh : public Entity
	{
		public:
			struct Builder : public ResourceBuilder<Builder,MD5Mesh>
			{
				std::shared_ptr<MD5Mesh> buildResource();

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
				
				Skeleton			 baseSkeleton; // build mesh vertex coords with this skeleton to get the default pose!
				AnimationData		 animationData; // comes from anim file!
				std::vector<SubMesh> subMeshes;
			};
			/** This constructs a new MD5Mesh.
			 */
			MD5Mesh();
			
			/** Move constructor.
			 */
			MD5Mesh(MD5Mesh && rhs);
			/** Move assignment operator.
			 */
			MD5Mesh& operator=(MD5Mesh && rhs);

		private:

			void createVAO(std::vector<Builder::SubMesh> const& meshes);
	};
};