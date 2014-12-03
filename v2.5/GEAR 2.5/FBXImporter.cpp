// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "FBXImporter.h"
#include "Logger.h"
#include "FBXAnimationComponent.h"
#include "TextureImporter.h"

using namespace G2;

std::shared_ptr<FBXMesh>
FBXImporter::importResource(std::string const& fileName, bool importNormals, bool importTexCoords, bool importAnimations, bool flipTexU, bool flipTexV, TextureImporter* texImporter)
{
	
	auto it = mCache.find(fileName);
	if(it != mCache.end())
	{
		// cache hit
		return it->second->build(importNormals, importTexCoords, importAnimations, flipTexU, flipTexV, texImporter);
	}
	// should never occur
	return std::shared_ptr<FBXMesh>();
}

std::pair<std::string,std::shared_ptr<FBXMesh::Builder>> 
FBXImporter::produceResourceBuilder(std::string const& meshFileName, bool importNormals, bool importTexCoords, bool importAnimations, bool flipTexU, bool flipTexV, TextureImporter* texImporter)
{
	if(isCached(meshFileName))
	{
		return std::make_pair(meshFileName,std::shared_ptr<FBXMesh::Builder>());
	}
	logger << "[FBXImporter] Import FBX file " << meshFileName << endl;

	initSdk();

	// Step 1: create builder and fill
	std::shared_ptr<FBXMesh::Builder> builder = std::shared_ptr<FBXMesh::Builder>(new FBXMesh::Builder);

	builder->fbxScene = FbxScene::Create(gSdkManager, "My Scene");
	if(!builder->fbxScene)
	{
		logger << "[FBXImporter] Error: Unable to create FBX scene!" << endl;
		return std::make_pair(meshFileName, std::shared_ptr<FBXMesh::Builder>());
	}

	builder->meshFilePath = meshFileName;
	size_t pos = meshFileName.find_last_of("/");
	if( pos != std::string::npos)
	{
		builder->name = meshFileName.substr(pos+1);
	}
	else 
	{
		pos = meshFileName.find_last_of("\\");
		if( pos != std::string::npos)
		{
			builder->name = meshFileName.substr(pos+1);
		}
		else 
		{
			builder->name = meshFileName;
		}
	}

	// Create the importer.
	int fileFormat = -1;
	builder->fbxImporter = FbxImporter::Create(gSdkManager,"");
	if (!gSdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(meshFileName.c_str(), fileFormat) )
	{
		// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
		fileFormat = gSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription( "FBX binary (*.fbx)" );;
	}
	

	// Initialize the importer by providing a filename.
	if(!builder->fbxImporter->Initialize(meshFileName.c_str(), fileFormat) == true)
	{
		logger << "[FBXImporter] Error: Unable to open file '" << meshFileName << "'! " << builder->fbxImporter->GetStatus().GetErrorString() << endl;
		return std::make_pair(meshFileName, std::shared_ptr<FBXMesh::Builder>());
	}

	if (builder->fbxImporter->Import(builder->fbxScene) == true)
	{

		// Convert Axis System to what is used in the file, if needed
		FbxAxisSystem SceneAxisSystem = builder->fbxScene->GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
		if( SceneAxisSystem != OurAxisSystem )
		{
			OurAxisSystem.ConvertScene(builder->fbxScene);
		}

		// Convert Unit System to what is used in this example, if needed
		FbxSystemUnit SceneSystemUnit = builder->fbxScene->GetGlobalSettings().GetSystemUnit();
		if( SceneSystemUnit.GetScaleFactor() != 1.0 )
		{
			//The unit in this example is centimeter.
			FbxSystemUnit::cm.ConvertScene(builder->fbxScene);
		}

		// Get the list of all the animation stack.
		builder->fbxScene->FillAnimStackNameArray(builder->animStackNameArray);

		if(!importAnimations || builder->animStackNameArray.Size() == 0)
		{
			builder->isAnimated = false;
		}
		else 
		{
			builder->isAnimated = true;
		}
		logger << "[FBXImporter] Is animated: " << builder->isAnimated << endl;
		
		if(builder->isAnimated)
		{
			logger << "[FBXImporter] Available Animations: " << endl;
			for(int i = 0; i < builder->animStackNameArray.Size(); ++i)
			{
				logger << builder->animStackNameArray[i]->Buffer() << endl;
			}
		}

		// Get the list of all the cameras in the scene.
		_fillCameraArray(builder);

		// Convert mesh, NURBS and patch into triangle mesh
		FbxGeometryConverter lGeomConverter(gSdkManager);
		lGeomConverter.Triangulate(builder->fbxScene, /*replace*/true);

		// Split meshes per material, so that we only have one material per mesh (for VBO support)
		lGeomConverter.SplitMeshesPerMaterial(builder->fbxScene, /*replace*/true);

		// Bake the scene for one frame
		_loadCacheRecursive(builder->fbxScene, builder->initialAnimLayer, meshFileName.c_str(), true, 0, builder);

		// initialize cache start and stop time
		builder->cacheStart = FBXSDK_TIME_INFINITE;
		builder->cacheStop = FBXSDK_TIME_MINUS_INFINITE;
		
		// Convert any .PC2 point cache data into the .MC format for 
		// vertex cache deformer playback.
		_preparePointCacheData(builder->fbxScene, builder->cacheStart, builder->cacheStop);

		// Get the list of pose in the scene
		_fillPoseArray(builder->fbxScene, builder->poseArray);
		builder->poseIndex = 1; // set default pose


		if(builder->isAnimated)
		{
		
			// select the base layer from the animation stack
			FbxAnimStack * lCurrentAnimationStack = builder->fbxScene->FindMember<FbxAnimStack>(builder->animStackNameArray[builder->poseIndex]->Buffer());
			if (lCurrentAnimationStack == NULL)
			{
				// this is a problem. The anim stack should be found in the scene!
				logger << "[FBXImporter] Error: No AnimationStack found in FBX-Scene from file '" << meshFileName << "'! " << endl;
				return std::make_pair(meshFileName, std::shared_ptr<FBXMesh::Builder>());
			}

			// we assume that the first animation layer connected to the animation stack is the base layer
			// (this is the assumption made in the FBXSDK)
			builder->initialAnimLayer = lCurrentAnimationStack->GetMember<FbxAnimLayer>();
			builder->initialAnimationStack = lCurrentAnimationStack;

		

			// init animation state
			FbxTakeInfo* lCurrentTakeInfo = builder->fbxScene->GetTakeInfo(*(builder->animStackNameArray[builder->poseIndex]));
			if (lCurrentTakeInfo)
			{
				builder->start = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
				builder->stop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
			}
			else
			{
				// Take the time line value
				FbxTimeSpan lTimeLineTimeSpan;
				builder->fbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);

				builder->start = lTimeLineTimeSpan.GetStart();
				builder->stop  = lTimeLineTimeSpan.GetStop();
			}

			// check for smallest start with cache start
			if(builder->cacheStart < builder->start)
				builder->start = builder->cacheStop;

			// check for biggest stop with cache stop
			if(builder->cacheStop  > builder->stop)  
				builder->stop  = builder->cacheStop;
		}
	}

	// Destroy the importer to release the file.
	builder->fbxImporter->Destroy();
	builder->fbxImporter = NULL;

	return std::make_pair(meshFileName, builder);
}

void
FBXImporter::_fillCameraArray(std::shared_ptr<FBXMesh::Builder> builder) 
{
	_fillCameraArrayRecursive(builder->fbxScene->GetRootNode(), builder->cameraArray);
}

void
FBXImporter::_fillCameraArrayRecursive(FbxNode* pNode, FbxArray<FbxNode*>& pCameraArray) 
{
	if(pNode)
	{
		if(pNode->GetNodeAttribute())
		{
			if(pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eCamera)
			{
				pCameraArray.Add(pNode);
			}
		}
		const int lCount = pNode->GetChildCount();
		for(int i = 0; i < lCount; ++i)
		{
			_fillCameraArrayRecursive(pNode->GetChild(i), pCameraArray);
		}
	}
}

void
FBXImporter::_loadCacheRecursive(FbxScene* pScene, 
								 FbxAnimLayer* pAnimLayer, 
								 const char * pFbxFileName, 
								 bool pSupportVBO, 
								 unsigned int nextVaoOffset, 
								 std::shared_ptr<FBXMesh::Builder> builder) 
{
	// Load the textures into GPU, only for file texture now
	const int lTextureCount = pScene->GetTextureCount();
	for (int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex)
	{

		FbxFileTexture * lFileTexture = FbxCast<FbxFileTexture>(pScene->GetTexture(lTextureIndex));

		if(lFileTexture != nullptr)
		{
			builder->meshTextures.push_back(lFileTexture);
		}

		const char* pTextureUses[] = { "Standard", "Shadow Map", "Light Map",
			"Spherical Reflexion Map", "Sphere Reflexion Map", "Bump Normal Map" };

		logger << "[FBXImporter] Warning: Texture loading is not supported but type is " << lFileTexture->GetTextureUse() << " and use is " << pTextureUses[lFileTexture->GetTextureUse()] << " and name is " << lFileTexture->GetFileName() << "!" << endl;
		
		continue;
		if (lFileTexture && !lFileTexture->GetUserDataPtr())
		{
			// Try to load the texture from absolute path
			const FbxString lFileName = lFileTexture->GetFileName();
			lFileTexture->TextureTypeUse;
			unsigned int lTextureObject = 0;
			bool lStatus = false;//LoadTextureFromFile(lFileName, lTextureObject);

			const FbxString lAbsFbxFileName = FbxPathUtils::Resolve(pFbxFileName);
			const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(lAbsFbxFileName);
			if (!lStatus)
			{
				// Load texture from relative file name (relative to FBX file)
				const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lFileTexture->GetRelativeFileName());
				lStatus = false;//LoadTextureFromFile(lResolvedFileName, lTextureObject);
			}

			if (!lStatus)
			{
				// Load texture from file name only (relative to FBX file)
				const FbxString lTextureFileName = FbxPathUtils::GetFileName(lFileName);
				const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lTextureFileName);
				lStatus = false;//LoadTextureFromFile(lResolvedFileName, lTextureObject);
			}

			if (!lStatus)
			{
				logger << "Failed to load texture file: " << lFileName.Buffer() << endl;
				continue;
			}

			if (lStatus)
			{
				unsigned int * lTextureName = new unsigned int(lTextureObject);
				lFileTexture->SetUserDataPtr(lTextureName);
			}
		}
	}

	_loadCacheRecursive(pScene->GetRootNode(), pAnimLayer, pSupportVBO, nextVaoOffset, builder);
}

// Bake node attributes and materials under this node recursively.
// Currently only mesh, light and material.
void
FBXImporter::_loadCacheRecursive(FbxNode * pNode, 
								 FbxAnimLayer * pAnimLayer, 
								 bool pSupportVBO, 
								 unsigned int& nextVaoOffset, 
								 std::shared_ptr<FBXMesh::Builder> builder) 
{
	// Bake material and hook as user data.
	const int lMaterialCount = pNode->GetMaterialCount();
	for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
	{
		FbxSurfaceMaterial* lMaterial = pNode->GetMaterial(lMaterialIndex);
		if (lMaterial && !lMaterial->GetUserDataPtr())
		{
			FbxAutoPtr<FBXMaterialCache> lMaterialCache(new FBXMaterialCache);
			if (lMaterialCache->Initialize(lMaterial))
			{
				lMaterial->SetUserDataPtr(lMaterialCache.Release());
			}
		}
	}

	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
	if (lNodeAttribute)
	{
		// Bake mesh as VBO(vertex buffer object) into GPU.
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh * lMesh = pNode->GetMesh();
			if (pSupportVBO && lMesh && !lMesh->GetUserDataPtr())
			{

#ifndef USE_META_DATA
				FbxAutoPtr<FBXVBOMesh> lMeshCache(new FBXVBOMesh);
				if (lMeshCache->Initialize(lMesh))
				{
					lMesh->SetUserDataPtr(lMeshCache.Release());
				}
#else
				logger << "[FBXImporter] Create vao offset " << nextVaoOffset << endl;
				FBXMesh::Builder::MeshMetaData singleMeshMetaData(lMesh, nextVaoOffset);

				if(singleMeshMetaData.vertices.size() > 0)
				{
					FbxMetaData* fbxMetaData = new FbxMetaData(nextVaoOffset);
					fbxMetaData->allByControlPoint = singleMeshMetaData.allByControlPoint;
					FbxAutoPtr<FbxMetaData> metaData(fbxMetaData);
					lMesh->SetUserDataPtr(metaData.Release());
					builder->meshMetaData.push_back(singleMeshMetaData);
					++nextVaoOffset;
				}
#endif


			}
		}
		// Bake light properties.
		else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eLight)
		{
			FbxLight * lLight = pNode->GetLight();
			if (lLight && !lLight->GetUserDataPtr())
			{
				FbxAutoPtr<FBXLightCache> lLightCache(new FBXLightCache);
				if (lLightCache->Initialize(lLight, pAnimLayer))
				{
					lLight->SetUserDataPtr(lLightCache.Release());
				}
			}
		}
	}

	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		_loadCacheRecursive(pNode->GetChild(lChildIndex), pAnimLayer, pSupportVBO, nextVaoOffset, builder);
	}
}

void
FBXImporter::_preparePointCacheData(FbxScene* pScene, FbxTime &pCache_Start, FbxTime &pCache_Stop) 
{
	// This function show how to cycle through scene elements in a linear way.
	const int lNodeCount = pScene->GetSrcObjectCount<FbxNode>();
	FbxStatus lStatus;

	for (int lIndex=0; lIndex<lNodeCount; lIndex++)
	{
		FbxNode* lNode = pScene->GetSrcObject<FbxNode>(lIndex);

		if (lNode->GetGeometry()) 
		{
			int i, lVertexCacheDeformerCount = lNode->GetGeometry()->GetDeformerCount(FbxDeformer::eVertexCache);

			// There should be a maximum of 1 Vertex Cache Deformer for the moment
			lVertexCacheDeformerCount = lVertexCacheDeformerCount > 0 ? 1 : 0;

			for (i=0; i<lVertexCacheDeformerCount; ++i )
			{
				// Get the Point Cache object
				FbxVertexCacheDeformer* lDeformer = static_cast<FbxVertexCacheDeformer*>(lNode->GetGeometry()->GetDeformer(i, FbxDeformer::eVertexCache));
				if( !lDeformer ) continue;
				FbxCache* lCache = lDeformer->GetCache();
				if( !lCache ) continue;

				// Process the point cache data only if the constraint is active
				if (lDeformer->Active)
				{
					if (lCache->GetCacheFileFormat() == FbxCache::eMaxPointCacheV2)
					{
						// This code show how to convert from PC2 to MC point cache format
						// turn it on if you need it.
#if 0 
						if (!lCache->ConvertFromPC2ToMC(FbxCache::eMCOneFile, 
							FbxTime::GetFrameRate(pScene->GetGlobalTimeSettings().GetTimeMode())))
						{
							// Conversion failed, retrieve the error here
							FbxString lTheErrorIs = lCache->GetStaus().GetErrorString();
						}
#endif
					}
					else if (lCache->GetCacheFileFormat() == FbxCache::eMayaCache)
					{
						// This code show how to convert from MC to PC2 point cache format
						// turn it on if you need it.
						//#if 0 
						if (!lCache->ConvertFromMCToPC2(FbxTime::GetFrameRate(pScene->GetGlobalSettings().GetTimeMode()), 0, &lStatus))
						{
							// Conversion failed, retrieve the error here
							FbxString lTheErrorIs = lStatus.GetErrorString();
						}
						//#endif
					}


					// Now open the cache file to read from it
					if (!lCache->OpenFileForRead(&lStatus))
					{
						// Cannot open file 
						FbxString lTheErrorIs = lStatus.GetErrorString();

						// Set the deformer inactive so we don't play it back
						lDeformer->Active = false;
					}
					else
					{
						// get the start and stop time of the cache
						int lChannelCount = lCache->GetChannelCount();
							
						for (int iChannelNo=0; iChannelNo < lChannelCount; iChannelNo++)
						{
							FbxTime lChannel_Start;
							FbxTime lChannel_Stop;

							if(lCache->GetAnimationRange(iChannelNo, lChannel_Start, lChannel_Stop))
							{
								// get the smallest start time
								if(lChannel_Start < pCache_Start) pCache_Start = lChannel_Start;

								// get the biggest stop time
								if(lChannel_Stop  > pCache_Stop)  pCache_Stop  = lChannel_Stop;
							}
						}
					}
				}
			}
		}
	}
}
// Find all poses in this scene.
void
FBXImporter::_fillPoseArray(FbxScene* pScene, FbxArray<FbxPose*>& pPoseArray) 
{
	const int lPoseCount = pScene->GetPoseCount();

	for (int i=0; i < lPoseCount; ++i)
	{
		pPoseArray.Add(pScene->GetPose(i));
	}
}

FbxManager* FBXImporter::gSdkManager = nullptr;

void
FBXImporter::initSdk() 
{
	// lazy initialization
	if(!gSdkManager) 
	{
		// Create the FBX SDK manager which is the object allocator for almost 
		// all the classes in the SDK and create the scene.
		gSdkManager = FbxManager::Create();
		if(!gSdkManager)
		{
			logger << "[FBXImporter] Error: Unable to create FBX Manager!" << endl;
			return;
		}
		logger << "[FBXImporter] FBX SDK Version: " << gSdkManager->GetVersion() << endl;

		//Create an IOSettings object. This object holds all import/export settings.
		FbxIOSettings* ios = FbxIOSettings::Create(gSdkManager, IOSROOT);
		gSdkManager->SetIOSettings(ios);

		//Load plugins from the executable directory (optional)
		FbxString lPath = FbxGetApplicationDirectory();
		gSdkManager->LoadPluginsDirectory(lPath.Buffer());
	}
}

FBXImporter::~FBXImporter() 
{
	
	// Delete the FBX SDK manager. All the objects that have been allocated 
	// using the FBX SDK manager and that haven't been explicitly destroyed 
	// are automatically destroyed at the same time.
	if(gSdkManager != nullptr)
	{
		// IMPORTANT - call destructor on all builders BEFORE deleting the FBX SDK Manager!
		mCache.clear();
		gSdkManager->Destroy();
		gSdkManager = nullptr;
	}
}