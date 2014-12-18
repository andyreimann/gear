// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Importer.h"
#include "FBXMesh.h"

namespace G2 
{

	class TextureImporter;
	/** This class implements the generic resource importer interface
	 * to provide an importer for FBXMesh objects from files.
	 * @created:	2014/02/18
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class FBXImporter : public Importer<FBXImporter, FBXMesh, FBXMesh::Builder>
	{
		public:
			
			/** This function will load and import the given FBX file
			 * @note Don't use this function directly, use import() instead!
			 * @param meshFileName The filename of the FBXMesh file to import
			 * @param importNormals Flag indicating whether you want the resulting mesh to import normals if available.
			 * @param importNormals Flag indicating whether you want the resulting mesh to import texture coordinates if available.
			 * @param importNormals Flag indicating whether you want the resulting mesh to import animations if available.
			 * @param flipTexU Flips the U-Coordinate of the UV coordinates.
			 * @param flipTexV Flips the V-Coordinate of the UV coordinates.
			 * @param texImporter A pointer to a texture importer to use for loading textures together with the mesh if available or nullptr otherwise.
			 * @return a shared pointer pointing to the FBXMesh or nullptr, if some error occurred.
			 * @note Requesting one mesh multiple times will result in cache hits.
			 */
			std::shared_ptr<FBXMesh> importResource(std::string const& fileName, bool importNormals = true, bool importTexCoords = true, bool importAnimations = true, bool flipTexU = false, bool flipTexV = false, TextureImporter* texImporter = nullptr);
			std::pair<std::string, std::shared_ptr<FBXMesh::Builder>> produceResourceBuilder(std::string const& fileName, bool importNormals = true, bool importTexCoords = true, bool importAnimations = true, bool flipTexU = false, bool flipTexV = false, TextureImporter* texImporter = nullptr);

			/** This function will load and import the given FBX file
			* @note This function is for testing purpose only!
			* @param targetEntity The target Entity object to attach the RenderComponent to.
			* @param meshFileName The filename of the FBXMesh file to import
			* @param importNormals Flag indicating whether you want the resulting mesh to import normals if available.
			* @param importNormals Flag indicating whether you want the resulting mesh to import texture coordinates if available.
			* @param importNormals Flag indicating whether you want the resulting mesh to import animations if available.
			* @param flipTexU Flips the U-Coordinate of the UV coordinates.
			* @param flipTexV Flips the V-Coordinate of the UV coordinates.
			* @param texImporter A pointer to a texture importer to use for loading textures together with the mesh if available or nullptr otherwise.
			* @note Requesting one mesh multiple times will result in cache hits.
			*/
			G2::Entity* _test_importResource(G2::Entity* targetEntity, std::string const& fileName, bool importNormals = true, bool importTexCoords = true, bool importAnimations = true, bool flipTexU = false, bool flipTexV = false, TextureImporter* texImporter = nullptr);

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
			static void			initSdk();
	};
};