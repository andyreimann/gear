// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "FBXMesh.h"
#include "FBXAnimationComponent.h"
#include "RenderComponent.h"
#include "NameComponent.h"
#include "FBXLightCache.h"
#include "FBXMaterialCache.h"
#include "TextureImporter.h"

using namespace G2;

const int TRIANGLE_VERTEX_COUNT = 3;

// Four floats for every position.
const int VERTEX_STRIDE = 4;
// Three floats for every normal.
const int NORMAL_STRIDE = 3;
// Two floats for every UV.
const int UV_STRIDE = 2;


FBXMesh::Builder::~Builder() 
{
	FbxArrayDelete(animStackNameArray);
	deleteScene();
}

void FBXMesh::Builder::deleteScene()
{
	//const int lTextureCount = pScene->GetTextureCount();
	//for (int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex)
	//{
	//    FbxTexture * lTexture = pScene->GetTexture(lTextureIndex);
	//    FbxFileTexture * lFileTexture = FbxCast<FbxFileTexture>(lTexture);
	//    if (lFileTexture && lFileTexture->GetUserDataPtr())
	//    {
	//        GLuint * lTextureName = static_cast<GLuint *>(lFileTexture->GetUserDataPtr());
	//        lFileTexture->SetUserDataPtr(NULL);
	//        glDeleteTextures(1, lTextureName);
	//        delete lTextureName;
	//    }
	//}

	unloadCacheRecursive(fbxScene->GetRootNode());
}

void FBXMesh::Builder::unloadCacheRecursive(FbxNode * pNode)
{
	// Unload the material cache
	const int lMaterialCount = pNode->GetMaterialCount();
	for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
	{
		FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lMaterialIndex);
		if (lMaterial && lMaterial->GetUserDataPtr())
		{
			FBXMaterialCache* lMaterialCache = static_cast<FBXMaterialCache*>(lMaterial->GetUserDataPtr());
			lMaterial->SetUserDataPtr(NULL);
			delete lMaterialCache;
		}
	}

	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
	if (lNodeAttribute)
	{
		// Unload the mesh cache
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh * lMesh = pNode->GetMesh();
			if (lMesh && lMesh->GetUserDataPtr())
			{

#ifndef USE_META_DATA
			VBOMesh * lMeshCache = static_cast<VBOMesh *>(lMesh->GetUserDataPtr());
			lMesh->SetUserDataPtr(NULL);
			delete lMeshCache;
#else
			FbxMetaData * fbxMetaData = static_cast<FbxMetaData*>(lMesh->GetUserDataPtr());
			lMesh->SetUserDataPtr(NULL);
			delete fbxMetaData;
#endif
			}
		}
		// Unload the light cache
		else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eLight)
		{
			FbxLight * lLight = pNode->GetLight();
			if (lLight && lLight->GetUserDataPtr())
			{
				FBXLightCache* lLightCache = static_cast<FBXLightCache*>(lLight->GetUserDataPtr());
				lLight->SetUserDataPtr(NULL);
				delete lLightCache;
			}
		}
	}

	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		unloadCacheRecursive(pNode->GetChild(lChildIndex));
	}
}

G2::Entity
FBXMesh::Builder::buildResource(bool importNormals, bool importTexCoords, bool importAnimations, bool flipTexU, bool flipTexV, TextureImporter* texImporte, G2::Entity* target)
{
	// create mesh
	G2::Entity meshLocal;
	G2::Entity* mesh = &meshLocal;
	if (target != nullptr)
	{
		mesh = target; // in case the user gave a valid pointer, we don't write into the local mesh
	}
	if(isAnimated && importAnimations)
	{
		// prepare animationData
		FBXAnimationData animData(cacheStart, cacheStop, animStackNameArray, 24.0);
		// prepare animation state
		FBXAnimationState animState(poseIndex, initialAnimLayer, initialAnimationStack, start, stop, false);
	
		auto* fbxAnimationComponent = mesh->addComponent<FBXAnimationComponent>(fbxScene, animState, animData);
	}

#ifdef USE_META_DATA
	// create a RenderComponent with the requested amount of VertexArrayObjects
	auto* renderComponent = mesh->addComponent<RenderComponent>();
	renderComponent->allocateVertexArrays((unsigned int)meshMetaData.size());

	// calculate the number of IndexArrayObjects we need
	unsigned int numIndexArrays = 0;
	for (unsigned int i = 0; i < meshMetaData.size() ; ++i) 
	{
		MeshMetaData const& meshData = meshMetaData[i];
		if(meshData.indices.size() > 0)
		{
			++numIndexArrays;
		}
	}
	renderComponent->allocateIndexArrays(numIndexArrays);

	// init the VAO
	unsigned int totalVertices = 0;
	unsigned int currentIndexArrayIndex = 0;
	for (unsigned int i = 0; i < meshMetaData.size() ; ++i) 
	{
		MeshMetaData const& meshData = meshMetaData[i];
		renderComponent->getVertexArray(i).resizeElementCount((unsigned int)meshData.vertices.size())
			.writeData(G2Core::Semantics::POSITION, &meshData.vertices[0]);
		totalVertices += (unsigned int)meshData.vertices.size();
		
		if(meshData.hasNormals && importNormals)
		{
			renderComponent->getVertexArray(i).writeData(G2Core::Semantics::NORMAL, &meshData.normals[0]);
		}
		if(meshData.hasUvs && importTexCoords)
		{
			if (flipTexU || flipTexV)
			{
				std::vector<glm::vec2> uvCopy = meshData.uvs; // copy
				std::transform(uvCopy.begin(), uvCopy.end(), uvCopy.begin(), [flipTexU,flipTexV](glm::vec2& uv) 
				{ 
					if (flipTexU)
					{
						uv.x = 1.0f - uv.x;
					}
					if (flipTexV)
					{
						uv.y = 1.0f - uv.y;
					}
					return uv; 
				});
				renderComponent->getVertexArray(i).writeData(G2Core::Semantics::TEXCOORD_0, &uvCopy[0]);
			}
			else
			{
				renderComponent->getVertexArray(i).writeData(G2Core::Semantics::TEXCOORD_0, &meshData.uvs[0]);
			}
		}
		if(meshData.indices.size() > 0)
		{
			IndexArrayObject& iao = renderComponent->getIndexArray(currentIndexArrayIndex);
			iao.writeIndices(&meshData.indices[0], (unsigned int)meshData.indices.size());
			
			++currentIndexArrayIndex;
		}
		// add a draw call
		renderComponent->addDrawCall(DrawCall()
			.setDrawMode(G2Core::DrawMode::TRIANGLES)
			.setEnabled(true)
			.setVaoIndex(i)
			.setModelSpaceAABB(meshData.modelSpaceAABB)
			.setIaoIndex(meshData.indices.size() > 0 ? currentIndexArrayIndex-1 : -1)
			.setAABBCalculationMode(AABBCalculationMode::MANUAL));
	}

	G2::logger << "[FBXMesh] Name: " << name << "\n\tIAO: " << renderComponent->getNumIndexArrays() << "\n\tVAO: " << renderComponent->getNumVertexArrays() << "\n\tVertices: " << totalVertices << "\n\tDraw-Calls: " << renderComponent->getNumDrawCalls() << "\n\tTriangles: " << renderComponent->getNumTriangles() << G2::endl;

#endif
	
	auto* nameComponent = mesh->addComponent<NameComponent>();
	nameComponent->name = name;

	if (texImporte != nullptr && renderComponent != nullptr)
	{
		for (int i = 0; i < meshTextures.size(); ++i)
		{
			FbxFileTexture* fbxTex = meshTextures[i];
			G2::Sampler::Name sampler = toSampler(fbxTex->GetTextureUse());

			if (sampler != G2::Sampler::SAMPLER_INVALID && renderComponent->material.getTexture(sampler).get() == nullptr)
			{
				// sampler is valid and RenderComponent has not yet set a texture at that sampler
				const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(FbxPathUtils::Resolve(meshFilePath.c_str()));
				const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, fbxTex->GetRelativeFileName());

				auto tex = texImporte->import(
					lResolvedFileName.Buffer(), 
					G2Core::DataFormat::Internal::R32G32B32A32_F, 
					G2Core::FilterMode::LINEAR, 
					G2Core::FilterMode::LINEAR, 
					false, 
					toWrapMode(fbxTex->GetWrapModeU()), 
					toWrapMode(fbxTex->GetWrapModeV())
				);
				if(tex.get() != nullptr)
				{
					renderComponent->material.setTexture(sampler, tex);
				}
			}
		}
	}

	return std::move(meshLocal);
}

FBXMesh::Builder::MeshMetaData::MeshMetaData(FbxMesh const* mesh, unsigned int vaoOffset) 
	: mesh(mesh),
	vaoOffset(vaoOffset),
	hasNormals(false),
	hasUvs(false)
{
	
	if (!mesh->GetNode())
		return;

	const int lPolygonCount = mesh->GetPolygonCount();

	// Count the polygon count of each material -> split up in submeshes
	FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
	FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
	if (mesh->GetElementMaterial())
	{
		lMaterialIndice = &mesh->GetElementMaterial()->GetIndexArray();
		lMaterialMappingMode = mesh->GetElementMaterial()->GetMappingMode();
		if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
		{
			FBX_ASSERT(lMaterialIndice->GetCount() == lPolygonCount);
			if (lMaterialIndice->GetCount() == lPolygonCount)
			{
				// Count the faces of each material
				for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
				{
					const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
					if (subMeshMetaData.size() < lMaterialIndex + 1)
					{
						subMeshMetaData.resize(lMaterialIndex + 1);
					}

					subMeshMetaData[lMaterialIndex].triangleCount += 1;
				}

				// Make sure we have no "holes" (NULL) in the mSubMeshes table. This can happen
				// if, in the loop above, we resized the mSubMeshes by more than one slot.
				//for (int i = 0; i < subMeshMetaData.size(); i++)
				//{
				//    if (subMeshMetaData[i] == NULL)
				//        mSubMeshes[i] = new SubMesh;
				//}

				// Record the offset (how many vertex)
				const int lMaterialCount = (int)subMeshMetaData.size();
				int lOffset = 0;
				for (int lIndex = 0; lIndex < lMaterialCount; ++lIndex)
				{
					subMeshMetaData[lIndex].indexOffset = lOffset;
					lOffset += subMeshMetaData[lIndex].triangleCount * 3;
					// This will be used as counter in the following procedures, reset to zero
					subMeshMetaData[lIndex].triangleCount = 0;
				}
				FBX_ASSERT(lOffset == lPolygonCount * 3);
			}
		}
	}

	// All faces will use the same material.
	if (subMeshMetaData.size() == 0)
	{
		subMeshMetaData.resize(1);
	}
	
	hasNormals = mesh->GetElementNormalCount() > 0;
	hasUvs = mesh->GetElementUVCount() > 0;

	allByControlPoint = true;
	
	// Congregate all the data of a mesh to be cached.
	// If normal or UV is by polygon vertex, record all vertex attributes by polygon vertex.
	FbxGeometryElement::EMappingMode lNormalMappingMode = FbxGeometryElement::eNone;
	FbxGeometryElement::EMappingMode lUVMappingMode = FbxGeometryElement::eNone;
	if (hasNormals)
	{
		lNormalMappingMode = mesh->GetElementNormal(0)->GetMappingMode();
		if (lNormalMappingMode == FbxGeometryElement::eNone)
		{
			hasNormals = false;
		}
		if (hasNormals && lNormalMappingMode != FbxGeometryElement::eByControlPoint)
		{
			allByControlPoint = false;
		}
	}
	if (hasUvs)
	{
		lUVMappingMode = mesh->GetElementUV(0)->GetMappingMode();
		if (lUVMappingMode == FbxGeometryElement::eNone)
		{
			hasUvs = false;
		}
		if (hasUvs && lUVMappingMode != FbxGeometryElement::eByControlPoint)
		{
			allByControlPoint = false;
		}
	}
	// Allocate the array memory, by control point or by polygon vertex.
	int lPolygonVertexCount = mesh->GetControlPointsCount();
	if (!allByControlPoint)
	{
		lPolygonVertexCount = lPolygonCount * TRIANGLE_VERTEX_COUNT;
	}
	vertices.resize(lPolygonVertexCount);
	indices.resize(lPolygonCount * TRIANGLE_VERTEX_COUNT);


	float * lNormals = NULL;
	if (hasNormals)
	{
		normals.resize(lPolygonVertexCount);
	}
	float * lUVs = NULL;
	FbxStringList lUVNames;
	mesh->GetUVSetNames(lUVNames);
	const char * lUVName = NULL;
	if (hasUvs && lUVNames.GetCount())
	{
		uvs.resize(lPolygonVertexCount);
		lUVName = lUVNames[0];
	}


	// Populate the array with vertex attribute, if by control point.
	const FbxVector4 * lControlPoints = mesh->GetControlPoints();
	FbxVector4 lCurrentVertex;
	FbxVector4 lCurrentNormal;
	FbxVector2 lCurrentUV;
	if (allByControlPoint)
	{
		const FbxGeometryElementNormal * lNormalElement = NULL;
		const FbxGeometryElementUV * lUVElement = NULL;
		if (hasNormals)
		{
			lNormalElement = mesh->GetElementNormal(0);
		}
		if (hasUvs)
		{
			lUVElement = mesh->GetElementUV(0);
		}
		for (int lIndex = 0; lIndex < lPolygonVertexCount; ++lIndex)
		{
			// Save the vertex position.
			lCurrentVertex = lControlPoints[lIndex];

			vertices[lIndex] = glm::vec3(static_cast<float>(lCurrentVertex[0]),
										 static_cast<float>(lCurrentVertex[1]),
										 static_cast<float>(lCurrentVertex[2]));
			modelSpaceAABB.merge(vertices[lIndex]);

			// Save the normal.
			if (hasNormals)
			{
				int lNormalIndex = lIndex;
				if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
				{
					lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndex);
				}
				lCurrentNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
				
				normals[lIndex] = glm::vec3(static_cast<float>(lCurrentNormal[0]),
											 static_cast<float>(lCurrentNormal[1]),
											 static_cast<float>(lCurrentNormal[2]));
			}

			// Save the UV.
			if (hasUvs)
			{
				int lUVIndex = lIndex;
				if (lUVElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
				{
					lUVIndex = lUVElement->GetIndexArray().GetAt(lIndex);
				}
				lCurrentUV = lUVElement->GetDirectArray().GetAt(lUVIndex);

				uvs[lIndex] = glm::vec2(static_cast<float>(lCurrentUV[0]),
										static_cast<float>(lCurrentUV[1]));
			}
		}
	}


	int lVertexCount = 0;
	for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
	{
		// The material for current face.
		int lMaterialIndex = 0;
		if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
		{
			lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
		}

		// Where should I save the vertex attribute index, according to the material
		const int lIndexOffset = subMeshMetaData[lMaterialIndex].indexOffset +
			subMeshMetaData[lMaterialIndex].triangleCount * 3;
		for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; ++lVerticeIndex)
		{
			const int lControlPointIndex = mesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);

			if (allByControlPoint)
			{
				indices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lControlPointIndex);
			}
			// Populate the array with vertex attribute, if by polygon vertex.
			else
			{
				indices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lVertexCount);

				lCurrentVertex = lControlPoints[lControlPointIndex];

				vertices[lVertexCount] = glm::vec3(static_cast<float>(lCurrentVertex[0]),
												   static_cast<float>(lCurrentVertex[1]),
												   static_cast<float>(lCurrentVertex[2]));
				modelSpaceAABB.merge(vertices[lVertexCount]);

				if (hasNormals)
				{
					mesh->GetPolygonVertexNormal(lPolygonIndex, lVerticeIndex, lCurrentNormal);

					normals[lVertexCount] = glm::vec3(static_cast<float>(lCurrentNormal[0]),
													   static_cast<float>(lCurrentNormal[1]),
													   static_cast<float>(lCurrentNormal[2]));
				}

				if (hasUvs)
				{
					bool lUnmappedUV;
					mesh->GetPolygonVertexUV(lPolygonIndex, lVerticeIndex, lUVName, lCurrentUV, lUnmappedUV);
					
					uvs[lVertexCount] = glm::vec2(static_cast<float>(lCurrentUV[0]),
												  static_cast<float>(lCurrentUV[1]));
				}
			}
			++lVertexCount;
		}
		subMeshMetaData[lMaterialIndex].triangleCount += 1;
	}
}

G2::Sampler::Name
G2::FBXMesh::Builder::toSampler(int fbxTextureUse)
{
	switch (fbxTextureUse)
	{
	case fbxsdk_2015_1::FbxTexture::ETextureUse::eStandard:
		return Sampler::DIFFUSE;
	case fbxsdk_2015_1::FbxTexture::ETextureUse::eBumpNormalMap:
		return Sampler::NORMAL;
	case fbxsdk_2015_1::FbxTexture::ETextureUse::eShadowMap:
		return Sampler::SHADOW;
	case fbxsdk_2015_1::FbxTexture::ETextureUse::eLightMap:					//! Light map
	case fbxsdk_2015_1::FbxTexture::ETextureUse::eSphericalReflectionMap:	//! Spherical reflection map: Object reflects the contents of the scene
	case fbxsdk_2015_1::FbxTexture::ETextureUse::eSphereReflectionMap:		//! Sphere reflection map: Object reflects the contents of the scene from only one point of view
	default:
		return Sampler::SAMPLER_INVALID; // not supported right now
	}
}

G2Core::WrapMode::Name
G2::FBXMesh::Builder::toWrapMode(int fbxTextureWrapMode)
{
	switch (fbxTextureWrapMode)
	{
	case fbxsdk_2015_1::FbxTexture::EWrapMode::eClamp:
		return G2Core::WrapMode::CLAMP_TO_EDGE;
	case fbxsdk_2015_1::FbxTexture::EWrapMode::eRepeat:
		return G2Core::WrapMode::REPEAT;
	default:
		return G2Core::WrapMode::REPEAT;
	}
}