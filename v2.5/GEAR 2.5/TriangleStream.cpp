#include "TriangleStream.h"

using namespace G2;

/** DrawMode	-> TRIANGLES
*	DataType	-> vec3
*  iao exists	-> no
*/
void _streamTriangleDrawModeTrianglesVec3(
	unsigned int* indices,
	float* vertexStream,
	unsigned int triangle,
	glm::vec3** p1,
	glm::vec3** p2,
	glm::vec3** p3)
{
	glm::vec3* vertices = (glm::vec3*)vertexStream;
	*p1 = &vertices[triangle * 3 + 0];
	*p2 = &vertices[triangle * 3 + 1];
	*p3 = &vertices[triangle * 3 + 2];
}
/** DrawMode	-> TRIANGLES
*	DataType	-> vec3
*   iao exists	-> yes
*/
void _streamTriangleDrawModeTrianglesVec3Iao(
	unsigned int* indices,
	float* vertexStream,
	unsigned int triangle,
	glm::vec3** p1,
	glm::vec3** p2,
	glm::vec3** p3)
{
	glm::vec3* vertices = (glm::vec3*)vertexStream;
	*p1 = &vertices[indices[triangle * 3 + 0]];
	*p2 = &vertices[indices[triangle * 3 + 1]];
	*p3 = &vertices[indices[triangle * 3 + 2]];
}

/** DrawMode	-> TRIANGLES
*	DataType	-> vec3
*  iao exists	-> no
*/
void _streamTriangleDrawModeTrianglesVec4(
	unsigned int* indices,
	float* vertexStream,
	unsigned int triangle,
	glm::vec3** p1,
	glm::vec3** p2,
	glm::vec3** p3)
{
	glm::vec4* vertices = (glm::vec4*)vertexStream;
	*p1 = ((glm::vec3*)(&(vertices[triangle * 3 + 0])));
	*p2 = ((glm::vec3*)(&(vertices[triangle * 3 + 1])));
	*p3 = ((glm::vec3*)(&(vertices[triangle * 3 + 2])));
}
/** DrawMode	-> TRIANGLES
*	DataType	-> vec4
*   iao exists	-> yes
*/
void _streamTriangleDrawModeTrianglesVec4Iao(
	unsigned int* indices,
	float* vertexStream,
	unsigned int triangle,
	glm::vec3** p1,
	glm::vec3** p2,
	glm::vec3** p3)
{
	glm::vec3* vertices = (glm::vec3*)vertexStream;
	*p1 = ((glm::vec3*)(&(vertices[indices[triangle * 3 + 0]])));
	*p2 = ((glm::vec3*)(&(vertices[indices[triangle * 3 + 1]])));
	*p3 = ((glm::vec3*)(&(vertices[indices[triangle * 3 + 2]])));
}

TriangleStream::TriangleStream(
	G2Core::Semantics::Name semantic,
	G2::IndexArrayObject* iao,
	G2::VertexArrayObject* vao,
	G2Core::DrawMode::Name drawMode) :
	mNumTriangles(0),
	mStreamedTriangles(0),
	mIndexStream(nullptr),
	mVertexStream(nullptr),
	mIndexArrayObject(iao),
	mVertexArrayObject(vao),
	mDrawMode(drawMode),
	mSemantic(semantic),
	_streamTriangle(nullptr)
{
	assert(mVertexArrayObject != nullptr);

	switch (drawMode)
	{
	case G2Core::DrawMode::TRIANGLES:
	{
		if(mVertexArrayObject->getNumBytesBySemantic(mSemantic) == sizeof(float) * 3)
		{
			mVertexStream = mVertexArrayObject->getDataPointer(mSemantic, G2Core::BufferAccessMode::READ_ONLY);
			if (mIndexArrayObject != nullptr)
			{
				mIndexStream = mIndexArrayObject->getIndexPointer(G2Core::BufferAccessMode::READ_ONLY);
				mNumTriangles = mIndexArrayObject->getNumElements() / 3; // overrides
				_streamTriangle = _streamTriangleDrawModeTrianglesVec3Iao;
			}
			else
			{
				mNumTriangles = mVertexArrayObject->getNumElements() / 3;
				_streamTriangle = _streamTriangleDrawModeTrianglesVec3;
			}
		}
		else if (mVertexArrayObject->getNumBytesBySemantic(mSemantic) == sizeof(float) * 4)
		{
			mVertexStream = mVertexArrayObject->getDataPointer(mSemantic, G2Core::BufferAccessMode::READ_ONLY);
			if (mIndexArrayObject != nullptr)
			{
				mIndexStream = mIndexArrayObject->getIndexPointer(G2Core::BufferAccessMode::READ_ONLY);
				mNumTriangles = mIndexArrayObject->getNumElements() / 3; // overrides
				_streamTriangle = _streamTriangleDrawModeTrianglesVec4Iao;
			}
			else
			{
				mNumTriangles = mVertexArrayObject->getNumElements() / 3;
				_streamTriangle = _streamTriangleDrawModeTrianglesVec4;
			}
		}
		break;
	}
	case G2Core::DrawMode::TRIANGLE_STRIP:
		// Not implemented
		assert(false);
		break;
	case G2Core::DrawMode::TRIANGLE_FAN:
		// Not implemented
		assert(false);
		break;
	case G2Core::DrawMode::QUADS:
		assert(false);
		// Not implemented
		break;
	case G2Core::DrawMode::LINES:
	case G2Core::DrawMode::LINE_LOOP:
	case G2Core::DrawMode::LINE_STRIP:
	case G2Core::DrawMode::POINTS:
		// Not supported
		mIndexArrayObject = nullptr;
		mVertexArrayObject = nullptr;
		break;
	default:
		break;
	}
}



TriangleStream::~TriangleStream() 
{
	if (mIndexArrayObject != nullptr)
	{
		mIndexArrayObject->returnIndexPointer();
	}
	if (mVertexArrayObject != nullptr)
	{
		mVertexArrayObject->returnDataPointer(mSemantic);
	}
}

bool
G2::TriangleStream::hasNext()
{
	return mStreamedTriangles < mNumTriangles;
}

void
G2::TriangleStream::next(glm::vec3** p1, glm::vec3** p2, glm::vec3** p3)
{
	// Check if somebody calls this function without a streaming function
	assert(_streamTriangle != nullptr);
	// fetching a triangle is actually only a function pointer call
	_streamTriangle(mIndexStream, mVertexStream, mStreamedTriangles++, p1, p2, p3);
}