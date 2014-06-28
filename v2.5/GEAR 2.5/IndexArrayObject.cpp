#include "IndexArrayObject.h"
#include <utility>

using namespace G2;

IndexArrayObject::IndexArrayObject() 
	: mBound(false),
	mNumElements(0),
	mMaxNumElements(0),
	mIndexArrayId(GL_INVALID_VALUE),
	mReferenceCounter(new RefCounter<int>())
{
}

IndexArrayObject::IndexArrayObject(unsigned int numElements) 
	: mBound(false),
	mNumElements(numElements),
	mMaxNumElements(numElements),
	mIndexArrayId(GL_INVALID_VALUE),
	mReferenceCounter(new RefCounter<int>())
{
	writeIndices(nullptr, numElements);
}

IndexArrayObject::IndexArrayObject(IndexArrayObject const& rhs) 
	: mBound(false),
	mNumElements(0),
	mMaxNumElements(0),
	mIndexArrayId(GL_INVALID_VALUE),
	mReferenceCounter(new RefCounter<int>())
{
	// eliminates redundant code
	*this = rhs;
}

IndexArrayObject::IndexArrayObject(IndexArrayObject && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

IndexArrayObject&
IndexArrayObject::operator=(IndexArrayObject const& rhs) 
{
	// copy
	mBound = rhs.mBound;
	mNumElements = rhs.mNumElements;
	mMaxNumElements = rhs.mMaxNumElements;
	mIndexArrayId = rhs.mIndexArrayId;
	// copy pointer to shared RefCounter - and increment
	mReferenceCounter = rhs.mReferenceCounter;
	mReferenceCounter->incr();
	// copy and increment the version!
	setVersion(rhs.getVersion()+1);

	return *this;
}

IndexArrayObject&
IndexArrayObject::operator=(IndexArrayObject && rhs) 
{
	// 1. Stage: delete maybe allocated resources on target type
	// nothing here
	// 2. Stage: transfer data from src to target
	mBound = rhs.mBound;
	mNumElements = rhs.mNumElements;
	mMaxNumElements = rhs.mMaxNumElements;
	mReferenceCounter = std::move(rhs.mReferenceCounter);
	// 3. Stage: modify src to a well defined state
	rhs.mBound = false;
	rhs.mNumElements = 0;
	rhs.mMaxNumElements = 0;
	mIndexArrayId = rhs.mIndexArrayId;
	rhs.mIndexArrayId = GL_INVALID_VALUE;

	// copy and increment the version!
	setVersion(rhs.getVersion()+1);
	
	return *this;
}

IndexArrayObject::~IndexArrayObject() 
{
	if(mReferenceCounter.get() != NULL && mReferenceCounter->decr() == 0)
	{
		_deleteBuffer();
	}
}

IndexArrayObject&
IndexArrayObject::resizeElementCount(unsigned int numElements) 
{
	if(numElements <= mMaxNumElements) 
	{
		mNumElements = numElements;
	}
	else 
	{
		mMaxNumElements = mNumElements = numElements;

		_deleteBuffer();
		_initIAOBuffer();
		writeIndices(nullptr, numElements);
	}
	return *this;
}

IndexArrayObject&
IndexArrayObject::writeIndices(unsigned int const* data, unsigned int numIndices) 
{
	unsigned int bytes = sizeof(unsigned int);
	if(mIndexArrayId == GL_INVALID_VALUE)
	{
		mNumElements = mMaxNumElements = numIndices;
		_initIAOBuffer();
		GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexArrayId) );
		GLDEBUG( glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytes * numIndices, data, GL_STATIC_DRAW) );
		GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
		// a new index array was added
		updateVersion();
		return *this;
	}
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexArrayId) )
	GLDEBUG( unsigned int* destination = (unsigned int*)glMapBuffer( GL_ELEMENT_ARRAY_BUFFER, G2::BufferAccessMode::WRITE_ONLY ) );
	memcpy(destination, data, bytes * numIndices);
	GLDEBUG( glUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER ) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) )
	return *this;
}

void
IndexArrayObject::bind() 
{
	if(!mBound) 
	{
		GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexArrayId) );
		mBound = true;
	}
}

void
IndexArrayObject::draw(int glDrawMode, int numIndices) 
{
	if(mIndexArrayId != GL_INVALID_VALUE)
	{
		GLDEBUG( glDrawElements(glDrawMode, numIndices == -1 ? mNumElements : numIndices, GL_UNSIGNED_INT, 0) );
	}
}

void
IndexArrayObject::unbind() 
{
	if(mBound) 
	{
		GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexArrayId) );
		mBound = false;
	}
}

void
IndexArrayObject::_deleteBuffer()
{
	if(mIndexArrayId != GL_INVALID_VALUE) 
	{
		GLDEBUG( glDeleteBuffers(1, &mIndexArrayId) );
		mIndexArrayId = GL_INVALID_VALUE;
	}
}

void
IndexArrayObject::_initIAOBuffer() 
{
	if(mIndexArrayId == GL_INVALID_VALUE) 
	{
		GLDEBUG( glGenBuffers(1, &mIndexArrayId) );
	}
}

unsigned int*
IndexArrayObject::getIndexPointer(G2::BufferAccessMode::Name mode /*= BufferAccessMode::WRITE_ONLY*/) 
{
	if(mIndexArrayId == GL_INVALID_VALUE)
	{
		return nullptr;
	}
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexArrayId) );
	GLDEBUG( unsigned int* destination = (unsigned int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, mode) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
	return destination;
}

void
IndexArrayObject::returnIndexPointer() 
{
	if(mIndexArrayId == GL_INVALID_VALUE)
	{
		return;
	}
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexArrayId) );
	GLDEBUG( glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) );
	GLDEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
}