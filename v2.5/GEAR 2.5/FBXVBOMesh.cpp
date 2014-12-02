// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "FBXVBOMesh.h"
#include "Defines.h"

const float WIREFRAME_COLOR[] = {0.5f, 0.5f, 0.5f, 1.0f};
const int TRIANGLE_VERTEX_COUNT = 3;

// Four floats for every position.
const int VERTEX_STRIDE = 4;
// Three floats for every normal.
const int NORMAL_STRIDE = 3;
// Two floats for every UV.
const int UV_STRIDE = 2;

FBXVBOMesh::FBXVBOMesh() : mHasNormal(false), mHasUV(false), mAllByControlPoint(true)
{
    // Reset every VBO to zero, which means no buffer.
    //for (int lVBOIndex = 0; lVBOIndex < VBO_COUNT; ++lVBOIndex)
    //{
    //    mVBONames[lVBOIndex] = 0;
    //}
}

FBXVBOMesh::~FBXVBOMesh()
{
    // Delete VBO objects, zeros are ignored automatically.
    //glDeleteBuffers(VBO_COUNT, mVBONames);
	
//	FbxArrayDelete(mSubMeshes);

	//for(int i=0; i < mSubMeshes.GetCount(); i++)
	//{
	//	delete mSubMeshes[i];
	//}
	//
	//mSubMeshes.Clear();

}

bool FBXVBOMesh::Initialize(const FbxMesh *pMesh)
{
    //if (!pMesh->GetNode())
    //    return false;

    //const int lPolygonCount = pMesh->GetPolygonCount();

    //// Count the polygon count of each material
    //FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
    //FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
    //if (pMesh->GetElementMaterial())
    //{
    //    lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
    //    lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
    //    if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
    //    {
    //        FBX_ASSERT(lMaterialIndice->GetCount() == lPolygonCount);
    //        if (lMaterialIndice->GetCount() == lPolygonCount)
    //        {
    //            // Count the faces of each material
    //            for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
    //            {
    //                const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
    //                if (mSubMeshes.GetCount() < lMaterialIndex + 1)
    //                {
    //                    mSubMeshes.Resize(lMaterialIndex + 1);
    //                }
    //                if (mSubMeshes[lMaterialIndex] == NULL)
    //                {
    //                    mSubMeshes[lMaterialIndex] = new SubMesh;
    //                }
    //                mSubMeshes[lMaterialIndex]->TriangleCount += 1;
    //            }

    //            // Make sure we have no "holes" (NULL) in the mSubMeshes table. This can happen
    //            // if, in the loop above, we resized the mSubMeshes by more than one slot.
    //            for (int i = 0; i < mSubMeshes.GetCount(); i++)
    //            {
    //                if (mSubMeshes[i] == NULL)
    //                    mSubMeshes[i] = new SubMesh;
    //            }

    //            // Record the offset (how many vertex)
    //            const int lMaterialCount = mSubMeshes.GetCount();
    //            int lOffset = 0;
    //            for (int lIndex = 0; lIndex < lMaterialCount; ++lIndex)
    //            {
    //                mSubMeshes[lIndex]->IndexOffset = lOffset;
    //                lOffset += mSubMeshes[lIndex]->TriangleCount * 3;
    //                // This will be used as counter in the following procedures, reset to zero
    //                mSubMeshes[lIndex]->TriangleCount = 0;
    //            }
    //            FBX_ASSERT(lOffset == lPolygonCount * 3);
    //        }
    //    }
    //}

    //// All faces will use the same material.
    //if (mSubMeshes.GetCount() == 0)
    //{
    //    mSubMeshes.Resize(1);
    //    mSubMeshes[0] = new SubMesh();
    //}

    //// Congregate all the data of a mesh to be cached in VBOs.
    //// If normal or UV is by polygon vertex, record all vertex attributes by polygon vertex.
    //mHasNormal = pMesh->GetElementNormalCount() > 0;
    //mHasUV = pMesh->GetElementUVCount() > 0;
    //FbxGeometryElement::EMappingMode lNormalMappingMode = FbxGeometryElement::eNone;
    //FbxGeometryElement::EMappingMode lUVMappingMode = FbxGeometryElement::eNone;
    //if (mHasNormal)
    //{
    //    lNormalMappingMode = pMesh->GetElementNormal(0)->GetMappingMode();
    //    if (lNormalMappingMode == FbxGeometryElement::eNone)
    //    {
    //        mHasNormal = false;
    //    }
    //    if (mHasNormal && lNormalMappingMode != FbxGeometryElement::eByControlPoint)
    //    {
    //        mAllByControlPoint = false;
    //    }
    //}
    //if (mHasUV)
    //{
    //    lUVMappingMode = pMesh->GetElementUV(0)->GetMappingMode();
    //    if (lUVMappingMode == FbxGeometryElement::eNone)
    //    {
    //        mHasUV = false;
    //    }
    //    if (mHasUV && lUVMappingMode != FbxGeometryElement::eByControlPoint)
    //    {
    //        mAllByControlPoint = false;
    //    }
    //}

    //// Allocate the array memory, by control point or by polygon vertex.
    //int lPolygonVertexCount = pMesh->GetControlPointsCount();
    //if (!mAllByControlPoint)
    //{
    //    lPolygonVertexCount = lPolygonCount * TRIANGLE_VERTEX_COUNT;
    //}
    //float * lVertices = new float[lPolygonVertexCount * VERTEX_STRIDE];
    //unsigned int * lIndices = new unsigned int[lPolygonCount * TRIANGLE_VERTEX_COUNT];
    //float * lNormals = NULL;
    //if (mHasNormal)
    //{
    //    lNormals = new float[lPolygonVertexCount * NORMAL_STRIDE];
    //}
    //float * lUVs = NULL;
    //FbxStringList lUVNames;
    //pMesh->GetUVSetNames(lUVNames);
    //const char * lUVName = NULL;
    //if (mHasUV && lUVNames.GetCount())
    //{
    //    lUVs = new float[lPolygonVertexCount * UV_STRIDE];
    //    lUVName = lUVNames[0];
    //}

    //// Populate the array with vertex attribute, if by control point.
    //const FbxVector4 * lControlPoints = pMesh->GetControlPoints();
    //FbxVector4 lCurrentVertex;
    //FbxVector4 lCurrentNormal;
    //FbxVector2 lCurrentUV;
    //if (mAllByControlPoint)
    //{
    //    const FbxGeometryElementNormal * lNormalElement = NULL;
    //    const FbxGeometryElementUV * lUVElement = NULL;
    //    if (mHasNormal)
    //    {
    //        lNormalElement = pMesh->GetElementNormal(0);
    //    }
    //    if (mHasUV)
    //    {
    //        lUVElement = pMesh->GetElementUV(0);
    //    }
    //    for (int lIndex = 0; lIndex < lPolygonVertexCount; ++lIndex)
    //    {
    //        // Save the vertex position.
    //        lCurrentVertex = lControlPoints[lIndex];
    //        lVertices[lIndex * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
    //        lVertices[lIndex * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
    //        lVertices[lIndex * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
    //        lVertices[lIndex * VERTEX_STRIDE + 3] = 1;

    //        // Save the normal.
    //        if (mHasNormal)
    //        {
    //            int lNormalIndex = lIndex;
    //            if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
    //            {
    //                lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndex);
    //            }
    //            lCurrentNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
    //            lNormals[lIndex * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
    //            lNormals[lIndex * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
    //            lNormals[lIndex * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);
    //        }

    //        // Save the UV.
    //        if (mHasUV)
    //        {
    //            int lUVIndex = lIndex;
    //            if (lUVElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
    //            {
    //                lUVIndex = lUVElement->GetIndexArray().GetAt(lIndex);
    //            }
    //            lCurrentUV = lUVElement->GetDirectArray().GetAt(lUVIndex);
    //            lUVs[lIndex * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
    //            lUVs[lIndex * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);
    //        }
    //    }

    //}

    //int lVertexCount = 0;
    //for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
    //{
    //    // The material for current face.
    //    int lMaterialIndex = 0;
    //    if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
    //    {
    //        lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
    //    }

    //    // Where should I save the vertex attribute index, according to the material
    //    const int lIndexOffset = mSubMeshes[lMaterialIndex]->IndexOffset +
    //        mSubMeshes[lMaterialIndex]->TriangleCount * 3;
    //    for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; ++lVerticeIndex)
    //    {
    //        const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);

    //        if (mAllByControlPoint)
    //        {
    //            lIndices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lControlPointIndex);
    //        }
    //        // Populate the array with vertex attribute, if by polygon vertex.
    //        else
    //        {
    //            lIndices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lVertexCount);

    //            lCurrentVertex = lControlPoints[lControlPointIndex];
    //            lVertices[lVertexCount * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
    //            lVertices[lVertexCount * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
    //            lVertices[lVertexCount * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
    //            lVertices[lVertexCount * VERTEX_STRIDE + 3] = 1;

    //            if (mHasNormal)
    //            {
    //                pMesh->GetPolygonVertexNormal(lPolygonIndex, lVerticeIndex, lCurrentNormal);
    //                lNormals[lVertexCount * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
    //                lNormals[lVertexCount * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
    //                lNormals[lVertexCount * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);
    //            }

    //            if (mHasUV)
    //            {
    //                bool lUnmappedUV;
    //                pMesh->GetPolygonVertexUV(lPolygonIndex, lVerticeIndex, lUVName, lCurrentUV, lUnmappedUV);
    //                lUVs[lVertexCount * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
    //                lUVs[lVertexCount * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);
    //            }
    //        }
    //        ++lVertexCount;
    //    }
    //    mSubMeshes[lMaterialIndex]->TriangleCount += 1;
    //}

    //// Create VBOs
    //glGenBuffers(VBO_COUNT, mVBONames);

    //// Save vertex attributes into GPU
    //glBindBuffer(GL_ARRAY_BUFFER, mVBONames[VERTEX_VBO]);
    //glBufferData(GL_ARRAY_BUFFER, lPolygonVertexCount * VERTEX_STRIDE * sizeof(float), lVertices, GL_STATIC_DRAW);
    //delete [] lVertices;

    //if (mHasNormal)
    //{
    //    glBindBuffer(GL_ARRAY_BUFFER, mVBONames[NORMAL_VBO]);
    //    glBufferData(GL_ARRAY_BUFFER, lPolygonVertexCount * NORMAL_STRIDE * sizeof(float), lNormals, GL_STATIC_DRAW);
    //    delete [] lNormals;
    //}
    //
    //if (mHasUV)
    //{
    //    glBindBuffer(GL_ARRAY_BUFFER, mVBONames[UV_VBO]);
    //    glBufferData(GL_ARRAY_BUFFER, lPolygonVertexCount * UV_STRIDE * sizeof(float), lUVs, GL_STATIC_DRAW);
    //    delete [] lUVs;
    //}
    //
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBONames[INDEX_VBO]);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, lPolygonCount * TRIANGLE_VERTEX_COUNT * sizeof(unsigned int), lIndices, GL_STATIC_DRAW);
    //delete [] lIndices;

    return true;
}

void FBXVBOMesh::UpdateVertexPosition(const FbxMesh * pMesh, const FbxVector4 * pVertices) const
{
    //// Convert to the same sequence with data in GPU.
    //float * lVertices = NULL;
    //int lVertexCount = 0;
    //if (mAllByControlPoint)
    //{
    //    lVertexCount = pMesh->GetControlPointsCount();
    //    lVertices = new float[lVertexCount * VERTEX_STRIDE];
    //    for (int lIndex = 0; lIndex < lVertexCount; ++lIndex)
    //    {
    //        lVertices[lIndex * VERTEX_STRIDE] = static_cast<float>(pVertices[lIndex][0]);
    //        lVertices[lIndex * VERTEX_STRIDE + 1] = static_cast<float>(pVertices[lIndex][1]);
    //        lVertices[lIndex * VERTEX_STRIDE + 2] = static_cast<float>(pVertices[lIndex][2]);
    //        lVertices[lIndex * VERTEX_STRIDE + 3] = 1;
    //    }
    //}
    //else
    //{
    //    const int lPolygonCount = pMesh->GetPolygonCount();
    //    lVertexCount = lPolygonCount * TRIANGLE_VERTEX_COUNT;
    //    lVertices = new float[lVertexCount * VERTEX_STRIDE];

    //    int lVertexCount = 0;
    //    for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
    //    {
    //        for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; ++lVerticeIndex)
    //        {
    //            const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);
    //            lVertices[lVertexCount * VERTEX_STRIDE] = static_cast<float>(pVertices[lControlPointIndex][0]);
    //            lVertices[lVertexCount * VERTEX_STRIDE + 1] = static_cast<float>(pVertices[lControlPointIndex][1]);
    //            lVertices[lVertexCount * VERTEX_STRIDE + 2] = static_cast<float>(pVertices[lControlPointIndex][2]);
    //            lVertices[lVertexCount * VERTEX_STRIDE + 3] = 1;
    //            ++lVertexCount;
    //        }
    //    }
    //}

    //// Transfer into GPU.
    //if (lVertices)
    //{
    //    glBindBuffer(GL_ARRAY_BUFFER, mVBONames[VERTEX_VBO]);
    //    glBufferData(GL_ARRAY_BUFFER, lVertexCount * VERTEX_STRIDE * sizeof(float), lVertices, GL_STATIC_DRAW);
    //    delete [] lVertices;
    //}
}

void FBXVBOMesh::Draw(int pMaterialIndex, ShadingMode pShadingMode) const
{
    //// Where to start.
    //GLsizei lOffset = mSubMeshes[pMaterialIndex]->IndexOffset * sizeof(unsigned int);
    //if ( pShadingMode == SHADING_MODE_SHADED)
    //{
    //    const GLsizei lElementCount = mSubMeshes[pMaterialIndex]->TriangleCount * 3;
    //    glDrawElements(GL_TRIANGLES, lElementCount, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>(lOffset));
    //}
    //else
    //{
    //    for (int lIndex = 0; lIndex < mSubMeshes[pMaterialIndex]->TriangleCount; ++lIndex)
    //    {
    //        // Draw line loop for every triangle.
    //        glDrawElements(GL_LINE_LOOP, TRIANGLE_VERTEX_COUNT, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>(lOffset));
    //        lOffset += sizeof(unsigned int) * TRIANGLE_VERTEX_COUNT;
    //    }
    //}
}

void FBXVBOMesh::BeginDraw(ShadingMode pShadingMode) const
{
    //// Push OpenGL attributes.
    //glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    //glPushAttrib(GL_ENABLE_BIT);
    //glPushAttrib(GL_CURRENT_BIT);
    //glPushAttrib(GL_LIGHTING_BIT);
    //glPushAttrib(GL_TEXTURE_BIT);

    //// Set vertex position array.
    //glBindBuffer(GL_ARRAY_BUFFER, mVBONames[VERTEX_VBO]);
    //glVertexPointer(VERTEX_STRIDE, GL_FLOAT, 0, 0);
    //glEnableClientState(GL_VERTEX_ARRAY);

    //// Set normal array.
    //if (mHasNormal && pShadingMode == SHADING_MODE_SHADED)
    //{
    //    glBindBuffer(GL_ARRAY_BUFFER, mVBONames[NORMAL_VBO]);
    //    glNormalPointer(GL_FLOAT, 0, 0);
    //    glEnableClientState(GL_NORMAL_ARRAY);
    //}
    //
    //// Set UV array.
    //if (mHasUV && pShadingMode == SHADING_MODE_SHADED)
    //{
    //    glBindBuffer(GL_ARRAY_BUFFER, mVBONames[UV_VBO]);
    //    glTexCoordPointer(UV_STRIDE, GL_FLOAT, 0, 0);
    //    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //}

    //// Set index array.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBONames[INDEX_VBO]);

    //if (pShadingMode == SHADING_MODE_SHADED)
    //{
    //    glEnable(GL_LIGHTING);

    //    glEnable(GL_TEXTURE_2D);

    //    glEnable(GL_NORMALIZE);
    //}
    //else
    //{
    //    glColor4fv(WIREFRAME_COLOR);
    //}
}

void FBXVBOMesh::EndDraw() const
{
    //// Reset VBO binding.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    //// Pop OpenGL attributes.
    //glPopAttrib();
    //glPopAttrib();
    //glPopAttrib();
    //glPopAttrib();
    //glPopClientAttrib();
}