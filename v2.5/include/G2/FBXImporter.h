// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Importer.h"
#include "FBXMesh.h"
#include "RefCounter.h"

namespace G2 
{

	class TextureImporter;
	/** This class implements the generic resource importer interface
	 * to provide an importer for FBXMesh objects from files.
	 * @created:	2014/02/18
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class FBXImporter : public Importer<FBXImporter, G2::Entity, FBXMesh::Builder>
	{
		public:

			FBXImporter();
			
			/** This function will load and import the given FBX file
			 * @note Don't use this function directly, use import() instead!
			 * @param meshFileName The filename of the FBXMesh file to import
			 * @param importNormals Flag indicating whether you want the resulting mesh to import normals if available.
			 * @param importNormals Flag indicating whether you want the resulting mesh to import texture coordinates if available.
			 * @param importNormals Flag indicating whether you want the resulting mesh to import animations if available.
			 * @param flipTexU Flips the U-Coordinate of the UV coordinates.
			 * @param flipTexV Flips the V-Coordinate of the UV coordinates.
			 * @param texImporter A pointer to a texture importer to use for loading textures together with the mesh if available or nullptr otherwise.
			 * @param target An optional pointer to an already existing G2::Entity to attach the mesh instead of at the returned one.
			 * @return A G2::Entity were the mesh is attached to in case you did not supply a valid pointer in the target parameter.
			 * @note Requesting one mesh multiple times will result in cache hits.
			 */
			G2::Entity importResource(std::string const& fileName, bool importNormals = true, bool importTexCoords = true, bool importAnimations = true, bool flipTexU = false, bool flipTexV = false, TextureImporter* texImporter = nullptr, G2::Entity* target = nullptr);
			std::pair<std::string, std::shared_ptr<FBXMesh::Builder>> produceResourceBuilder(std::string const& fileName, bool importNormals = true, bool importTexCoords = true, bool importAnimations = true, bool flipTexU = false, bool flipTexV = false, TextureImporter* texImporter = nullptr, G2::Entity* target = nullptr);
			
			~FBXImporter();

		protected:
		private:
			
			void _fillCameraArray(std::shared_ptr<FBXMesh::Builder> builder);
			void _fillCameraArrayRecursive(FbxNode* pNode, FbxArray<FbxNode*>& pCameraArray);
			
			void _loadCacheRecursive(FbxScene* pScene, FbxAnimLayer* pAnimLayer, const char * pFbxFileName, bool pSupportVBO, unsigned int nextVaoOffset, std::shared_ptr<FBXMesh::Builder> builder);
			void _loadCacheRecursive(FbxNode * pNode, FbxAnimLayer * pAnimLayer, bool pSupportVBO, unsigned int& nextVaoOffset, std::shared_ptr<FBXMesh::Builder> builder);

			void _preparePointCacheData(FbxScene* pScene, FbxTime &pCache_Start, FbxTime &pCache_Stop);
			void _fillPoseArray(FbxScene* pScene, FbxArray<FbxPose*>& pPoseArray);


			static FbxManager*	gSdkManager;
			static RefCounter<int,0>	gRefCounter;
			static void			initSdk();
	};
};