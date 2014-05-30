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

#ifndef _DG_COLLISION_TAPERED_CYLINDER_H__
#define _DG_COLLISION_TAPERED_CYLINDER_H__

#include "dgCollisionConvex.h"

#define DG_CYLINDER_SEGMENTS 8


class dgCollisionTaperedCylinder: public dgCollisionConvex  
{
	public:
	dgCollisionTaperedCylinder(dgMemoryAllocator* const allocator, dgUnsigned32 signature, dgFloat32 radio0, dgFloat32 radio1, dgFloat32 height);
	dgCollisionTaperedCylinder(dgWorld* const world, dgDeserialize deserialization, void* const userData);
	virtual ~dgCollisionTaperedCylinder();


	private:
	void Init (dgFloat32 radio0, dgFloat32 radio1, dgFloat32 height);

	virtual dgVector SupportVertex (const dgVector& dir, dgInt32* const vertexIndex) const;

	virtual dgFloat32 CalculateMassProperties (const dgMatrix& offset, dgVector& inertia, dgVector& crossInertia, dgVector& centerOfMass) const;
	virtual void DebugCollision  (const dgMatrix& matrix, OnDebugCollisionMeshCallback callback, void* const userData) const;
	
	virtual dgInt32 CalculatePlaneIntersection (const dgVector& normal, const dgVector& point, dgVector* const contactsOut) const;
	
	virtual dgInt32 CalculateSignature () const;
	virtual void SetCollisionBBox (const dgVector& p0, const dgVector& p1);
	dgFloat32 GetSkinThickness () const;

	virtual void GetCollisionInfo(dgCollisionInfo* const info) const;
	virtual void Serialize(dgSerialize callback, void* const userData) const;

	static dgInt32 CalculateSignature (dgFloat32 radio0, dgFloat32 radio1, dgFloat32 height);

	// special feature based contact calculation for conics convex (ex spheres, capsules, tapered capsules, and chamfered cylinders)
	// in newton we only deal with sub set of conic function, that can be expressed by the equation
	// ((x - x0) / a)^2 + ((y - y0) / b)^2 + ((z - z0) / c)^2  = 1   and possible a linear or circular sweep of the same equation
	// this preclude parabolic and hyperbolic conics 
	virtual dgVector ConvexConicSupporVertex (const dgVector& dir) const;
	virtual dgVector ConvexConicSupporVertex (const dgVector& point, const dgVector& dir) const;
	virtual dgInt32 CalculateContacts (const dgVector& point, const dgVector& normal, dgCollisionParamProxy& proxy, dgVector* const contactsOut) const;



	dgVector m_dirVector;
	dgFloat32 m_height;
	dgFloat32 m_radio0;
	dgFloat32 m_radio1;

	dgVector m_vertex[DG_CYLINDER_SEGMENTS * 2];
	static dgInt32 m_shapeRefCount;
	static dgConvexSimplexEdge m_edgeArray[];

	friend class dgWorld;
};

#endif 

