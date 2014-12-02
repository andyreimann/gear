// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "FBXAnimationSystem.h"
#include "FBXAnimationComponent.h"
#include "RenderComponent.h"

using namespace G2;

#define G2_OPTIMIZE_FBX_ANIM 1

// FBXMatrix helper functions

// Scale all the elements of a matrix.
void fbxMatrixScale(FbxAMatrix& pMatrix, double pValue)
{
	for (int i = 0; i < 4; ++i)
	{
#ifndef G2_OPTIMIZE_FBX_ANIM
		for (int j = 0; j < 4; ++j)
		{
			pMatrix[i][j] *= pValue;
		}
#else
		// unrolled loop
		pMatrix[i][0] *= pValue;
		pMatrix[i][1] *= pValue;
		pMatrix[i][2] *= pValue;
		pMatrix[i][3] *= pValue;
#endif
	}
}


// Add a value to all the elements in the diagonal of the matrix.
void fbxMatrixAddToDiagonal(FbxAMatrix& pMatrix, double pValue)
{
	pMatrix[0][0] += pValue;
	pMatrix[1][1] += pValue;
	pMatrix[2][2] += pValue;
	pMatrix[3][3] += pValue;
}

// Sum two matrices element by element.
void fbxMatrixAdd(FbxAMatrix& pDstMatrix, FbxAMatrix& pSrcMatrix)
{
	for (int i = 0; i < 4; ++i)
	{
#ifndef G2_OPTIMIZE_FBX_ANIM
		for (int j = 0; j < 4; ++j)
		{
			pDstMatrix[i][j] += pSrcMatrix[i][j];
		}
#else
		// unrolled loop
		pDstMatrix[i][0] += pSrcMatrix[i][0];
		pDstMatrix[i][1] += pSrcMatrix[i][1];
		pDstMatrix[i][2] += pSrcMatrix[i][2];
		pDstMatrix[i][3] += pSrcMatrix[i][3];
#endif
	}
}

void
FBXAnimationSystem::runPhase(std::string const& name, FrameInfo const& frameInfo)
{
	if(name == "postUpdate") 
	{
		// update all animations
		
		for (int i = 0; i < components.size() ; ++i) 
		{
			FBXAnimationComponent& comp = components[i];

			if(mRenderSystem == nullptr) 
			{
				mRenderSystem = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>();
			}
			// check 
			auto* renderComponent = mRenderSystem->get(comp.getEntityId());
#ifdef USE_META_DATA
			if(renderComponent == nullptr)
			{
				continue; // no RenderComponent attached for some strange reason!
			}
#endif
			// reconnect scene to this animation
			comp.fbxScene->SetCurrentAnimationStack(comp.animationState.currentAnimationStack);

			if(!_updateAnimationStack(&comp, frameInfo))
			{
				continue; // no animation update needed or animation is paused
			}

			if(!comp.fbxScene)
			{
				continue; // no valid FBXScene attached
			}

			// get the pose for the currently set pose
			FbxPose * lPose = NULL;
			if (comp.animationState.poseIndex != -1)
			{
				lPose = comp.fbxScene->GetPose(comp.animationState.poseIndex);
			}

			FbxAMatrix lDummyGlobalPosition;
			// draw the hole scene
			_animateNodeRecursive(comp.fbxScene->GetRootNode(), 
							   comp.animationState.currentTime, 
							   comp.animationState.currentAnimLayer, 
							   lDummyGlobalPosition, 
							   lPose, 
							   ShadingMode::SHADING_MODE_SHADED,
							   renderComponent);
		}
	}
}

bool
FBXAnimationSystem::_updateAnimationStack(FBXAnimationComponent* component, FrameInfo const& frameInfo) 
{
	// Loop in the animation stack if not paused.
	if (component->animationState.stop > component->animationState.start && !component->animationState.pause)
	{

		// Set the scene status flag to refresh 
		// the scene in the next timer callback.
		//mStatus = MUST_BE_REFRESHED;

		component->animationState.timeSinceLastAnimationFrame += frameInfo.timeSinceLastFrame;

		// animate with 60 FPS
		if(component->animationState.timeSinceLastAnimationFrame > component->animationData.invFrameRate || component->animationState.updateFirstFrame) 
		{
			int numFrames = 1;
			
			// when an animation is rendered in it's first frame
			// we have to update it's pose from the base pose to the first frame
			if(!component->animationState.updateFirstFrame)
			{
				numFrames = (int)(component->animationState.timeSinceLastAnimationFrame * component->animationData.frameRate);
				component->animationState.timeSinceLastAnimationFrame -= component->animationData.invFrameRate*numFrames;
			}
			else
			{
				component->animationState.updateFirstFrame = false;
			}
			FbxTime frameTime;
			frameTime.SetFrame(numFrames, component->fbxScene->GetGlobalSettings().GetTimeMode());

			component->animationState.currentTime += frameTime;

			if (component->animationState.currentTime > component->animationState.stop)
			{
				component->animationState.currentTime = component->animationState.start;
			}
			return true;
		}
	}
	return false;
}

// Draw recursively each node of the scene. To avoid recomputing 
// uselessly the global positions, the global position of each 
// node is passed to it's children while browsing the node tree.
// If the node is part of the given pose for the current scene,
// it will be drawn at the position specified in the pose, Otherwise
// it will be drawn at the given time.
void
FBXAnimationSystem::_animateNodeRecursive(FbxNode* pNode, 
									   FbxTime& pTime, 
									   FbxAnimLayer* pAnimLayer, 
									   FbxAMatrix& pParentGlobalPosition, 
									   FbxPose* pPose, 
									   ShadingMode pShadingMode,
									   RenderComponent* component) 
{
	FbxAMatrix lGlobalPosition = _getGlobalPosition(pNode, pTime, pPose, &pParentGlobalPosition);
	
	if(pNode->GetNodeAttribute())
	{
		// Geometry offset.
		// it is not inherited by the children.
		FbxAMatrix lGeometryOffset = _getGeometry(pNode);
		FbxAMatrix lGlobalOffPosition = lGlobalPosition * lGeometryOffset;

		_animateNode(pNode, pTime, pAnimLayer, pParentGlobalPosition, lGlobalOffPosition, pPose, pShadingMode, component);
	}

	const int lChildCount = pNode->GetChildCount();
	for(int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		_animateNodeRecursive(pNode->GetChild(lChildIndex), pTime, pAnimLayer, lGlobalPosition, pPose, pShadingMode, component);
	}
}

// Get the global position of the node for the current pose.
// If the specified node is not part of the pose or no pose is specified, get its
// global position at the current time.
FbxAMatrix
FBXAnimationSystem::_getGlobalPosition(FbxNode* pNode, const FbxTime& pTime, FbxPose* pPose, FbxAMatrix* pParentGlobalPosition) 
{
	FbxAMatrix lGlobalPosition;
	bool        lPositionFound = false;

	if (pPose)
	{
		int lNodeIndex = pPose->Find(pNode);

		if (lNodeIndex > -1)
		{
			// The bind pose is always a global matrix.
			// If we have a rest pose, we need to check if it is
			// stored in global or local space.
			if (pPose->IsBindPose() || !pPose->IsLocalMatrix(lNodeIndex))
			{
				lGlobalPosition = _getPoseMatrix(pPose, lNodeIndex);
			}
			else
			{
				// We have a local matrix, we need to convert it to
				// a global space matrix.
				FbxAMatrix lParentGlobalPosition;

				if (pParentGlobalPosition)
				{
					lParentGlobalPosition = *pParentGlobalPosition;
				}
				else
				{
					if (pNode->GetParent())
					{
						lParentGlobalPosition = _getGlobalPosition(pNode->GetParent(), pTime, pPose);
					}
				}

				FbxAMatrix lLocalPosition = _getPoseMatrix(pPose, lNodeIndex);
				lGlobalPosition = lParentGlobalPosition * lLocalPosition;
			}

			lPositionFound = true;
		}
	}

	if (!lPositionFound)
	{
		// There is no pose entry for that node, get the current global position instead.

		// Ideally this would use parent global position and local position to compute the global position.
		// Unfortunately the equation 
		//    lGlobalPosition = pParentGlobalPosition * lLocalPosition
		// does not hold when inheritance type is other than "Parent" (RSrs).
		// To compute the parent rotation and scaling is tricky in the RrSs and Rrs cases.
		lGlobalPosition = pNode->EvaluateGlobalTransform(pTime);
	}

	return lGlobalPosition;
}

// Get the matrix of the given pose
FbxAMatrix
FBXAnimationSystem::_getPoseMatrix(FbxPose* pPose, int pNodeIndex) 
{
	FbxAMatrix lPoseMatrix;
	FbxMatrix lMatrix = pPose->GetMatrix(pNodeIndex);

	memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

	return lPoseMatrix;
}

// Get the geometry offset to a node. It is never inherited by the children.
FbxAMatrix
FBXAnimationSystem::_getGeometry(FbxNode* pNode) 
{
	const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}

// Draw the node following the content of it's node attribute.
void
FBXAnimationSystem::_animateNode(FbxNode* pNode, 
							  FbxTime& pTime, 
							  FbxAnimLayer* pAnimLayer, 
							  FbxAMatrix& pParentGlobalPosition, 
							  FbxAMatrix& pGlobalPosition, 
							  FbxPose* pPose, 
							  ShadingMode pShadingMode, 
							  RenderComponent* component) 
{
	 FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

	if (lNodeAttribute)
	{
		// All lights has been processed before the whole scene because they influence every geometry.
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMarker)
		{
			//DrawMarker(pGlobalPosition);
		}
		else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			//DrawSkeleton(pNode, pParentGlobalPosition, pGlobalPosition);
		}
		// NURBS and patch have been converted into triangluation meshes.
		else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			_animateMesh(pNode, pTime, pAnimLayer, pGlobalPosition, pPose, pShadingMode, component);
		}
		else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eCamera)
		{
			//DrawCamera(pNode, pTime, pAnimLayer, pGlobalPosition);
		}
		else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNull)
		{
			//DrawNull(pGlobalPosition);
		}
	}
	else
	{
		// Draw a Null for nodes without attribute.
		//DrawNull(pGlobalPosition);
	}
}

// Draw the vertices of a mesh.
void
FBXAnimationSystem::_animateMesh(FbxNode* pNode, 
							  FbxTime& pTime, 
							  FbxAnimLayer* pAnimLayer, 
							  FbxAMatrix& pGlobalPosition, 
							  FbxPose* pPose, 
							  ShadingMode pShadingMode, 
							  RenderComponent* component) 
{
	FbxMesh* lMesh = pNode->GetMesh();
	const int lVertexCount = lMesh->GetControlPointsCount();

	// No vertex to draw.
	if (lVertexCount == 0)
	{
		return;
	}

	// GET THE USER DATA POINTER OUT OF THE MESH CREATED EARLIER!
#ifndef USE_META_DATA
	const FBXVBOMesh* lMeshCache = static_cast<const FBXVBOMesh *>(lMesh->GetUserDataPtr());
#else
	FbxMetaData* metaData = static_cast<FbxMetaData *>(lMesh->GetUserDataPtr());
#endif

	// If it has some deformer connection, update the vertices position
	const bool lHasVertexCache = lMesh->GetDeformerCount(FbxDeformer::eVertexCache) &&
		(static_cast<FbxVertexCacheDeformer*>(lMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->Active;
	const bool lHasShape = lMesh->GetShapeCount() > 0;
	const bool lHasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
	const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;
	// check if this instance has imported normal vectors
	bool hasNormals = component->getVertexArray(metaData->vaoOffset).getNumBytesBySemantic(G2Core::Semantics::NORMAL) > 0u;
	
	FbxVector4* lVertexArray = nullptr;
	FbxArray<FbxVector4>* lNormalArray = nullptr;
#ifndef USE_META_DATA
	if (!lMeshCache || lHasDeformation)
#else
	if (!metaData || lHasDeformation)
#endif
	{
		lVertexArray = new FbxVector4[lVertexCount];

		if(hasNormals)
		{
			// Get the normal vectors we want to animate if the mesh has normals provided
			lNormalArray = new FbxArray<FbxVector4>(lVertexCount);
			if(metaData->allByControlPoint)
			{
				const FbxGeometryElementNormal * lNormalElement = lMesh->GetElementNormal(0);
				FbxVector4 lCurrentNormal;
				for (int lIndex = 0; lIndex < lVertexCount; ++lIndex)
				{
					int lNormalIndex = lIndex;
					if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
					{
						lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndex);
					}
					lNormalArray->SetAt(lIndex, lNormalElement->GetDirectArray().GetAt(lNormalIndex));
				}
			}
			else
			{
				for (int lIndex = 0; lIndex < lVertexCount; ++lIndex)
				{
					for (int lVerticeIndex = 0; lVerticeIndex < 3; ++lVerticeIndex)
					{
						const int lControlPointIndex = lMesh->GetPolygonVertex(lIndex, lVerticeIndex);
						lNormalArray->SetAt(lIndex, lMesh->GetControlPoints()[lControlPointIndex]);
					}
				}
			}
		}

		memcpy(lVertexArray, lMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
	}

	if (lHasDeformation)
	{
		// Active vertex cache deformer will overwrite any other deformer
		if (lHasVertexCache)
		{
			_readVertexCacheData(lMesh, pTime, lVertexArray);
		}
		else
		{
			if (lHasShape)
			{
				// Deform the vertex array with the shapes.
				_computeShapeDeformation(lMesh, pTime, pAnimLayer, lVertexArray);
			}

			//we need to get the number of clusters
			const int lSkinCount = lMesh->GetDeformerCount(FbxDeformer::eSkin);
			int lClusterCount = 0;
			for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
			{
				lClusterCount += ((FbxSkin *)(lMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin)))->GetClusterCount();
			}
			if (lClusterCount)
			{
				// Deform the vertex array with the skin deformer.
				_computeSkinDeformation(pGlobalPosition, lMesh, pTime, lVertexArray, pPose, hasNormals ? lNormalArray->GetArray() : nullptr);
			}
		}
#ifndef USE_META_DATA
		if (lMeshCache)
			lMeshCache->UpdateVertexPosition(lMesh, lVertexArray);
#else
		if (metaData) 
		{
			_updateVertexPosition(lMesh, lVertexArray, hasNormals ? lNormalArray->GetArray() : nullptr, component, metaData);
		}
#endif
	}
	
#ifndef USE_META_DATA
	glPushMatrix();
	glMultMatrixd((const double*)pGlobalPosition);
	if (lMeshCache)
	{
		lMeshCache->BeginDraw(pShadingMode);
		const int lSubMeshCount = lMeshCache->GetSubMeshCount();
		for (int lIndex = 0; lIndex < lSubMeshCount; ++lIndex)
		{
			if (pShadingMode == SHADING_MODE_SHADED)
			{
				const FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lIndex);
				if (lMaterial)
				{
					const FBXMaterialCache* lMaterialCache = static_cast<const FBXMaterialCache*>(lMaterial->GetUserDataPtr());
					if (lMaterialCache)
					{
						//lMaterialCache->SetCurrentMaterial();
					}
				}
				else
				{
					// Draw green for faces without material
					//MaterialCache::SetDefaultMaterial();
				}
			}

			lMeshCache->Draw(lIndex, pShadingMode);
		}
		lMeshCache->EndDraw();
	}
	else
	{
		// OpenGL driver is too lower and use Immediate Mode
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		const int lPolygonCount = lMesh->GetPolygonCount();
		for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; lPolygonIndex++)
		{
			const int lVerticeCount = lMesh->GetPolygonSize(lPolygonIndex);
			glBegin(GL_LINE_LOOP);
			for (int lVerticeIndex = 0; lVerticeIndex < lVerticeCount; lVerticeIndex++)
			{
				glVertex3dv((GLdouble *)lVertexArray[lMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex)]);
			}
			glEnd();
		}
	}

	glPopMatrix();
#endif
	delete [] lVertexArray;
	delete lNormalArray;
}

void
FBXAnimationSystem::_updateVertexPosition(const FbxMesh * pMesh, 
										  const FbxVector4 * pVertices, 
										  const FbxVector4 * pNormals, 
										  RenderComponent* component, 
										  FbxMetaData* metaData) const 
{
	int lVertexCount = 0;

	if (metaData->allByControlPoint)
	{
		lVertexCount = pMesh->GetControlPointsCount();
		metaData->vertexCache.resize(lVertexCount);
		if(pNormals != nullptr)
		{
			metaData->normalCache.resize(lVertexCount);
		}
		for (int lIndex = 0; lIndex < lVertexCount; ++lIndex)
		{			
			metaData->vertexCache[lIndex].x = static_cast<float>(pVertices[lIndex][0]);
			metaData->vertexCache[lIndex].y = static_cast<float>(pVertices[lIndex][1]);
			metaData->vertexCache[lIndex].z = static_cast<float>(pVertices[lIndex][2]);
			if(pNormals != nullptr)
			{
				metaData->normalCache[lIndex].x = static_cast<float>(pNormals[lIndex][0]);
				metaData->normalCache[lIndex].y = static_cast<float>(pNormals[lIndex][1]);
				metaData->normalCache[lIndex].z = static_cast<float>(pNormals[lIndex][2]);
				metaData->normalCache[lIndex] = glm::normalize(metaData->normalCache[lIndex]);
			}
		}
	}
	else
	{
		const int lPolygonCount = pMesh->GetPolygonCount();
		int lVertexCount = 0;
		for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
		{
			for (int lVerticeIndex = 0; lVerticeIndex < 3; ++lVerticeIndex)
			{
				const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);

				metaData->vertexCache[lVertexCount].x = static_cast<float>(pVertices[lControlPointIndex][0]);
				metaData->vertexCache[lVertexCount].y = static_cast<float>(pVertices[lControlPointIndex][1]);
				metaData->vertexCache[lVertexCount].z = static_cast<float>(pVertices[lControlPointIndex][2]);
				
				if(pNormals != nullptr)
				{
					metaData->normalCache[lVertexCount].x = static_cast<float>(pNormals[lControlPointIndex][0]);
					metaData->normalCache[lVertexCount].y = static_cast<float>(pNormals[lControlPointIndex][1]);
					metaData->normalCache[lVertexCount].z = static_cast<float>(pNormals[lControlPointIndex][2]);
					metaData->normalCache[lVertexCount] = glm::normalize(metaData->normalCache[lVertexCount]);
				}
				++lVertexCount;
			}
		}
	}

	component->getVertexArray(metaData->vaoOffset).writeData(G2Core::Semantics::POSITION, &metaData->vertexCache[0]);
	if(pNormals != nullptr)
	{
		component->getVertexArray(metaData->vaoOffset).writeData(G2Core::Semantics::NORMAL, &metaData->normalCache[0]);
	}
}

void
FBXAnimationSystem::_readVertexCacheData(FbxMesh* pMesh, FbxTime& pTime, FbxVector4* pVertexArray) 
{
	FbxVertexCacheDeformer* lDeformer     = static_cast<FbxVertexCacheDeformer*>(pMesh->GetDeformer(0, FbxDeformer::eVertexCache));
	FbxCache*               lCache        = lDeformer->GetCache();
	int                      lChannelIndex = -1;
	unsigned int             lVertexCount  = (unsigned int)pMesh->GetControlPointsCount();
	bool                     lReadSucceed  = false;
	double*                  lReadBuf      = new double[3*lVertexCount];

	if (lCache->GetCacheFileFormat() == FbxCache::eMayaCache)
	{
		if ((lChannelIndex = lCache->GetChannelIndex(lDeformer->Channel.GetName())) > -1)
		{
			lReadSucceed = lCache->Read(lChannelIndex, pTime, lReadBuf, lVertexCount);
		}
	}
	else // eMaxPointCacheV2
	{
		lReadSucceed = lCache->Read((unsigned int)pTime.GetFrameCount(), lReadBuf, lVertexCount);
	}

	if (lReadSucceed)
	{
		unsigned int lReadBufIndex = 0;

		while (lReadBufIndex < 3*lVertexCount)
		{
			// In statements like "pVertexArray[lReadBufIndex/3].SetAt(2, lReadBuf[lReadBufIndex++])", 
			// on Mac platform, "lReadBufIndex++" is evaluated before "lReadBufIndex/3". 
			// So separate them.
			pVertexArray[lReadBufIndex/3].mData[0] = lReadBuf[lReadBufIndex]; lReadBufIndex++;
			pVertexArray[lReadBufIndex/3].mData[1] = lReadBuf[lReadBufIndex]; lReadBufIndex++;
			pVertexArray[lReadBufIndex/3].mData[2] = lReadBuf[lReadBufIndex]; lReadBufIndex++;
		}
	}

	delete [] lReadBuf;
}

void
FBXAnimationSystem::_computeShapeDeformation(FbxMesh* pMesh, FbxTime& pTime, FbxAnimLayer * pAnimLayer, FbxVector4* pVertexArray) 
{

	// since pVertexArray is already a copy of the base geometry, we can optimize
	// and obviously remove some of the memcpy operations here



	int lVertexCount = pMesh->GetControlPointsCount();
	
	FbxVector4* lSrcVertexArray = pVertexArray;

	
#ifndef G2_OPTIMIZE_FBX_ANIM
	FbxVector4* lDstVertexArray = new FbxVector4[lVertexCount];
	memcpy(lDstVertexArray, pVertexArray, lVertexCount * sizeof(FbxVector4));
#else
	FbxVector4* lDstVertexArray = nullptr;
#endif

#ifdef G2_OPTIMIZE_FBX_ANIM
	bool changed = false;
#endif
	int lBlendShapeDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eBlendShape);
	for(int lBlendShapeIndex = 0; lBlendShapeIndex<lBlendShapeDeformerCount; ++lBlendShapeIndex)
	{
		FbxBlendShape* lBlendShape = (FbxBlendShape*)pMesh->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);

		int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
		for(int lChannelIndex = 0; lChannelIndex<lBlendShapeChannelCount; ++lChannelIndex)
		{
			FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);
			if(lChannel)
			{
				// Get the percentage of influence on this channel.
				FbxAnimCurve* lFCurve = pMesh->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer);
				if (!lFCurve) 
				{
					continue;
				}
#ifdef G2_OPTIMIZE_FBX_ANIM
				if(!changed)
				{
					// first time - do some lazy init
					lDstVertexArray = new FbxVector4[lVertexCount];
					memcpy(lDstVertexArray, pVertexArray, lVertexCount * sizeof(FbxVector4));
				}
				changed = true;
#endif
				double lWeight = lFCurve->Evaluate(pTime);

				/*
				If there is only one targetShape on this channel, the influence is easy to calculate:
				influence = (targetShape - baseGeometry) * weight * 0.01
				dstGeometry = baseGeometry + influence

				But if there are more than one targetShapes on this channel, this is an in-between 
				blendshape, also called progressive morph. The calculation of influence is different.
				
				For example, given two in-between targets, the full weight percentage of first target
				is 50, and the full weight percentage of the second target is 100.
				When the weight percentage reach 50, the base geometry is already be fully morphed 
				to the first target shape. When the weight go over 50, it begin to morph from the 
				first target shape to the second target shape.

				To calculate influence when the weight percentage is 25:
				1. 25 falls in the scope of 0 and 50, the morphing is from base geometry to the first target.
				2. And since 25 is already half way between 0 and 50, so the real weight percentage change to 
				the first target is 50.
				influence = (firstTargetShape - baseGeometry) * (25-0)/(50-0) * 100
				dstGeometry = baseGeometry + influence

				To calculate influence when the weight percentage is 75:
				1. 75 falls in the scope of 50 and 100, the morphing is from the first target to the second.
				2. And since 75 is already half way between 50 and 100, so the real weight percentage change 
				to the second target is 50.
				influence = (secondTargetShape - firstTargetShape) * (75-50)/(100-50) * 100
				dstGeometry = firstTargetShape + influence
				*/

				// Find the two shape indices for influence calculation according to the weight.
				// Consider index of base geometry as -1.

				int lShapeCount = lChannel->GetTargetShapeCount();
				double* lFullWeights = lChannel->GetTargetShapeFullWeights();

				// Find out which scope the lWeight falls in.
				int lStartIndex = -1;
				int lEndIndex = -1;
				for(int lShapeIndex = 0; lShapeIndex<lShapeCount; ++lShapeIndex)
				{
					if(lWeight > 0 && lWeight <= lFullWeights[0])
					{
						lEndIndex = 0;
						break;
					}
					if(lWeight > lFullWeights[lShapeIndex] && lWeight < lFullWeights[lShapeIndex+1])
					{
						lStartIndex = lShapeIndex;
						lEndIndex = lShapeIndex + 1;
						break;
					}
				}

				FbxShape* lStartShape = NULL;
				FbxShape* lEndShape = NULL;
				if(lStartIndex > -1)
				{
					lStartShape = lChannel->GetTargetShape(lStartIndex);
				}
				if(lEndIndex > -1)
				{
					lEndShape = lChannel->GetTargetShape(lEndIndex);
				}

				//The weight percentage falls between base geometry and the first target shape.
				if(lStartIndex == -1 && lEndShape) 
				{
					double lEndWeight = lFullWeights[0];
					// Calculate the real weight.
					lWeight = (lWeight/lEndWeight) * 100;
					// Initialize the lDstVertexArray with vertex of base geometry.
					memcpy(lDstVertexArray, lSrcVertexArray, lVertexCount * sizeof(FbxVector4));
					for (int j = 0; j < lVertexCount; j++)
					{
						// Add the influence of the shape vertex to the mesh vertex.
#ifdef G2_OPTIMIZE_FBX_ANIM
						FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - pMesh->GetControlPoints()[j]) * lWeight * 0.01;
#else
						FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lSrcVertexArray[j]) * lWeight * 0.01;
#endif


						lDstVertexArray[j] += lInfluence;
					}	
				}
				//The weight percentage falls between two target shapes.
				else if(lStartShape && lEndShape)
				{
					double lStartWeight = lFullWeights[lStartIndex];
					double lEndWeight = lFullWeights[lEndIndex];
					// Calculate the real weight.
					lWeight = ((lWeight-lStartWeight)/(lEndWeight-lStartWeight)) * 100;
					// Initialize the lDstVertexArray with vertex of the previous target shape geometry.
					memcpy(lDstVertexArray, lStartShape->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
					for (int j = 0; j < lVertexCount; ++j)
					{
						// Add the influence of the shape vertex to the previous shape vertex.
						FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lStartShape->GetControlPoints()[j]) * lWeight * 0.01;
						lDstVertexArray[j] += lInfluence;
					}	
				}
			}//If lChannel is valid
		}//For each blend shape channel
	}//For each blend shape deformer
#ifdef G2_OPTIMIZE_FBX_ANIM
	if(changed) 
#endif
	{
		memcpy(pVertexArray, lDstVertexArray, lVertexCount * sizeof(FbxVector4));
	}

	delete [] lDstVertexArray;
}

// Deform the vertex array according to the links contained in the mesh and the skinning type.
void
FBXAnimationSystem::_computeSkinDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxTime& pTime, FbxVector4* pVertexArray, FbxPose* pPose, FbxVector4* pNormalArray) 
{
	FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(0, FbxDeformer::eSkin);
	FbxSkin::EType lSkinningType = lSkinDeformer->GetSkinningType();

	if(lSkinningType == FbxSkin::eLinear || lSkinningType == FbxSkin::eRigid)
	{
		_computeLinearDeformation(pGlobalPosition, pMesh, pTime, pVertexArray, pNormalArray, pPose);
	}
	else if(lSkinningType == FbxSkin::eDualQuaternion)
	{
		_computeDualQuaternionDeformation(pGlobalPosition, pMesh, pTime, pVertexArray, pPose);
	}
	else if(lSkinningType == FbxSkin::eBlend)
	{
		int lVertexCount = pMesh->GetControlPointsCount();

		FbxVector4* lVertexArrayLinear = new FbxVector4[lVertexCount];
		memcpy(lVertexArrayLinear, pMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

		FbxVector4* lVertexArrayDQ = new FbxVector4[lVertexCount];
		memcpy(lVertexArrayDQ, pMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

		// TODO PASSING pNormal IS OBVIOUSLY NOT RIGHT HERE!
		_computeLinearDeformation(pGlobalPosition, pMesh, pTime, lVertexArrayLinear, pNormalArray, pPose);
		_computeDualQuaternionDeformation(pGlobalPosition, pMesh, pTime, lVertexArrayDQ, pPose);

		// To blend the skinning according to the blend weights
		// Final vertex = DQSVertex * blend weight + LinearVertex * (1- blend weight)
		// DQSVertex: vertex that is deformed by dual quaternion skinning method;
		// LinearVertex: vertex that is deformed by classic linear skinning method;
		int lBlendWeightsCount = lSkinDeformer->GetControlPointIndicesCount();
		for(int lBWIndex = 0; lBWIndex<lBlendWeightsCount; ++lBWIndex)
		{
			double lBlendWeight = lSkinDeformer->GetControlPointBlendWeights()[lBWIndex];
			pVertexArray[lBWIndex] = lVertexArrayDQ[lBWIndex] * lBlendWeight + lVertexArrayLinear[lBWIndex] * (1 - lBlendWeight);
		}

		// TEST
		delete[] lVertexArrayLinear;
		delete[] lVertexArrayDQ;
	}
}

// Deform the vertex array in classic linear way.
void
FBXAnimationSystem::_computeLinearDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxTime& pTime, FbxVector4* pVertexArray, FbxVector4* pNormalArray, FbxPose* pPose) 
{
	// All the links must have the same link mode.
	FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

	int lVertexCount = pMesh->GetControlPointsCount();
	FbxAMatrix* lClusterDeformation = new FbxAMatrix[lVertexCount];
	memset(lClusterDeformation, 0, lVertexCount * sizeof(FbxAMatrix));

	double* lClusterWeight = new double[lVertexCount];
	memset(lClusterWeight, 0, lVertexCount * sizeof(double));

	if (lClusterMode == FbxCluster::eAdditive)
	{
		for (int i = 0; i < lVertexCount; ++i)
		{
			lClusterDeformation[i].SetIdentity();
		}
	}

	// For all skins and all clusters, accumulate their deformation and weight
	// on each vertices and store them in lClusterDeformation and lClusterWeight.
	int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	for ( int lSkinIndex=0; lSkinIndex<lSkinCount; ++lSkinIndex)
	{
		FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);
		
		int lClusterCount = lSkinDeformer->GetClusterCount();

#ifdef G2_OPTIMIZE_FBX_ANIM

#endif

		for ( int lClusterIndex=0; lClusterIndex<lClusterCount; ++lClusterIndex)
		{
			FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
			if (!lCluster->GetLink())
				continue;

			FbxAMatrix lVertexTransformMatrix;
			_computeClusterDeformation(pGlobalPosition, pMesh, lCluster, lVertexTransformMatrix, pTime, pPose);
			
			int lVertexIndexCount = lCluster->GetControlPointIndicesCount();
			for (int k = 0; k < lVertexIndexCount; ++k) 
			{            
				int lIndex = lCluster->GetControlPointIndices()[k];

				// Sometimes, the mesh can have less points than at the time of the skinning
				// because a smooth operator was active when skinning but has been deactivated during export.
				if (lIndex >= lVertexCount)
					continue;

				double lWeight = lCluster->GetControlPointWeights()[k];

				if (lWeight == 0.0)
				{
					continue;
				}

				// Compute the influence of the link on the vertex.
				FbxAMatrix lInfluence = lVertexTransformMatrix;
				fbxMatrixScale(lInfluence, lWeight);

				if (lClusterMode == FbxCluster::eAdditive)
				{    
					// Multiply with the product of the deformations on the vertex.
					fbxMatrixAddToDiagonal(lInfluence, 1.0 - lWeight);
					lClusterDeformation[lIndex] = lInfluence * lClusterDeformation[lIndex];

					// Set the link to 1.0 just to know this vertex is influenced by a link.
					lClusterWeight[lIndex] = 1.0;
				}
				else // lLinkMode == FbxCluster::eNormalize || lLinkMode == FbxCluster::eTotalOne
				{
					// Add to the sum of the deformations on the vertex.
					fbxMatrixAdd(lClusterDeformation[lIndex], lInfluence);

					// Add to the sum of weights to either normalize or complete the vertex.
					lClusterWeight[lIndex] += lWeight;
				}
			}//For each vertex			
		}//lClusterCount
	}

	//Actually deform each vertices here by information stored in lClusterDeformation and lClusterWeight
	for (int i = 0; i < lVertexCount; i++) 
	{
		FbxVector4 lSrcVertex = pVertexArray[i];
		FbxVector4& lDstVertex = pVertexArray[i];
		
		FbxVector4 lSrcNormal;
		FbxVector4* lDstNormal;
		if(pNormalArray != nullptr)
		{
			pNormalArray[i].Normalize();
			lSrcNormal = pNormalArray[i];
			lDstNormal = &pNormalArray[i];
		}
		double lWeight = lClusterWeight[i];

		// Deform the vertex if there was at least a link with an influence on the vertex,
		if (lWeight != 0.0) 
		{
			lDstVertex = lClusterDeformation[i].MultT(lSrcVertex);
			// use inverse transpose for normals
			if(pNormalArray != nullptr)
			{
				*lDstNormal = lClusterDeformation[i].Inverse().Transpose().MultT(lSrcNormal);
				lDstNormal->Normalize();
			}
			if (lClusterMode == FbxCluster::eNormalize)
			{
				// In the normalized link mode, a vertex is always totally influenced by the links. 
				lDstVertex /= lWeight;
				if(pNormalArray != nullptr)
				{
					*lDstNormal /= lWeight;
				}
			}
			else if (lClusterMode == FbxCluster::eTotalOne)
			{
				// In the total 1 link mode, a vertex can be partially influenced by the links. 
				lSrcVertex *= (1.0 - lWeight);
				lDstVertex += lSrcVertex; 
				if(pNormalArray != nullptr)
				{
					lSrcNormal *= (1.0 - lWeight);
					*lDstNormal += lSrcNormal;
				}
			}
		} 
	}
	
	delete [] lClusterDeformation;
	delete [] lClusterWeight;
}

void
FBXAnimationSystem::_computeClusterDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxCluster* pCluster, FbxAMatrix& pVertexTransformMatrix, FbxTime pTime, FbxPose* pPose) 
{

	// Note only the pCluster is changed for 60 calls!

	FbxCluster::ELinkMode lClusterMode = pCluster->GetLinkMode();

	FbxAMatrix lReferenceGlobalInitPosition;
	FbxAMatrix lReferenceGlobalCurrentPosition;
	FbxAMatrix lAssociateGlobalInitPosition;
	FbxAMatrix lAssociateGlobalCurrentPosition;
	FbxAMatrix lClusterGlobalInitPosition;
	FbxAMatrix lClusterGlobalCurrentPosition;

	FbxAMatrix lReferenceGeometry;
	FbxAMatrix lAssociateGeometry;
	FbxAMatrix lClusterGeometry;

	FbxAMatrix lClusterRelativeInitPosition;
	FbxAMatrix lClusterRelativeCurrentPositionInverse;
	
	if (lClusterMode == FbxCluster::eAdditive && pCluster->GetAssociateModel())
	{
		pCluster->GetTransformAssociateModelMatrix(lAssociateGlobalInitPosition);
		// Geometric transform of the model
		lAssociateGeometry = _getGeometry(pCluster->GetAssociateModel());
		lAssociateGlobalInitPosition *= lAssociateGeometry;
		lAssociateGlobalCurrentPosition = _getGlobalPosition(pCluster->GetAssociateModel(), pTime, pPose);

		pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
		// Multiply lReferenceGlobalInitPosition by Geometric Transformation
		lReferenceGeometry = _getGeometry(pMesh->GetNode());
		lReferenceGlobalInitPosition *= lReferenceGeometry;
		lReferenceGlobalCurrentPosition = pGlobalPosition;

		// Get the link initial global position and the link current global position.
		pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
		// Multiply lClusterGlobalInitPosition by Geometric Transformation
		lClusterGeometry = _getGeometry(pCluster->GetLink());
		lClusterGlobalInitPosition *= lClusterGeometry;
		lClusterGlobalCurrentPosition = _getGlobalPosition(pCluster->GetLink(), pTime, pPose);

		// Compute the shift of the link relative to the reference.
		//ModelM-1 * AssoM * AssoGX-1 * LinkGX * LinkM-1*ModelM
		pVertexTransformMatrix = lReferenceGlobalInitPosition.Inverse() * lAssociateGlobalInitPosition * lAssociateGlobalCurrentPosition.Inverse() *
			lClusterGlobalCurrentPosition * lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;
	}
	else
	{
		pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
		lReferenceGlobalCurrentPosition = pGlobalPosition;
		// Multiply lReferenceGlobalInitPosition by Geometric Transformation
		lReferenceGeometry = _getGeometry(pMesh->GetNode());
		lReferenceGlobalInitPosition *= lReferenceGeometry;

		// Get the link initial global position and the link current global position.
		pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
		lClusterGlobalCurrentPosition = _getGlobalPosition(pCluster->GetLink(), pTime, pPose);

		// Compute the initial position of the link relative to the reference.
		lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

		// Compute the current position of the link relative to the reference.
		lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;

		// Compute the shift of the link relative to the reference.
		pVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;
	}
}

void
FBXAnimationSystem::_computeDualQuaternionDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxTime& pTime, FbxVector4* pVertexArray, FbxPose* pPose) 
{
	// All the links must have the same link mode.
	FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

	int lVertexCount = pMesh->GetControlPointsCount();
	int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);

	FbxDualQuaternion* lDQClusterDeformation = new FbxDualQuaternion[lVertexCount];
	memset(lDQClusterDeformation, 0, lVertexCount * sizeof(FbxDualQuaternion));

	double* lClusterWeight = new double[lVertexCount];
	memset(lClusterWeight, 0, lVertexCount * sizeof(double));

	// For all skins and all clusters, accumulate their deformation and weight
	// on each vertices and store them in lClusterDeformation and lClusterWeight.
	for ( int lSkinIndex=0; lSkinIndex<lSkinCount; ++lSkinIndex)
	{
		FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);
		int lClusterCount = lSkinDeformer->GetClusterCount();
		for ( int lClusterIndex=0; lClusterIndex<lClusterCount; ++lClusterIndex)
		{
			FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
			if (!lCluster->GetLink())
				continue;

			FbxAMatrix lVertexTransformMatrix;
			_computeClusterDeformation(pGlobalPosition, pMesh, lCluster, lVertexTransformMatrix, pTime, pPose);

			FbxQuaternion lQ = lVertexTransformMatrix.GetQ();
			FbxVector4 lT = lVertexTransformMatrix.GetT();
			FbxDualQuaternion lDualQuaternion(lQ, lT);

			int lVertexIndexCount = lCluster->GetControlPointIndicesCount();
			for (int k = 0; k < lVertexIndexCount; ++k) 
			{ 
				int lIndex = lCluster->GetControlPointIndices()[k];

				// Sometimes, the mesh can have less points than at the time of the skinning
				// because a smooth operator was active when skinning but has been deactivated during export.
				if (lIndex >= lVertexCount)
					continue;

				double lWeight = lCluster->GetControlPointWeights()[k];

				if (lWeight == 0.0)
					continue;

				// Compute the influence of the link on the vertex.
				FbxDualQuaternion lInfluence = lDualQuaternion * lWeight;
				if (lClusterMode == FbxCluster::eAdditive)
				{    
					// Simply influenced by the dual quaternion.
					lDQClusterDeformation[lIndex] = lInfluence;

					// Set the link to 1.0 just to know this vertex is influenced by a link.
					lClusterWeight[lIndex] = 1.0;
				}
				else // lLinkMode == FbxCluster::eNormalize || lLinkMode == FbxCluster::eTotalOne
				{
					if(lClusterIndex == 0)
					{
						lDQClusterDeformation[lIndex] = lInfluence;
					}
					else
					{
						// Add to the sum of the deformations on the vertex.
						// Make sure the deformation is accumulated in the same rotation direction. 
						// Use dot product to judge the sign.
						double lSign = lDQClusterDeformation[lIndex].GetFirstQuaternion().DotProduct(lDualQuaternion.GetFirstQuaternion());
						if( lSign >= 0.0 )
						{
							lDQClusterDeformation[lIndex] += lInfluence;
						}
						else
						{
							lDQClusterDeformation[lIndex] -= lInfluence;
						}
					}
					// Add to the sum of weights to either normalize or complete the vertex.
					lClusterWeight[lIndex] += lWeight;
				}
			}//For each vertex
		}//lClusterCount
	}

	//Actually deform each vertices here by information stored in lClusterDeformation and lClusterWeight
	for (int i = 0; i < lVertexCount; ++i) 
	{
		FbxVector4 lSrcVertex = pVertexArray[i];
		FbxVector4& lDstVertex = pVertexArray[i];
		double lWeightSum = lClusterWeight[i];

		// Deform the vertex if there was at least a link with an influence on the vertex,
		if (lWeightSum != 0.0) 
		{
			lDQClusterDeformation[i].Normalize();
			lDstVertex = lDQClusterDeformation[i].Deform(lDstVertex);

			if (lClusterMode == FbxCluster::eNormalize)
			{
				// In the normalized link mode, a vertex is always totally influenced by the links. 
				lDstVertex /= lWeightSum;
			}
			else if (lClusterMode == FbxCluster::eTotalOne)
			{
				// In the total 1 link mode, a vertex can be partially influenced by the links. 
				lSrcVertex *= (1.0 - lWeightSum);
				lDstVertex += lSrcVertex;
			}
		} 
	}

	delete [] lDQClusterDeformation;
	delete [] lClusterWeight;
}