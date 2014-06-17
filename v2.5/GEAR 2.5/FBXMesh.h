// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "ResourceBuilder.h"

// FBX Helper Classes
#include "FBXMaterialCache.h"
#include "FBXVBOMesh.h"
#include "FBXLightCache.h"

#include <G2Core/Entity.h>

#include <fbxsdk.h>
#include <glm/glm.hpp>

namespace G2 
{
	/** This class holds all the data needed to visualize a FBXMesh.
	 * Since it is a derivation of the Entity, it takes advantage of the
	 * features of the ECS.
	 * Instances are usually created using the FBXImporter class.
	 * @created:	2014/02/18
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class FBXMesh : public Entity
	{
		public:

			struct Builder : public ResourceBuilder<Builder,FBXMesh>
			{
				struct MeshMetaData
				{
					MeshMetaData(FbxMesh const*	mesh, unsigned int vaoOffset);

					struct SubMeshMetaData
					{
						SubMeshMetaData() 
							: indexOffset(0), 
							triangleCount(0) 
						{}

						int indexOffset;
						int triangleCount;
					};

					unsigned int			vaoOffset;		// ENGINE RELATED index into the VAO array of the resulting RenderComponent
					FbxMesh const*			mesh;			// The mesh, the Meta Data belongs to
					bool					allByControlPoint; // Vertices are controlled by the control points or not
					bool					hasNormals;		// Indicates whether the Mesh has normals or not
					bool					hasUvs;			// Indicates whether the Mesh has texture coordinates or not
					std::vector<glm::vec3>  vertices;		// The initial vertices
					std::vector<glm::vec3>  normals;		// The initial normals
					std::vector<glm::vec2>  uvs;			// The initial texture coordinates
					std::vector<unsigned int> indices;		// The indices

					std::vector<SubMeshMetaData> subMeshMetaData; // Meta data for how to render the resulting index buffer (is rendered in multiple draw calls)
				};
				
				void deleteScene();
				/** Unload the cache and release the memory under this node recursively.
				 */
				void unloadCacheRecursive(FbxNode * pNode);
				~Builder();

				std::shared_ptr<FBXMesh> buildResource();

				std::string				name;
				FbxScene*				fbxScene; // This object holds most objects imported from files.
				FbxImporter *			fbxImporter;
				bool					isAnimated;

				FbxArray<FbxString*>	animStackNameArray;
				FbxArray<FbxNode*>		cameraArray;
				FbxTime					cacheStart, cacheStop;
				FbxTime					start, stop; // the initial start and stop times
				FbxArray<FbxPose*>		poseArray;

				// things needed for animation and rendering
				int						poseIndex;	// The index of the currently used pose
				FbxAnimLayer *			initialAnimLayer;
				FbxAnimStack *			initialAnimationStack;
				//FbxTime				currentTime;

				std::vector<MeshMetaData> meshMetaData; // The Meta Data for the Meshes, the FBXMesh holds
			};

			/** This constructs a new FBXMesh.
			 */
			FBXMesh();

			FBXMesh(FBXMesh && rhs);

			FBXMesh& operator=(FBXMesh && rhs);


		protected:
		private:
	};
};