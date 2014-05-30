/* Copyright (c) <2003-2011> <Julio Jerez, Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef __DGCOLLISION_USERMESH__
#define __DGCOLLISION_USERMESH__

#include "dgCollision.h"
#include "dgCollisionMesh.h"


typedef void (dgApi *OnUserMeshDestroyCallback) (void* const userData);

typedef void (dgApi *OnUserMeshCollideCallback) (void* const collideData);
typedef dgFloat32 (dgApi *OnUserMeshRayHitCallback) (dgCollisionMeshRayHitDesc& rayHitdata);
typedef void (dgApi *OnUserMeshCollisionInfo) (void* userData, dgCollisionInfo* infoRecord);
typedef dgInt32 (dgApi *OnUserMeshAABBOverlapTest) (void* const userData, const dgVector& boxP0, const dgVector& boxP1);
typedef void (dgApi *OnUserMeshFacesInAABB) (void* userData, const dgFloat32* p0, const dgFloat32* p1,
										   const dgFloat32** vertexArray, dgInt32* vertexCount, dgInt32* vertexStrideInBytes, 
										   const dgInt32* indexList, dgInt32 maxIndexCount, const dgInt32* faceAttribute);
typedef void (dgApi *OnUserMeshSerialize) (void* const userCollisionData, dgSerialize function, void* const serilalizeObject);


class dgUserMeshCreation
{
	public:
	void* m_userData;
	OnUserMeshSerialize m_serializeCallback;
	OnUserMeshCollideCallback m_collideCallback;
	OnUserMeshRayHitCallback m_rayHitCallback;
	OnUserMeshDestroyCallback m_destroyCallback;
	OnUserMeshCollisionInfo m_getInfoCallback;
	OnUserMeshFacesInAABB m_faceInAABBCallback;
	OnUserMeshAABBOverlapTest m_getAABBOvelapTestCallback;
};


class dgCollisionUserMesh: public dgCollisionMesh
{
	public:
	dgCollisionUserMesh(dgWorld* const world, const dgVector& boxP0, const dgVector& boxP1, const dgUserMeshCreation& data);
	dgCollisionUserMesh (dgWorld* const world, dgDeserialize deserialization, void* const userData);
	virtual ~dgCollisionUserMesh(void);

	void GetVertexListIndexList (const dgVector& p0, const dgVector& p1, dgMeshVertexListIndexList &data) const;

	bool AABBOvelapTest (const dgVector& boxP0, const dgVector& boxP1) const;

	private:
	void Serialize(dgSerialize callback, void* const userData) const;

	dgVector SupportVertex (const dgVector& dir) const;

	virtual void GetCollisionInfo(dgCollisionInfo* const info) const;
	virtual dgFloat32 RayCast (const dgVector& localP0, const dgVector& localP1, dgFloat32 maxT, dgContactPoint& contactOut, const dgBody* const body, void* const userData) const;
	virtual void GetCollidingFaces (dgPolygonMeshDesc* const data) const;
	virtual void DebugCollision (const dgMatrix& matrixPtr, OnDebugCollisionMeshCallback callback, void* const userData) const;

	void* m_userData;
	OnUserMeshSerialize m_serializeCallback;
	OnUserMeshCollisionInfo m_getInfoCallback;
	OnUserMeshFacesInAABB m_faceInAABBCalback;
	OnUserMeshRayHitCallback m_rayHitCallback;
	OnUserMeshCollideCallback m_collideCallback;
	OnUserMeshDestroyCallback m_destroyCallback;
	OnUserMeshAABBOverlapTest m_getAABBOvelapTestCallback;
};


#endif
