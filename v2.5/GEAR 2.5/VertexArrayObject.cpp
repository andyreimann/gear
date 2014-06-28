// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "VertexArrayObject.h"
#include "Defines.h"

using namespace G2;

VertexArrayObject::VertexArrayObject()
	: mBound(false),
	mNumElements(0),
	mMaxNumElements(0),
	mVertexArrayId(GL_INVALID_VALUE),
	mReferenceCounter(new RefCounter<int>()) 
{
	mBufferIds.fill(GL_INVALID_VALUE);
	mBytesPerSemantic.fill(0);
}

VertexArrayObject::VertexArrayObject(unsigned int numElements)
	: mBound(false),
	mNumElements(numElements),
	mMaxNumElements(numElements),
	mVertexArrayId(GL_INVALID_VALUE),
	mReferenceCounter(new RefCounter<int>())
{
	mBufferIds.fill(GL_INVALID_VALUE);
	mBytesPerSemantic.fill(0);
	writeData(Semantics::POSITION, (glm::vec4*)NULL);
}

VertexArrayObject::VertexArrayObject(VertexArrayObject const& rhs) 
{
	// eliminates redundant code
	*this = rhs;
}

VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject const& rhs)
{
	// copy
	mBound = rhs.mBound;
	mNumElements = rhs.mNumElements;
	mMaxNumElements = rhs.mMaxNumElements;
	mVertexArrayId = rhs.mVertexArrayId;
	mBufferIds = rhs.mBufferIds;
	mBytesPerSemantic = rhs.mBytesPerSemantic;
	// copy pointer to shared RefCounter - and increment
	mReferenceCounter = rhs.mReferenceCounter;
	mReferenceCounter->incr();
	// copy and increment the version!
	setVersion(rhs.getVersion()+1);
	
	return *this;
}

VertexArrayObject::VertexArrayObject(VertexArrayObject && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject && rhs) 
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
	mVertexArrayId = rhs.mVertexArrayId;
	mBufferIds = std::move(rhs.mBufferIds);
	mBytesPerSemantic = std::move(rhs.mBytesPerSemantic);
	rhs.mBufferIds.fill(GL_INVALID_VALUE);
	rhs.mBytesPerSemantic.fill(0);
	rhs.mVertexArrayId = GL_INVALID_VALUE;
	rhs.mReferenceCounter.reset();

	// copy and increment the version!
	setVersion(rhs.getVersion()+1);
	
	return *this;
}

VertexArrayObject::~VertexArrayObject()
{
	if(mReferenceCounter.get() != NULL && mReferenceCounter->decr() == 0)
	{
		_deleteBuffers();
	}
}

VertexArrayObject&
VertexArrayObject::resizeElementCount(unsigned int numElements) 
{
	if(numElements <= mMaxNumElements) 
	{
		mNumElements = numElements;
	}
	else 
	{
		mMaxNumElements = mNumElements = numElements;
		// save buffer states
		std::array<unsigned int,Semantics::NUM_SEMANTICS> tmpBufferIds = mBufferIds;
		std::array<unsigned int,Semantics::NUM_SEMANTICS> tmpBytesPerSemantic = mBytesPerSemantic;

		_deleteBuffers();
		_initVAOBuffer();
		for (unsigned int i = 0; i < Semantics::NUM_SEMANTICS; ++i) 
		{
			if(tmpBufferIds[i] != GL_INVALID_VALUE)
			{
				if(tmpBytesPerSemantic[i] == sizeof(float) * 2)
				{
					writeData(static_cast<Semantics::Name>(i), (glm::vec2*)NULL);
				}
				else if(tmpBytesPerSemantic[i] == sizeof(float) * 3)
				{
					writeData(static_cast<Semantics::Name>(i), (glm::vec3*)NULL);
				}
				else if(tmpBytesPerSemantic[i] == sizeof(float) * 4)
				{
					writeData(static_cast<Semantics::Name>(i), (glm::vec4*)NULL);
				}
			}
		}
	}
	return *this;
}

VertexArrayObject&
VertexArrayObject::writeData(Semantics::Name semantic, glm::vec2 const* data, int numElements)
{
	if(mNumElements <= 0) 
	{
		return *this;
	}
	int numElementsToTransfer = numElements == -1 ? mNumElements : numElements;
	_initVAOBuffer();
	unsigned int& bufferId = mBufferIds[semantic];
	unsigned int& bytes = mBytesPerSemantic[semantic];
	bytes = sizeof(float) * 2;
	if(bufferId == GL_INVALID_VALUE)
	{
		bind();
		GLDEBUG( glGenBuffers(1, &bufferId) );
		GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, bufferId) );
		GLDEBUG( glBufferData(GL_ARRAY_BUFFER, bytes * numElementsToTransfer, data, GL_STATIC_DRAW) );
		GLDEBUG( glVertexAttribPointer(semantic, 2, GL_FLOAT, GL_FALSE, 0, 0) );
		GLDEBUG( glEnableVertexAttribArray(semantic) );
		unbind();
		// a new semantic was added
		updateVersion();
		return *this;
	}
	GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, bufferId) );
	GLDEBUG( glBufferData(GL_ARRAY_BUFFER, bytes * numElementsToTransfer, data, GL_STATIC_DRAW) );
	GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, 0) );
	return *this;
}

VertexArrayObject&
VertexArrayObject::writeData(Semantics::Name semantic, glm::vec3 const* data, int numElements)
{
	if(mNumElements <= 0) 
	{
		return *this;
	}
	int numElementsToTransfer = numElements == -1 ? mNumElements : numElements;
	_initVAOBuffer();
	unsigned int& bufferId = mBufferIds[semantic];
	unsigned int& bytes = mBytesPerSemantic[semantic];
	bytes = sizeof(float) * 3;
	if(bufferId == GL_INVALID_VALUE)
	{
		bind();
		GLDEBUG( glGenBuffers(1, &bufferId) );
		GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, bufferId) );
		GLDEBUG( glBufferData(GL_ARRAY_BUFFER, bytes * numElementsToTransfer, data, GL_STATIC_DRAW) );
		GLDEBUG( glVertexAttribPointer(semantic, 3, GL_FLOAT, GL_FALSE, 0, 0) );
		GLDEBUG( glEnableVertexAttribArray(semantic) );
		unbind();
		// a new semantic was added
		updateVersion();
		return *this;
	}
	GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, bufferId) );
	GLDEBUG( glBufferData(GL_ARRAY_BUFFER, bytes * numElementsToTransfer, data, GL_STATIC_DRAW) );
	GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, 0) );
	return *this;
}

VertexArrayObject&
VertexArrayObject::writeData(Semantics::Name semantic, glm::vec4 const* data, int numElements)
{
	if(mNumElements <= 0) 
	{
		return *this;
	}
	int numElementsToTransfer = numElements == -1 ? mNumElements : numElements;
	_initVAOBuffer();
	unsigned int& bufferId = mBufferIds[semantic];
	unsigned int& bytes = mBytesPerSemantic[semantic];
	if(bufferId == GL_INVALID_VALUE)
	{
		bytes = sizeof(float) * 4;
		bind();
		GLDEBUG( glGenBuffers(1, &bufferId) );
		GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, bufferId) );
		GLDEBUG( glBufferData(GL_ARRAY_BUFFER, bytes * numElementsToTransfer, data, GL_STATIC_DRAW) );
		GLDEBUG( glVertexAttribPointer(semantic, 4, GL_FLOAT, GL_FALSE, 0, 0) );
		GLDEBUG( glEnableVertexAttribArray(semantic) );
		unbind();
		// a new semantic was added
		updateVersion();
		return *this;
	}
	GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, bufferId) );
	GLDEBUG( glBufferData(GL_ARRAY_BUFFER, bytes * numElementsToTransfer, data, GL_STATIC_DRAW) );
	GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, 0) );
	return *this;
}

float*
VertexArrayObject::getDataPointer(Semantics::Name semantic, G2::BufferAccessMode::Name mode) 
{
	unsigned int& bufferId = mBufferIds[semantic];
	GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, bufferId) );
	GLDEBUG( float* destination = (float*)glMapBuffer( GL_ARRAY_BUFFER, mode ) );
	GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, 0) );
	return destination;
}

void
VertexArrayObject::returnDataPointer(Semantics::Name semantic) 
{
	unsigned int& bufferId = mBufferIds[semantic];
	GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, bufferId) );
	GLDEBUG( glUnmapBuffer( GL_ARRAY_BUFFER ) );
	GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, 0) );
}

void
VertexArrayObject::bind() 
{
	if(!mBound) 
	{
		GLDEBUG( glBindVertexArray(mVertexArrayId) );
		mBound = true;
	}
}

void
VertexArrayObject::unbind() 
{
	if(mBound) 
	{
		GLDEBUG( glBindVertexArray(0) );
		mBound = false;
	}
}

void
VertexArrayObject::draw(int glDrawMode, int numVertices) 
{
	if(mBufferIds[Semantics::POSITION] != GL_INVALID_VALUE && mNumElements > 0) 
	{
		GLDEBUG( glDrawArrays(glDrawMode, 0, numVertices == -1 ? mNumElements : numVertices) );
	}
}

void
VertexArrayObject::_initVAOBuffer() 
{
	if(mVertexArrayId == GL_INVALID_VALUE) 
	{
		GLDEBUG( glGenVertexArrays(1, &mVertexArrayId) );
	}
}

void
VertexArrayObject::_deleteBuffers()
{

	// no other one holds a reference to the VAO -> delete GL resources
	for (size_t i = 0; i < mBufferIds.size(); ++i) 
	{
		if(mBufferIds[i] != GL_INVALID_VALUE) 
		{
			GLDEBUG( glDeleteBuffers(1, &mBufferIds[i]) );
			mBufferIds[i] = GL_INVALID_VALUE;
			mBytesPerSemantic[i] = 0;
		}
	}
	if(mVertexArrayId != GL_INVALID_VALUE) 
	{
		GLDEBUG( glDeleteVertexArrays(1, &mVertexArrayId) );
		mVertexArrayId = GL_INVALID_VALUE;
	}
}

unsigned int
VertexArrayObject::getNumBytesBySemantic(Semantics::Name semantic) const 
{
	return mBytesPerSemantic[semantic];
}

Semantics::Name
Semantics::getSemantic(std::string const& name) 
{
	if(name == "POSITION") 
	{
		return POSITION;
	}
	else if(name == "BLENDWEIGHT") 
	{
		return BLENDWEIGHT;
	}
	else if(name == "NORMAL") 
	{
		return NORMAL;
	}
	else if(name == "COLOR_0") 
	{
		return COLOR_0;
	}
	else if(name == "COLOR_1") 
	{
		return COLOR_1;
	}
	else if(name == "FOGCOORD") 
	{
		return FOGCOORD;
	}
	else if(name == "PSIZE") 
	{
		return PSIZE;
	}
	else if(name == "BLENDINDICES") 
	{
		return BLENDINDICES;
	}
	else if(name == "TEXCOORD_0") 
	{
		return TEXCOORD_0;
	}
	else if(name == "TEXCOORD_1") 
	{
		return TEXCOORD_1;
	}
	else if(name == "TEXCOORD_2") 
	{
		return TEXCOORD_2;
	}
	else if(name == "TEXCOORD_3") 
	{
		return TEXCOORD_3;
	}
	else if(name == "TEXCOORD_4") 
	{
		return TEXCOORD_4;
	}
	else if(name == "TEXCOORD_5") 
	{
		return TEXCOORD_5;
	}
	else if(name == "TANGENT") 
	{
		return TANGENT;
	}
	else if(name == "BINORMAL") 
	{
		return BINORMAL;
	}
	return SEMANTIC_INVALID;
}