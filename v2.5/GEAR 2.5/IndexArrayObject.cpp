#include "IndexArrayObject.h"
#include <utility>

#include <G2Core/GfxDevice.h>

using namespace G2;

IndexArrayObject::IndexArrayObject() 
	: mNumElements(0),
	mMaxNumElements(0),
	mIndexArrayResource(nullptr),
	mReferenceCounter(new RefCounter<int>())
{
}

IndexArrayObject::IndexArrayObject(unsigned int numElements) 
	: mNumElements(numElements),
	mMaxNumElements(numElements),
	mIndexArrayResource(nullptr),
	mReferenceCounter(new RefCounter<int>())
{
	writeIndices(nullptr, numElements);
}

IndexArrayObject::IndexArrayObject(IndexArrayObject const& rhs) 
	: mNumElements(0),
	mMaxNumElements(0),
	mIndexArrayResource(nullptr),
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
	mNumElements = rhs.mNumElements;
	mMaxNumElements = rhs.mMaxNumElements;
	mIndexArrayResource = rhs.mIndexArrayResource;
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
	mIndexArrayResource = rhs.mIndexArrayResource;
	mNumElements = rhs.mNumElements;
	mMaxNumElements = rhs.mMaxNumElements;
	mReferenceCounter = std::move(rhs.mReferenceCounter);
	// 3. Stage: modify src to a well defined state
	rhs.mNumElements = 0;
	rhs.mMaxNumElements = 0;
	rhs.mIndexArrayResource = nullptr;

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
		_initIBOBuffer();
		writeIndices(nullptr, numElements);
	}
	return *this;
}

IndexArrayObject&
IndexArrayObject::writeIndices(unsigned int const* data, unsigned int numIndices) 
{
	unsigned int bytes = sizeof(unsigned int);
	if(mIndexArrayResource == nullptr)
	{
		mNumElements = mMaxNumElements = numIndices;
		_initIBOBuffer();
		// a new index array was added
		updateVersion();
	}
	G2_gfxDevice()->updateIBOIndices(mIndexArrayResource, data, mNumElements);
	return *this;
}

void
IndexArrayObject::bind() 
{
	G2_gfxDevice()->bindIBO(mIndexArrayResource);
}

void
IndexArrayObject::draw(G2Core::DrawMode::Name drawMode, int numIndices) 
{
	G2_gfxDevice()->drawIBO(mIndexArrayResource, drawMode, numIndices == -1 ? mNumElements : numIndices);
}

void
IndexArrayObject::unbind() 
{
	G2_gfxDevice()->unbindIBO(mIndexArrayResource);
}

void
IndexArrayObject::_deleteBuffer()
{
	if(mIndexArrayResource != nullptr) 
	{
		G2_gfxDevice()->freeGfxResource(mIndexArrayResource);
	}
}

void
IndexArrayObject::_initIBOBuffer() 
{
	if(mIndexArrayResource == nullptr) 
	{
		mIndexArrayResource = G2_gfxDevice()->createIBO();
	}
}

unsigned int*
IndexArrayObject::getIndexPointer(G2Core::BufferAccessMode::Name mode /*= BufferAccessMode::WRITE_ONLY*/) 
{
	return G2_gfxDevice()->getIBODataPointer(mIndexArrayResource,mode);
}

void
IndexArrayObject::returnIndexPointer() 
{
	G2_gfxDevice()->returnIBODataPointer(mIndexArrayResource);
}