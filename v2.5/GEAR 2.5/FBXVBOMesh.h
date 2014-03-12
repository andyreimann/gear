// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"

#include <fbxsdk.h>

enum ShadingMode
{
    SHADING_MODE_WIREFRAME,
    SHADING_MODE_SHADED,
};
// Save mesh vertices, normals, UVs and indices in GPU with OpenGL Vertex Buffer Objects
class FBXVBOMesh
{
public:
    FBXVBOMesh();
    ~FBXVBOMesh();

    // Save up data into GPU buffers.
    bool Initialize(const FbxMesh * pMesh);

    // Update vertex positions for deformed meshes.
    void UpdateVertexPosition(const FbxMesh * pMesh, const FbxVector4 * pVertices) const;

    // Bind buffers, set vertex arrays, turn on lighting and texture.
    void BeginDraw(ShadingMode pShadingMode) const;
    // Draw all the faces with specific material with given shading mode.
    void Draw(int pMaterialIndex, ShadingMode pShadingMode) const;
    // Unbind buffers, reset vertex arrays, turn off lighting and texture.
    void EndDraw() const;

    // Get the count of material groups
    int GetSubMeshCount() const { return mSubMeshes.GetCount(); }

private:
    enum
    {
        VERTEX_VBO,
        NORMAL_VBO,
        UV_VBO,
        INDEX_VBO,
        VBO_COUNT,
    };

    // For every material, record the offsets in every VBO and triangle counts
    struct SubMesh
    {
        SubMesh() : IndexOffset(0), TriangleCount(0) {}

        int IndexOffset;
        int TriangleCount;
    };

    GLuint mVBONames[VBO_COUNT];
    FbxArray<SubMesh*> mSubMeshes;
    bool mHasNormal;
    bool mHasUV;
    bool mAllByControlPoint; // Save data in VBO by control point or by polygon vertex.
};
