#include "VertexStream.h"
#include <utility>

using namespace G2;

VertexStream::VertexStream(int num, int componentsPerVertex) 
	: mBytesPerVertex(componentsPerVertex*sizeof(float)),
	mWriteIndex(0)
{
	mVertexStream.resize(num*componentsPerVertex);
}

VertexStream::VertexStream(VertexStream && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

VertexStream&
VertexStream::operator=(VertexStream && rhs) 
{
	// 1. Stage: delete maybe allocated resources on target type
	// 2. Stage: transfer data from src to target
	mBytesPerVertex = rhs.mBytesPerVertex;
	mVertexStream = std::move(rhs.mVertexStream);
	mWriteIndex = rhs.mWriteIndex;
	// 3. Stage: modify src to a well defined state
	rhs.mBytesPerVertex = 0;
	rhs.mVertexStream.clear();
	rhs.mWriteIndex = 0;

	return *this;
}

void
VertexStream::write(VertexArrayObject& vao, Semantics::Name semantic) 
{
	if(vao.getNumBytesBySemantic(semantic) == mBytesPerVertex && mBytesPerVertex != 0)
	{
		unsigned int numComponentsPerVertex = mBytesPerVertex / sizeof(float);
		unsigned int numComponentsToWrite = std::min<unsigned int>(vao.getNumElements() * numComponentsPerVertex, (unsigned int)mVertexStream.size()-mWriteIndex);
		if(numComponentsToWrite > 0)
		{
			float* data = vao.getDataPointer(semantic, BufferAccessMode::READ_ONLY);
			memcpy(&mVertexStream[mWriteIndex], data, numComponentsToWrite*sizeof(float));
			vao.returnDataPointer(semantic);
		}
		mWriteIndex += numComponentsToWrite;
	}
	else if(mBytesPerVertex < vao.getNumBytesBySemantic(semantic))
	{
		unsigned int numComponentsPerVertex = mBytesPerVertex / sizeof(float);
		float* data = vao.getDataPointer(semantic, BufferAccessMode::READ_ONLY);
		for(unsigned int i = 0; i < vao.getNumElements(); ++i)
		{
			if(mWriteIndex+numComponentsPerVertex > mVertexStream.size())
			{
				break;
			}
			glm::vec4 pt = ((glm::vec4*)data)[i];
			memcpy(&mVertexStream[mWriteIndex], &pt, mBytesPerVertex);
			mWriteIndex += numComponentsPerVertex;
		}
		vao.returnDataPointer(semantic);
	}
}

void
VertexStream::writeTriangles(VertexArrayObject& vao) 
{
	if(vao.getNumBytesBySemantic(Semantics::POSITION) == mBytesPerVertex && mBytesPerVertex != 0)
	{
		unsigned int numComponentsPerVertex = mBytesPerVertex / sizeof(float);
		unsigned int numComponentsToWrite = std::min<unsigned int>(vao.getNumElements() * numComponentsPerVertex, (unsigned int)mVertexStream.size()-mWriteIndex);
		if(numComponentsToWrite > 0)
		{
			float* data = vao.getDataPointer(Semantics::POSITION, BufferAccessMode::READ_ONLY);
			memcpy(&mVertexStream[mWriteIndex], data, numComponentsToWrite*sizeof(float));
			vao.returnDataPointer(Semantics::POSITION);
		}
		mWriteIndex += numComponentsToWrite;
	}
	else if(mBytesPerVertex < vao.getNumBytesBySemantic(Semantics::POSITION))
	{
		unsigned int numComponentsPerVertex = mBytesPerVertex / sizeof(float);
		if(vao.hasIndices())
		{
			unsigned int* indices = vao.getIndexPointer(BufferAccessMode::READ_ONLY);
			float* data = vao.getDataPointer(Semantics::POSITION, BufferAccessMode::READ_ONLY);
			for(unsigned int i = 0; i < vao.getNumIndices(); ++i)
			{
				if(mWriteIndex+numComponentsPerVertex > mVertexStream.size())
				{
					break;
				}
				glm::vec4 pt = ((glm::vec4*)data)[i];
				memcpy(&mVertexStream[mWriteIndex], &pt, mBytesPerVertex);
				mWriteIndex += numComponentsPerVertex;
			}
			vao.returnDataPointer(Semantics::POSITION);
			vao.returnIndexPointer();
		}
		else
		{
			float* data = vao.getDataPointer(Semantics::POSITION, BufferAccessMode::READ_ONLY);
			for(unsigned int i = 0; i < vao.getNumElements(); ++i)
			{
				if(mWriteIndex+numComponentsPerVertex > mVertexStream.size())
				{
					break;
				}
				glm::vec4 pt = ((glm::vec4*)data)[i];
				memcpy(&mVertexStream[mWriteIndex], &pt, mBytesPerVertex);
				mWriteIndex += numComponentsPerVertex;
			}
			vao.returnDataPointer(Semantics::POSITION);
		}
	}
}

void
VertexStream::writeTriangles(VertexArrayObject& vao, glm::mat4 const& transformation) 
{
	if(vao.getNumBytesBySemantic(Semantics::POSITION) == mBytesPerVertex && mBytesPerVertex != 0)
	{
		unsigned int numComponentsPerVertex = mBytesPerVertex / sizeof(float);
		unsigned int numComponentsToWrite = std::min<unsigned int>(vao.getNumElements() * numComponentsPerVertex, (unsigned int)mVertexStream.size()-mWriteIndex);
		if(numComponentsToWrite > 0)
		{
			float* data = vao.getDataPointer(Semantics::POSITION, BufferAccessMode::READ_ONLY);
			for(unsigned int i = 0; i < vao.getNumElements(); ++i)
			{
				if(mWriteIndex+numComponentsPerVertex > mVertexStream.size())
				{
					break;
				}
				glm::vec3 tmp = ((glm::vec3*)data)[i];
				glm::vec4 pt = transformation * glm::vec4(tmp.x, tmp.y,tmp.z,1.f);

				memcpy(&mVertexStream[mWriteIndex], &pt, mBytesPerVertex);
				mWriteIndex += numComponentsPerVertex;
			}
			vao.returnDataPointer(Semantics::POSITION);
		}
		mWriteIndex += numComponentsToWrite;
	}
	else if(mBytesPerVertex < vao.getNumBytesBySemantic(Semantics::POSITION))
	{
		unsigned int numComponentsPerVertex = mBytesPerVertex / sizeof(float);
		if(vao.hasIndices())
		{
			unsigned int* indices = vao.getIndexPointer(BufferAccessMode::READ_ONLY);
			float* data = vao.getDataPointer(Semantics::POSITION, BufferAccessMode::READ_ONLY);
			for(unsigned int i = 0; i < vao.getNumIndices(); ++i)
			{
				if(mWriteIndex+numComponentsPerVertex > mVertexStream.size())
				{
					break;
				}
				glm::vec4 pt = transformation * glm::vec4(((glm::vec4*)data)[i]);
				memcpy(&mVertexStream[mWriteIndex], &pt, mBytesPerVertex);
				mWriteIndex += numComponentsPerVertex;
			}
			vao.returnDataPointer(Semantics::POSITION);
			vao.returnIndexPointer();
		}
		else
		{
			float* data = vao.getDataPointer(Semantics::POSITION, BufferAccessMode::READ_ONLY);
			for(unsigned int i = 0; i < vao.getNumElements(); ++i)
			{
				if(mWriteIndex+numComponentsPerVertex > mVertexStream.size())
				{
					break;
				}
				glm::vec4 pt = transformation * glm::vec4(((glm::vec4*)data)[i]);

				memcpy(&mVertexStream[mWriteIndex], &pt, mBytesPerVertex);
				mWriteIndex += numComponentsPerVertex;
			}
			vao.returnDataPointer(Semantics::POSITION);
		}
	}
}