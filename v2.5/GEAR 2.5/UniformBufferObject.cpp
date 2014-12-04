#include "UniformBufferObject.h"

#include <G2Core/GfxDevice.h>

using namespace G2;

UniformBufferObject::UniformBufferObject(std::string const& shadingLanguage, unsigned int byteSize, void* data, G2Core::BufferUsage::Name bufferUsage, G2Core::UniformBufferBindingPoint::Name uniformBufferBindingPoint)
	: mNumBytes(byteSize),
	mBufferUsage(bufferUsage),
	mUniformBufferResource(nullptr)
{
	mUniformBufferResource = G2_gfxDevice()->createUBO(shadingLanguage, byteSize, data, mBufferUsage);

	G2_gfxDevice()->bindUBO(mUniformBufferResource);
	// bind it to the default binding point of it
	G2_gfxDevice()->setUBOBindingPoint(mUniformBufferResource, uniformBufferBindingPoint);
}

UniformBufferObject::UniformBufferObject(UniformBufferObject && rhs)
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

UniformBufferObject&
UniformBufferObject::operator=(UniformBufferObject && rhs)
{
	// 1. Stage: delete maybe allocated resources on target type
	// nothing here
	// 2. Stage: transfer data from src to target
	mNumBytes = rhs.mNumBytes;
	mBufferUsage = rhs.mBufferUsage;
	mUniformBufferResource = rhs.mUniformBufferResource;
	// 3. Stage: modify src to a well defined state
	rhs.mNumBytes = 0;
	rhs.mUniformBufferResource = nullptr;
	
	return *this;
}

UniformBufferObject::~UniformBufferObject()
{
	if (mUniformBufferResource != nullptr)
	{
		G2_gfxDevice()->freeGfxResource(mUniformBufferResource);
	}
}

void
UniformBufferObject::bind()
{
	G2_gfxDevice()->bindUBO(mUniformBufferResource);
}

void
UniformBufferObject::updateSubData(unsigned int byteOffset, unsigned int byteSize, void* data)
{
	G2_gfxDevice()->updateUBOSubData(mUniformBufferResource, byteOffset, byteSize, data);
}

void
UniformBufferObject::unbind()
{
	G2_gfxDevice()->unbindUBO(mUniformBufferResource);
}