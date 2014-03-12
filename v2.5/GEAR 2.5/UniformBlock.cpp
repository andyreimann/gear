//// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
//// (c) 2014 GEAR 2.5
//#include "UniformBlock.h"
//#include "Defines.h"
//#include "Shader.h"
//
//using namespace G2;
//
//UniformBlock::UniformBlock(ShaderOld* shader, std::string const& blockName) 
//	: mBlockName(blockName),
//	mBlockIndex(GL_INVALID_INDEX),
//	mBlockBuffer(GL_INVALID_INDEX),
//	mBindingPoint(1) /* SHOULD BE CALCULATED BY SHADER */
//{
//	// the binding point must be smaller than GL_MAX_UNIFORM_BUFFER_BINDINGS -> should be managed by an own class
//	float myFloats[8] = {1.0, 0.0, 0.0, 1.0,   0.4, 0.0, 0.0, 1.0};
// 
//	mBlockIndex = glGetUniformBlockIndex(shader->mShaderId, blockName.c_str()); // may return INVALID_INDEX if not found!
//	// bind block to an own binding point - same as uniform buffer later
//	glUniformBlockBinding(shader->mShaderId, mBlockIndex, mBindingPoint);
//	// generate new uniform buffer 
//	glGenBuffers(1, &mBlockBuffer);
//	glBindBuffer(GL_UNIFORM_BUFFER, mBlockBuffer);
//	// send data
//	glBufferData(GL_UNIFORM_BUFFER, sizeof(myFloats), myFloats, GL_DYNAMIC_DRAW);
//	glBindBufferBase(GL_UNIFORM_BUFFER, mBindingPoint, mBlockBuffer);
//	// bind buffer to same binding point here
//}
//
//UniformBlock::UniformBlock(UniformBlock const& rhs)
//{
//	// do copy here
//}
//
//UniformBlock&
//UniformBlock::operator=(UniformBlock const& rhs)
//{
//	// do assignment here
//	mBlockName = rhs.mBlockName;
//	mBlockIndex = rhs.mBlockIndex;
//	mBlockBuffer = rhs.mBlockBuffer;
//	mBindingPoint = rhs.mBindingPoint;
//	return *this;
//}
//
//UniformBlock::~UniformBlock()
//{
//}
