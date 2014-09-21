// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "VertexArrayObject.h"
#include "Defines.h"
#include "Shader.h"

#include <G2Core/GfxDevice.h>

using namespace G2;

VertexArrayObject::VertexArrayObject()
	: mNumElements(0),
	mMaxNumElements(0),
	mReferenceCounter(new RefCounter<int>()),
	mVertexArrayResource(nullptr)
{
	mBytesPerSemantic.fill(0);
}

VertexArrayObject::VertexArrayObject(unsigned int numElements)
	: mNumElements(numElements),
	mMaxNumElements(numElements),
	mReferenceCounter(new RefCounter<int>()),
	mVertexArrayResource(nullptr)
{
	mBytesPerSemantic.fill(0);
	writeData(G2Core::Semantics::POSITION, (glm::vec4*)NULL);
}

VertexArrayObject::VertexArrayObject(VertexArrayObject const& rhs) 
{
	// eliminates redundant code
	*this = rhs;
}

VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject const& rhs)
{
	// copy
	mNumElements = rhs.mNumElements;
	mMaxNumElements = rhs.mMaxNumElements;
	mVertexArrayResource = rhs.mVertexArrayResource;
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
	mNumElements = rhs.mNumElements;
	mMaxNumElements = rhs.mMaxNumElements;
	mReferenceCounter = std::move(rhs.mReferenceCounter);
	// 3. Stage: modify src to a well defined state
	rhs.mNumElements = 0;
	rhs.mMaxNumElements = 0;
	mVertexArrayResource = rhs.mVertexArrayResource;
	mBytesPerSemantic = std::move(rhs.mBytesPerSemantic);
	rhs.mBytesPerSemantic.fill(0);
	rhs.mVertexArrayResource = nullptr;
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
		std::array<unsigned int,G2Core::Semantics::NUM_SEMANTICS> tmpBytesPerSemantic = mBytesPerSemantic;

		_deleteBuffers();
		_initVAOBuffer();
		for (unsigned int i = 0; i < G2Core::Semantics::NUM_SEMANTICS; ++i) 
		{
			if(tmpBytesPerSemantic[i] > 0)
			{
				if(tmpBytesPerSemantic[i] == sizeof(float) * 2)
				{
					writeData(static_cast<G2Core::Semantics::Name>(i), (glm::vec2*)NULL);
				}
				else if(tmpBytesPerSemantic[i] == sizeof(float) * 3)
				{
					writeData(static_cast<G2Core::Semantics::Name>(i), (glm::vec3*)NULL);
				}
				else if(tmpBytesPerSemantic[i] == sizeof(float) * 4)
				{
					writeData(static_cast<G2Core::Semantics::Name>(i), (glm::vec4*)NULL);
				}
			}
		}
	}
	return *this;
}

VertexArrayObject&
VertexArrayObject::writeData(G2Core::Semantics::Name semantic, glm::vec2 const* data, int numElements)
{
	if(mNumElements <= 0) 
	{
		return *this;
	}
	int numElementsToTransfer = numElements == -1 ? mNumElements : numElements;
	assert(numElementsToTransfer <= (int)mMaxNumElements);
	_initVAOBuffer();
	if(mBytesPerSemantic[semantic] == 0)
	{
		bind(std::shared_ptr<G2::Shader>());
		G2_gfxDevice()->updateVAOVertexBufferVec2(mVertexArrayResource, semantic, data, numElementsToTransfer);
		unbind();
		// a new semantic was added
		updateVersion();
		return *this;
	}
	G2_gfxDevice()->updateVAOVertexBufferVec2(mVertexArrayResource, semantic, data, numElementsToTransfer);
	mBytesPerSemantic[semantic] = sizeof(float) * 2;
	return *this;
}

VertexArrayObject&
VertexArrayObject::writeData(G2Core::Semantics::Name semantic, glm::vec3 const* data, int numElements)
{
	if(mNumElements <= 0) 
	{
		return *this;
	}
	int numElementsToTransfer = numElements == -1 ? mNumElements : numElements;
	assert(numElementsToTransfer <= (int)mMaxNumElements);
	_initVAOBuffer();
	if(mBytesPerSemantic[semantic] == 0)
	{
		bind(std::shared_ptr<G2::Shader>());
		G2_gfxDevice()->updateVAOVertexBufferVec3(mVertexArrayResource, semantic, data, numElementsToTransfer);
		unbind();
		// a new semantic was added
		updateVersion();
		return *this;
	}
	G2_gfxDevice()->updateVAOVertexBufferVec3(mVertexArrayResource, semantic, data, numElementsToTransfer);
	mBytesPerSemantic[semantic] = sizeof(float) * 3;
	return *this;
}

VertexArrayObject&
VertexArrayObject::writeData(G2Core::Semantics::Name semantic, glm::vec4 const* data, int numElements)
{
	if(mNumElements <= 0) 
	{
		return *this;
	}
	int numElementsToTransfer = numElements == -1 ? mNumElements : numElements;
	assert(numElementsToTransfer <= (int)mMaxNumElements);
	_initVAOBuffer();
	if(mBytesPerSemantic[semantic] == 0)
	{
		bind(std::shared_ptr<G2::Shader>());
		G2_gfxDevice()->updateVAOVertexBufferVec4(mVertexArrayResource, semantic, data, numElementsToTransfer);
		unbind();
		// a new semantic was added
		updateVersion();
		return *this;
	}
	G2_gfxDevice()->updateVAOVertexBufferVec4(mVertexArrayResource, semantic, data, numElementsToTransfer);
	mBytesPerSemantic[semantic] = sizeof(float) * 4;
	return *this;
}

float*
VertexArrayObject::getDataPointer(G2Core::Semantics::Name semantic, G2Core::BufferAccessMode::Name mode) 
{
	return (float*)G2_gfxDevice()->getVAODataPointer(mVertexArrayResource, semantic, mode);
}

void
VertexArrayObject::returnDataPointer(G2Core::Semantics::Name semantic) 
{
	G2_gfxDevice()->returnVAODataPointer(mVertexArrayResource, semantic);
}

void
VertexArrayObject::bind(std::shared_ptr<G2::Shader> boundShader) 
{
	G2_gfxDevice()->bindVAO(mVertexArrayResource, boundShader.get() == nullptr ? nullptr : boundShader->mGfxHandle);
}

void
VertexArrayObject::unbind() 
{
	G2_gfxDevice()->unbindVAO(mVertexArrayResource);
}

void
VertexArrayObject::draw(G2Core::DrawMode::Name drawMode, int numVertices) 
{
	G2_gfxDevice()->drawVAO(mVertexArrayResource, drawMode, numVertices == -1 ? mNumElements : numVertices);
}

void
VertexArrayObject::_initVAOBuffer() 
{
	if(mVertexArrayResource == nullptr) 
	{
		mVertexArrayResource = G2_gfxDevice()->createVAO();
	}
}

void
VertexArrayObject::_deleteBuffers()
{
	G2_gfxDevice()->freeGfxResource(mVertexArrayResource);
	mVertexArrayResource = nullptr;
}

unsigned int
VertexArrayObject::getNumBytesBySemantic(G2Core::Semantics::Name semantic) const 
{
	return mBytesPerSemantic[semantic];
}