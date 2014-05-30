//// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
//// (c) 2014 GEAR 2.5
//#include "VertexBuffer.h"
//#include <stdlib.h>
//#include <assert.h>
//
//using namespace G2;
//
//
//VertexBuffer::VertexBuffer(int numVertices, glm::vec4* vertices, glm::vec4* colors) 
//	: mNumVertices(numVertices),
//	mNumFloats(numVertices*4),
//	mVao(0),
//	mVbo(0),
//	mCbo(0),
//	mNbo(0),
//	mBufferData(NULL)
//{
//	
//	GLDEBUG( glGenVertexArrays(1, &mVao) );
//    GLDEBUG( glBindVertexArray(mVao) );
//
//    GLDEBUG( glGenBuffers(1, &mVbo) );
//    GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, mVbo) );
//    GLDEBUG( glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mNumFloats, vertices, GL_STATIC_DRAW) );
//    GLDEBUG( glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0) );
//    GLDEBUG( glEnableVertexAttribArray(0) );
//
//    GLDEBUG( glGenBuffers(1, &mCbo) );
//    GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, mCbo) );
//    GLDEBUG( glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mNumFloats, colors, GL_STATIC_DRAW) );
//    GLDEBUG( glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0) );
//    GLDEBUG( glEnableVertexAttribArray(1) );
//	/*
//    GLDEBUG( glGenBuffers(1, &mNbo) );
//    GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, mNbo) );
//    GLDEBUG( glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mNumFloats, colors, GL_STATIC_DRAW) );
//    GLDEBUG( glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0) );
//    GLDEBUG( glEnableVertexAttribArray(1) );
//	*/
//}
//
//VertexBuffer::VertexBuffer(int numVertices, glm::vec4* vertices, glm::vec4* colors, glm::vec3* normals) 
//	: mNumVertices(numVertices),
//	mNumFloats(numVertices*4),
//	mVao(0),
//	mVbo(0),
//	mCbo(0),
//	mNbo(0),
//	mBufferData(NULL)
//{
//	GLDEBUG( glGenVertexArrays(1, &mVao) );
//    GLDEBUG( glBindVertexArray(mVao) );
//
//    GLDEBUG( glGenBuffers(1, &mVbo) );
//    GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, mVbo) );
//    GLDEBUG( glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mNumFloats, vertices, GL_STATIC_DRAW) );
//    GLDEBUG( glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0) );
//    GLDEBUG( glEnableVertexAttribArray(0) );
//
//    GLDEBUG( glGenBuffers(1, &mCbo) );
//    GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, mCbo) );
//    GLDEBUG( glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mNumFloats, colors, GL_STATIC_DRAW) );
//    GLDEBUG( glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0) );
//    GLDEBUG( glEnableVertexAttribArray(1) );
//	
//    GLDEBUG( glGenBuffers(1, &mNbo) );
//    GLDEBUG( glBindBuffer(GL_ARRAY_BUFFER, mNbo) );
//    GLDEBUG( glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mNumVertices * 3, normals, GL_STATIC_DRAW) );
//    GLDEBUG( glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0) );
//    GLDEBUG( glEnableVertexAttribArray(2) );
//}
//
//void
//VertexBuffer::bind()
//{
//	GLDEBUG( glBindVertexArray(mVao) );
//}
//
//void
//VertexBuffer::draw(int glDrawMode) 
//{
//	GLDEBUG( glDrawArrays(glDrawMode, 0, mNumVertices) );
//}
//
//void
//VertexBuffer::unbind() 
//{
//	GLDEBUG( glBindVertexArray(0) );
//}
//
//void
//VertexBuffer::lock() 
//{
//	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
//	/*
//	GLDEBUG( data = (float*)glMapBuffer( GL_ARRAY_BUFFER, G2::BufferAccessMode::WRITE_ONLY ) );
//	data[0] = -0.6f;
//	GLDEBUG( glUnmapBuffer( GL_ARRAY_BUFFER ) );
//	// try to modify color data in vertex array
//	glBindBuffer(GL_ARRAY_BUFFER, mCbo);
//	GLDEBUG( data = (float*)glMapBuffer( GL_ARRAY_BUFFER, G2::BufferAccessMode::WRITE_ONLY ) );
//	data[1] = 1.0f;
//	GLDEBUG( glUnmapBuffer( GL_ARRAY_BUFFER ) );
//	*/
//
//	GLDEBUG( mBufferData = (float*)glMapBuffer( GL_ARRAY_BUFFER, G2::BufferAccessMode::WRITE_ONLY ) );
//}
//
//float* 
//VertexBuffer::getDataPointer()
//{
//	return mBufferData;
//}
//
//void
//VertexBuffer::unlock()
//{
//	GLDEBUG( glUnmapBuffer( GL_ARRAY_BUFFER ) );
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//}
//
//VertexBuffer::~VertexBuffer(void)
//{
//	// correctly release memory of GL!
//}
