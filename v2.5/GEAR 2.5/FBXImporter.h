// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Importer.h"
#include "FBXMesh.h"

namespace G2 
{
	/** This class implements the generic resource importer interface
	 * to provide an importer for FBXMesh objects from files.
	 * @created:	2014/02/18
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class FBXImporter : public Importer<FBXImporter,FBXMesh, FBXMesh::Builder>
	{
		public:
			
			/** This function will load and import the given FBX file
			 * @note Don't use this function directly, use import() instead!
			 * @param meshFileName The filename of the FBX-Mesh file to import
			 * @return a shared pointer pointing to the MD5Mesh or nullptr, if some error occurred.
			 * @note Requesting one mesh multiple times will result in cache hits.
			 */
			std::shared_ptr<FBXMesh> importResource(std::string const& meshFileName);

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