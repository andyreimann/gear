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
		ECSManager::getShared().getSystem<RenderSystem,RenderComponent>()->updateTransparencyMode(getEntityId(), false);
	}
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
	int sizeDifference = (int)numVertexArrayObjects - (int)mVaos.size();
	mVaos.resize(numVertexArrayObjects);
	mVaosFrustumCulled.resize(numVertexArrayObjects);
	auto* renderSystem = ECSManager::getShared().getSystem<RenderSystem,RenderComponent>();
	renderSystem->scheduleAABBRecalculation(getEntityId());
	renderSystem->_onVertexArrayObjectsResize(getEntityId(),sizeDifference);
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
		if(!vao.hasIndices())
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
			// transfer indices data from GPU to CPU
			unsigned int* indices = vao.getIndexPointer(BufferAccessMode::READ_ONLY);

			// do something
			for(unsigned int i = 0; i < vao.getNumIndices()-2; i+=2)
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
			vao.returnIndexPointer();
		}
		// release data to GPU
		vao.returnDataPointer(texCoordsSemantic);
		vao.returnDataPointer(vertexSemantic);
		// transmit data
		vao.writeData(Semantics::BINORMAL, &binormals[0]);
		vao.writeData(Semantics::TANGENT, &tangents[0]);
	}
}