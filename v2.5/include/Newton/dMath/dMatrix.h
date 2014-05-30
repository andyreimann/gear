/* Copyright (c) <2009> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/


#ifndef __dMatrix__
#define __dMatrix__

#include "dVector.h"

class dMatrix ;
class dQuaternion;

// small but fully operational 4x4 matrix class
class dQuaternion;

inline dMatrix GetIdentityMatrix();

D_MSC_VECTOR_ALIGMENT
class dMatrix
{
	public:
	dMatrix ();
	dMatrix (const dFloat* const array);
	dMatrix (const dVector &front, const dVector &up, const dVector &right, const dVector &posit);
	dMatrix (const dQuaternion &rotation, const dVector &position);
	dMatrix (dFloat pitch, dFloat yaw, dFloat roll, const dVector& location);

	dVector& operator[] (int i);
	const dVector& operator[] (int i) const;

	dMatrix Inverse () const;
	dMatrix Transpose () const;
	dMatrix Transpose4X4 () const;
	
	dVector RotateVector (const dVector &v) const;
	dVector UnrotateVector (const dVector &v) const;
	dVector TransformVector (const dVector &v) const;
	dVector UntransformVector (const dVector &v) const;
	dVector TransformPlane (const dVector &localPlane) const;
	dVector UntransformPlane (const dVector &globalPlane) const;
	dVector GetEulerAngles(dEulerAngleOrder order = m_pitchYawRoll) const;

	dMatrix Inverse4x4 () const;
	dVector RotateVector4x4 (const dVector &v) const;
	dMatrix JacobiDiagonalization (dVector& eigenValues, const dMatrix& initialMatrix = GetIdentityMatrix()) const;

	// decompose this matrix into [this = transpose(stretchAxis) * matrix(scale) * stretchAxis * transformMatrix];
	void PolarDecomposition (dMatrix& transformMatrix, dVector& scale, dMatrix& stretchAxis, const dMatrix& initialStretchAxis = GetIdentityMatrix()) const;
	
	// constructor for polar composition
	dMatrix (const dMatrix& transformMatrix, const dVector& scale, const dMatrix& stretchAxis);

	void TransformTriplex (dFloat* const dst, int dstStrideInBytes, const dFloat* const src, int srcStrideInBytes, int count) const;
	void TransformTriplex (dFloat64* const dst, int dstStrideInBytes, const dFloat64* const src, int srcStrideInBytes, int count) const;

	dMatrix operator* (const dMatrix & B) const;

	bool SanityCheck() const;

	dVector m_front;
	dVector m_up;
	dVector m_right;
	dVector m_posit;
};


inline dMatrix GetIdentityMatrix()
{
	return dMatrix (dVector (1.0f, 0.0f, 0.0f, 0.0f),
				    dVector (0.0f, 1.0f, 0.0f, 0.0f),
				    dVector (0.0f, 0.0f, 1.0f, 0.0f),
				    dVector (0.0f, 0.0f, 0.0f, 1.0f));
}

inline dMatrix GetZeroMatrix ()
{
	return dMatrix (dVector (0.0f, 0.0f, 0.0f, 0.0f),
					dVector (0.0f, 0.0f, 0.0f, 0.0f),
					dVector (0.0f, 0.0f, 0.0f, 0.0f),
					dVector (0.0f, 0.0f, 0.0f, 0.0f));
}


inline dMatrix::dMatrix ()
{
}

inline dMatrix::dMatrix (
	const dVector &front, 
	const dVector &up,
	const dVector &right,
	const dVector &posit)
	:m_front (front), m_up(up), m_right(right), m_posit(posit)
{
}

inline dMatrix::dMatrix (const dFloat* const array)
{
	memcpy (&(*this)[0][0], array, sizeof (dMatrix));
}

inline dVector& dMatrix::operator[] (int  i)
{
	return (&m_front)[i];
}

inline const dVector& dMatrix::operator[] (int  i) const
{
	return (&m_front)[i];
}

dMatrix dRollMatrix(dFloat ang);
dMatrix dYawMatrix(dFloat ang);
dMatrix dPitchMatrix(dFloat ang);
dMatrix dgGrammSchmidt(const dVector& dir);
#endif

