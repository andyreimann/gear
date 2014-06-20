#include "RenderComponent.h"
#include "TriangeTools.h"

using namespace G2;

RenderComponent::RenderComponent() 
	: billboarding(false),
	drawMode(GL_TRIANGLES),
	aabbAnimationRecalc(false),
	mRenderSystem(ECSManager::getShared().getSystem<RenderSystem,RenderComponent>())
{
}

RenderComponent::RenderComponent(RenderComponent && rhs) 
	: mRenderSystem(ECSManager::getShared().getSystem<RenderSystem,RenderComponent>())
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

RenderComponent&
RenderComponent::operator=(RenderComponent && rhs) 
{
	material = std::move(rhs.material);
	mVaos = std::move(rhs.mVaos);
	mVaosFrustumCulled = std::move(rhs.mVaosFrustumCulled);
	drawMode = rhs.drawMode;
	billboarding = rhs.billboarding;
	objectSpaceAABBs = std::move(rhs.objectSpaceAABBs);
	worldSpaceAABBs = std::move(rhs.worldSpaceAABBs);
	aabbAnimationRecalc = rhs.aabbAnimationRecalc;
	mEffect = std::move(rhs.mEffect);
	mShaderCache = std::move(rhs.mShaderCache);
	mRenderSystem = rhs.mRenderSystem;
	mRenderStatesGroup = rhs.mRenderStatesGroup;
	
	rhs.mVaos.clear();
	rhs.mVaosFrustumCulled.clear();
	rhs.objectSpaceAABBs.clear();
	rhs.drawMode = GL_INVALID_VALUE;
	rhs.mRenderSystem = nullptr;
	rhs.mRenderStatesGroup = std::shared_ptr<RenderStatesGroup>();
	
	return static_cast<RenderComponent&>(BaseComponent::operator=(std::move(rhs)));
}

G2::RenderComponent::~RenderComponent() 
{
	if(material.isTransparent() && getEntityId() != Entity::UNINITIALIZED_ENTITY_ID)
	{
		auto* renderSystem = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>();
		renderSystem->updateTransparencyMode(getEntityId(), false);
	}
}

unsigned int
RenderComponent::getNumDrawCalls() const 
{
	unsigned int drawCalls = 0;
	for(size_t i = 0; i < mVaos.size(); ++i)
	{
		drawCalls += mVaos[i].getNumDrawCalls();
	}
	return drawCalls;
}

void
RenderComponent::_updateRenderStatesGroupLinkage(std::shared_ptr<RenderStatesGroup> newGroup) 
{
	if(mRenderStatesGroup.get() != nullptr)
	{
		// remove from group
		mRenderStatesGroup->erase(getEntityId());
	}
	mRenderStatesGroup = newGroup;
}
void
RenderComponent::setEffect(std::shared_ptr<G2::Effect> const& value) 
{
	mEffect = value; 
	mShaderCache = ShaderCache(); // invalidate
}

void
RenderComponent::allocateVertexArrays(unsigned int numVertexArrayObjects) 
{
	int drawCallsDifference = 0;
	if(mVaos.size() > numVertexArrayObjects)
	{
		// calculate the number of draw calls, which will disappear
		for(size_t i = numVertexArrayObjects; i < mVaos.size(); ++i)
		{
			drawCallsDifference -= mVaos[i].getNumDrawCalls();
		}
	}
	else
	{
		// we just add one draw call per newly added VertexArrayObject
		drawCallsDifference = (int)numVertexArrayObjects - (int)mVaos.size();
	}
	mVaos.resize(numVertexArrayObjects);
	mVaosFrustumCulled.resize(mVaosFrustumCulled.size()+drawCallsDifference);
	
	mRenderSystem->scheduleAABBRecalculation(getEntityId());
	mRenderSystem->_onDrawCallResize(getEntityId(),drawCallsDifference);
	material._connectToEntityId(getEntityId());
}

void
RenderComponent::allocateIndexArrays(unsigned int vertexArrayIndex, unsigned int numIndexArrays) 
{
	int drawCallsDifference = 0;
	if(mVaos[vertexArrayIndex].hasIndexBuffers())
	{
		// calculate the number of draw calls, which will be added
		drawCallsDifference = (int)numIndexArrays - (int)mVaos[vertexArrayIndex].getNumDrawCalls();
	}
	else
	{
		// this vao is already registered with one draw call from allocateVertexArrays at some point before!
		// take this into account -> -1
		drawCallsDifference = (int)numIndexArrays-1;
	}
	mVaos[vertexArrayIndex]._resizeIndexBufferCount(numIndexArrays);
	mVaosFrustumCulled.resize(mVaosFrustumCulled.size()+drawCallsDifference);

	mRenderSystem->scheduleAABBRecalculation(getEntityId());
	mRenderSystem->_onDrawCallResize(getEntityId(),drawCallsDifference);
}

void
RenderComponent::setPolygonOffsetUnits(float const& value) 
{
	RenderStates copy(mRenderStatesGroup->getRenderStates());
	copy.setPolygonOffsetUnits(value);
	mRenderSystem->_updateRenderStatesGroup(this, &copy);
}

void
RenderComponent::setPolygonOffsetFactor(float const& value) 
{
	RenderStates copy(mRenderStatesGroup->getRenderStates());
	copy.setPolygonOffsetFactor(value);
	mRenderSystem->_updateRenderStatesGroup(this, &copy);
}

void
RenderComponent::setPolygonDrawMode(PolygonDrawMode::Name const& value) 
{
	RenderStates copy(mRenderStatesGroup->getRenderStates());
	copy.setPolygonDrawMode(value);
	mRenderSystem->_updateRenderStatesGroup(this, &copy);
}

void
RenderComponent::setFaceCulling(FaceCulling::Name const& value) 
{
	RenderStates copy(mRenderStatesGroup->getRenderStates());
	copy.setFaceCulling(value);
	mRenderSystem->_updateRenderStatesGroup(this, &copy);
}

void
RenderComponent::setSourceBlendFactor(BlendFactor::Name const& value) 
{
	RenderStates copy(mRenderStatesGroup->getRenderStates());
	copy.setSourceFactor(value);
	mRenderSystem->_updateRenderStatesGroup(this, &copy);
}

void
RenderComponent::setDestinationBlendFactor(BlendFactor::Name const& value) 
{
	RenderStates copy(mRenderStatesGroup->getRenderStates());
	copy.setDestinationFactor(value);
	mRenderSystem->_updateRenderStatesGroup(this, &copy);
}

void
RenderComponent::calculateBinormalsAndTangents(Semantics::Name vertexSemantic, Semantics::Name texCoordsSemantic) 
{
	logger << "RenderComponent has " << mVaos.size() << " VAOs\n";
	for(size_t v = 0; v < mVaos.size(); ++v)
	{
		VertexArrayObject& vao = mVaos[v];
		// allocate data for binormals and tangents
		std::vector<glm::vec3> binormals(vao.getNumElements());
		std::vector<glm::vec3> tangents(vao.getNumElements());
		// get some data out of VertexArrayObject
		unsigned int vertexComponents = vao.getNumBytesBySemantic(vertexSemantic) / sizeof(float);
		unsigned int texCoordsComponents = vao.getNumBytesBySemantic(texCoordsSemantic) / sizeof(float);
		// transfer data from GPU to CPU
		float* vertices = vao.getDataPointer(vertexSemantic,BufferAccessMode::READ_ONLY);
		float* texCoords = vao.getDataPointer(texCoordsSemantic,BufferAccessMode::READ_ONLY);
		if(!vao.hasIndexBuffers())
		{ // interpret as continuous vertex data for triangles
			for(unsigned int i = 0; i < vao.getNumElements()-2; i+=2)
			{
				// get 3 vertices (vec3 also works for vec4 data)
				glm::vec3& p1 = *(glm::vec3*)(&(vertices[i*vertexComponents]));
				glm::vec3& p2 = *(glm::vec3*)(&(vertices[(i+1)*vertexComponents]));
				glm::vec3& p3 = *(glm::vec3*)(&(vertices[(i+2)*vertexComponents]));
				// get 3 texture coordinates
				glm::vec2& t1 = *(glm::vec2*)(&(vertices[i*texCoordsComponents]));
				glm::vec2& t2 = *(glm::vec2*)(&(vertices[(i+1)*texCoordsComponents]));
				glm::vec2& t3 = *(glm::vec2*)(&(vertices[(i+2)*texCoordsComponents]));
				
				//calculate for current triangle
				TriangeTools::calculateTangentAndBinormalForTriangle(i, i+1, i+2, p1, p2, p3, t1, t2, t3, binormals, tangents);
			}
		}
		else
		{ // interpret as indexed vertex data for triangles
			/********************************************************************
			 * Since a VertexArrayObject can have more than one index buffer	*
			 * but we can only calculate tangent and binormal vectors for one,	*
			 * we just calculate for the very first index buffer!				*
			 ********************************************************************/
			// transfer indices data from GPU to CPU
			unsigned int* indices = vao.getIndexPointer(0, BufferAccessMode::READ_ONLY);

			// do something
			for(unsigned int i = 0; i < vao.getNumIndices(0)-2; i+=2)
			{
				// get 3 vertices (vec3 also works for vec4 data)
				glm::vec3& p1 = *(glm::vec3*)(&(vertices[indices[i]*vertexComponents]));
				glm::vec3& p2 = *(glm::vec3*)(&(vertices[indices[i+1]*vertexComponents]));
				glm::vec3& p3 = *(glm::vec3*)(&(vertices[indices[i+2]*vertexComponents]));
				// get 3 texture coordinates
				glm::vec2& t1 = *(glm::vec2*)(&(vertices[indices[i]*texCoordsComponents]));
				glm::vec2& t2 = *(glm::vec2*)(&(vertices[indices[i+1]*texCoordsComponents]));
				glm::vec2& t3 = *(glm::vec2*)(&(vertices[indices[i+2]*texCoordsComponents]));
				
				//calculate for current triangle
				TriangeTools::calculateTangentAndBinormalForTriangle(indices[i], indices[i+1], indices[i+2], p1, p2, p3, t1, t2, t3, binormals, tangents);
			}
			// release data to GPU
			vao.returnIndexPointer(0);
		}
		// release data to GPU
		vao.returnDataPointer(texCoordsSemantic);
		vao.returnDataPointer(vertexSemantic);
		// transmit data
		vao.writeData(Semantics::BINORMAL, &binormals[0]);
		vao.writeData(Semantics::TANGENT, &tangents[0]);
	}
}