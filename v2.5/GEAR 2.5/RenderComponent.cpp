#include "RenderComponent.h"
#include "TriangleTools.h"

using namespace G2;

RenderComponent::RenderComponent() 
	: billboarding(false),
	mRenderLayerMask(G2Core::Flags::ALL_FLAGS),
	mRenderSystem(ECSManager::getShared().getSystem<RenderSystem,RenderComponent>())
{
}

G2::RenderComponent::RenderComponent(G2Core::RenderLayer::RenderLayerMask renderLayers)
	: billboarding(false),
	mRenderLayerMask(renderLayers),
	mRenderSystem(ECSManager::getShared().getSystem<RenderSystem, RenderComponent>())
{

}

RenderComponent::RenderComponent(RenderComponent && rhs) 
	: mRenderSystem(ECSManager::getShared().getSystem<RenderSystem, RenderComponent>()),
	mRenderLayerMask(0)
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

RenderComponent&
RenderComponent::operator=(RenderComponent && rhs) 
{
	material = std::move(rhs.material);
	mVaos = std::move(rhs.mVaos);
	mIaos = std::move(rhs.mIaos);
	mDrawCalls = std::move(rhs.mDrawCalls);
	billboarding = rhs.billboarding;
	mEffect = std::move(rhs.mEffect);
	mShaderCache = std::move(rhs.mShaderCache);
	mRenderSystem = rhs.mRenderSystem;
	mRenderStatesGroup = rhs.mRenderStatesGroup;
	mRenderLayerMask = rhs.mRenderLayerMask;
	
	rhs.mVaos.clear();
	rhs.mIaos.clear();
	rhs.mDrawCalls.clear();
	rhs.mRenderSystem = nullptr;
	rhs.mRenderStatesGroup = std::shared_ptr<RenderStatesGroup>();
	rhs.mRenderLayerMask = G2Core::Flags::ALL_FLAGS;

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
G2::RenderComponent::getNumTriangles()
{
	int numTris = 0;

	for (unsigned int i = 0; i < getNumDrawCalls(); ++i)
	{
		auto& drawCall = getDrawCall(i);
		if (drawCall.getDrawMode() != G2Core::DrawMode::TRIANGLES)
		{
			continue;
		}
		if (drawCall.getIaoIndex() != -1)
		{
			auto& iao = getIndexArray(drawCall.getIaoIndex());
			numTris += iao.getNumElements() / 3;
		}
		else
		{
			auto& vao = getVertexArray(drawCall.getVaoIndex());
			numTris += vao.getNumElements() / 3;
		}
	}
	return numTris;
}
void
RenderComponent::addDrawCall(DrawCall const& drawCall) 
{
	mDrawCalls.push_back(drawCall);
	// set the internal parameters of the DrawCall to link it to this RenderComponent
	mDrawCalls.back()._setDrawCallIndex((int)mDrawCalls.size()-1);
	mDrawCalls.back()._setEntityId(getEntityId());
	scheduleAABBRecalculation();
}

unsigned int
RenderComponent::getNumDrawCalls() const 
{
	return (unsigned int)mDrawCalls.size();
}

void
RenderComponent::scheduleAABBRecalculation() const 
{
	auto* renderSystem = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>();
	renderSystem->scheduleAABBRecalculation(getEntityId());
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
	mVaos.resize(numVertexArrayObjects);
	material._connectToEntityId(getEntityId());
}

void
RenderComponent::allocateIndexArrays(unsigned int numIndexArrays) 
{
	mIaos.resize(numIndexArrays);
	material._connectToEntityId(getEntityId());
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
RenderComponent::setPolygonDrawMode(G2Core::PolygonDrawMode::Name const& value) 
{
	RenderStates copy(mRenderStatesGroup->getRenderStates());
	copy.setPolygonDrawMode(value);
	mRenderSystem->_updateRenderStatesGroup(this, &copy);
}

void
RenderComponent::setFaceCulling(G2Core::FaceCulling::Name const& value) 
{
	RenderStates copy(mRenderStatesGroup->getRenderStates());
	copy.setFaceCulling(value);
	mRenderSystem->_updateRenderStatesGroup(this, &copy);
}

void
RenderComponent::setSourceBlendFactor(G2Core::BlendFactor::Name const& value) 
{
	RenderStates copy(mRenderStatesGroup->getRenderStates());
	copy.setSourceFactor(value);
	mRenderSystem->_updateRenderStatesGroup(this, &copy);
}

void
RenderComponent::setDestinationBlendFactor(G2Core::BlendFactor::Name const& value) 
{
	RenderStates copy(mRenderStatesGroup->getRenderStates());
	copy.setDestinationFactor(value);
	mRenderSystem->_updateRenderStatesGroup(this, &copy);
}

void
RenderComponent::calculateBinormalsAndTangents(G2Core::Semantics::Name vertexSemantic, G2Core::Semantics::Name texCoordsSemantic) 
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
		float* vertices = vao.getDataPointer(vertexSemantic,G2Core::BufferAccessMode::READ_ONLY);
		float* texCoords = vao.getDataPointer(texCoordsSemantic,G2Core::BufferAccessMode::READ_ONLY);
		//if(!vao.hasIndexBuffers())
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
				TriangleTools::calculateTangentAndBinormalForTriangle(i, i+1, i+2, p1, p2, p3, t1, t2, t3, binormals, tangents);
			}
		}
		//else
		//{ // interpret as indexed vertex data for triangles
		//	/********************************************************************
		//	 * Since a VertexArrayObject can have more than one index buffer	*
		//	 * but we can only calculate tangent and binormal vectors for one,	*
		//	 * we just calculate for the very first index buffer!				*
		//	 ********************************************************************/
		//	// transfer indices data from GPU to CPU
		//	unsigned int* indices = vao.getIndexPointer(0, BufferAccessMode::READ_ONLY);

		//	// do something
		//	for(unsigned int i = 0; i < vao.getNumIndices(0)-2; i+=2)
		//	{
		//		// get 3 vertices (vec3 also works for vec4 data)
		//		glm::vec3& p1 = *(glm::vec3*)(&(vertices[indices[i]*vertexComponents]));
		//		glm::vec3& p2 = *(glm::vec3*)(&(vertices[indices[i+1]*vertexComponents]));
		//		glm::vec3& p3 = *(glm::vec3*)(&(vertices[indices[i+2]*vertexComponents]));
		//		// get 3 texture coordinates
		//		glm::vec2& t1 = *(glm::vec2*)(&(vertices[indices[i]*texCoordsComponents]));
		//		glm::vec2& t2 = *(glm::vec2*)(&(vertices[indices[i+1]*texCoordsComponents]));
		//		glm::vec2& t3 = *(glm::vec2*)(&(vertices[indices[i+2]*texCoordsComponents]));
		//		
		//		//calculate for current triangle
		//		TriangeTools::calculateTangentAndBinormalForTriangle(indices[i], indices[i+1], indices[i+2], p1, p2, p3, t1, t2, t3, binormals, tangents);
		//	}
		//	// release data to GPU
		//	vao.returnIndexPointer(0);
		//}
		// release data to GPU
		vao.returnDataPointer(texCoordsSemantic);
		vao.returnDataPointer(vertexSemantic);
		// transmit data
		vao.writeData(G2Core::Semantics::BINORMAL, &binormals[0]);
		vao.writeData(G2Core::Semantics::TANGENT, &tangents[0]);
	}
}