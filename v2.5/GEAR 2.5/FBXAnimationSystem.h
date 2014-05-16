// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "FBXVBOMesh.h"
#include "FBXMaterialCache.h"

#include <G2Core/BaseSystem.h>

#include <fbxsdk.h>



namespace G2 
{
	class RenderSystem;
	class RenderComponent;
	class FBXAnimationComponent;
	struct FbxMetaData;

	/** This class is responsible for the animation of the loaded FBXAnimationComponents.
	 * The animation itself takes place in the 'postUpdate' phase during rendering.
	 * @created:	2014/02/19
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class FBXAnimationSystem : public BaseSystem<FBXAnimationSystem,FBXAnimationComponent> 
	{
		public:

			FBXAnimationSystem() 
				: mRenderSystem(nullptr) 
			{}

			void runPhase(std::string const& name, FrameInfo const& frameInfo);

		private:

			bool _updateAnimationStack(FBXAnimationComponent* component, FrameInfo const& frameInfo);

			void _drawNodeRecursive(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer,
									FbxAMatrix& pParentGlobalPosition, FbxPose* pPose,
									ShadingMode pShadingMode,
									RenderComponent* component);

			FbxAMatrix _getGlobalPosition(FbxNode* pNode, 
										  const FbxTime& pTime, 
										  FbxPose* pPose = nullptr, 
										  FbxAMatrix* pParentGlobalPosition = nullptr);
			FbxAMatrix _getPoseMatrix(FbxPose* pPose, int pNodeIndex);

			FbxAMatrix _getGeometry(FbxNode* pNode);

			void _drawNode(FbxNode* pNode, 
						  FbxTime& pTime, 
						  FbxAnimLayer* pAnimLayer,
						  FbxAMatrix& pParentGlobalPosition,
						  FbxAMatrix& pGlobalPosition,
						  FbxPose* pPose, 
						  ShadingMode pShadingMode,
						  RenderComponent* component);

			void _drawMesh(FbxNode* pNode, 
						   FbxTime& pTime, 
						   FbxAnimLayer* pAnimLayer,
						   FbxAMatrix& pGlobalPosition, 
						   FbxPose* pPose, 
						   ShadingMode pShadingMode,
						   RenderComponent* component);

			void _readVertexCacheData(FbxMesh* pMesh, FbxTime& pTime, FbxVector4* pVertexArray);

			void _computeShapeDeformation(FbxMesh* pMesh, 
										  FbxTime& pTime, 
										  FbxAnimLayer * pAnimLayer, 
										  FbxVector4* pVertexArray);

			// Start skin deformation functions
			void _computeSkinDeformation(FbxAMatrix& pGlobalPosition, 
										 FbxMesh* pMesh, 
										 FbxTime& pTime, 
										 FbxVector4* pVertexArray,
										 FbxPose* pPose);

			void _computeLinearDeformation(FbxAMatrix& pGlobalPosition, 
										   FbxMesh* pMesh, 
										   FbxTime& pTime, 
										   FbxVector4* pVertexArray,
										   FbxPose* pPose);
			
			void _computeClusterDeformation(FbxAMatrix& pGlobalPosition, 
										   FbxMesh* pMesh,
							   			   FbxCluster* pCluster, 
										   FbxAMatrix& pVertexTransformMatrix,
										   FbxTime pTime, 
										   FbxPose* pPose);

			void _computeDualQuaternionDeformation(FbxAMatrix& pGlobalPosition, 
												   FbxMesh* pMesh, 
												   FbxTime& pTime, 
												   FbxVector4* pVertexArray,
												   FbxPose* pPose);
			// NEW UPLOAD INTO RENDERCOMPONENT
			void _updateVertexPosition(const FbxMesh * pMesh, 
									   const FbxVector4 * pVertices, 
									   RenderComponent* component,
									   FbxMetaData * metaData) const;

			// Start skin deformation functions

			RenderSystem*	mRenderSystem;
	};
};