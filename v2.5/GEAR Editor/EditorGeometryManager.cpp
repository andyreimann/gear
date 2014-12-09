#include "EditorGeometryManager.h"
#include "EditorComponent.h"
#include "Editor.h"

#include <G2/RenderComponent.h>
#include <G2/CameraComponent.h>
#include <G2/TransformComponent.h>

#include <G2Core/EventDistributer.h>

using namespace G2::Editor;


EditorGeometryManager::EditorGeometryManager(RootEditor* editor) 
	: mEditor(editor),
	mEditorGrid(EditorSystem::createEditorEntity())
{
	mRenderSystem = G2::ECSManager::getShared().getSystem<G2::RenderSystem,G2::RenderComponent>();
	mCameraSystem = G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>();
	mTransformSystem = G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>();
	mEditorSystem = G2::ECSManager::getShared().getSystem<EditorSystem, EditorComponent>();
	
	G2::EventDistributer::onRenderFrame.hook(this, &EditorGeometryManager::_onRenderFrame);
	
	mEditor->getUI()->onComponentSelectionChanged.hook(this, &EditorGeometryManager::_setAsSelected);

	mSolidFx = mEffectImporter.import(mEditor->getEditorAssetsFolder() + "shader/Solid.g2fx");

	_initEditorGrid();

	setEditorGeometryVisibility(false);
}

EditorGeometryManager::~EditorGeometryManager() 
{
	G2::EventDistributer::onRenderFrame.unHookAll(this);
}

void
EditorGeometryManager::_setAsSelected(unsigned int entityId) 
{
	auto* renderComp = mRenderSystem->get(entityId);
	if(renderComp == nullptr)
	{
		return;
	}
	auto* transformComp = mTransformSystem->get(entityId);

	mSelectedEntityHighlight = EditorSystem::createEditorEntity();
	// connect to transformation of model
	if (transformComp)
	{
		auto* aabbVisTransform = mSelectedEntityHighlight.addComponent<G2::TransformComponent>();
		aabbVisTransform->setParent(transformComp);
	}

	// visualize model space AABBs
	auto* aabbVis = mSelectedEntityHighlight.addComponent<G2::RenderComponent>(G2Core::RenderLayer::LAYER_30);
	aabbVis->allocateVertexArrays((unsigned int)renderComp->getNumDrawCalls());
	//aabbVis->drawMode = GL_LINES;
	aabbVis->material.setAmbient(glm::vec4(0.f,1.f,0.f,1.f));

	for(unsigned int i = 0; i < renderComp->getNumDrawCalls(); ++i)
	{
		G2::AABB const& aabb = renderComp->getDrawCall(i).getModelSpaceAABB();
		G2::VertexArrayObject& vao = aabbVis->getVertexArray(i);
		vao.resizeElementCount(24);
		
		glm::vec3 const& center = aabb.getCenter();
		glm::vec3 const& he = aabb.getHalfExtends();

		glm::vec3 geometry[32];
		// bottom plane
		geometry[0] = center + glm::vec3(-he.x,-he.y,he.z);
		geometry[1] = center + glm::vec3(-he.x,-he.y,-he.z);
		geometry[2] = center + glm::vec3(-he.x,-he.y,-he.z);
		geometry[3] = center + glm::vec3( he.x,-he.y,-he.z);
		geometry[4] = center + glm::vec3( he.x,-he.y,-he.z);
		geometry[5] = center + glm::vec3( he.x,-he.y, he.z);
		geometry[6] = center + glm::vec3( he.x,-he.y, he.z);
		geometry[7] = center + glm::vec3(-he.x,-he.y, he.z);
		// top plane
		geometry[8] =  center + glm::vec3(-he.x, he.y,he.z);
		geometry[9] =  center + glm::vec3(-he.x, he.y,-he.z);
		geometry[10] = center + glm::vec3(-he.x, he.y,-he.z);
		geometry[11] = center + glm::vec3( he.x, he.y,-he.z);
		geometry[12] = center + glm::vec3( he.x, he.y,-he.z);
		geometry[13] = center + glm::vec3( he.x, he.y, he.z);
		geometry[14] = center + glm::vec3( he.x, he.y, he.z);
		geometry[15] = center + glm::vec3(-he.x, he.y, he.z);
		// roundings
		geometry[16] = center + glm::vec3(-he.x, he.y, he.z);
		geometry[17] = center + glm::vec3(-he.x,-he.y, he.z);
		geometry[18] = center + glm::vec3(-he.x, he.y,-he.z);
		geometry[19] = center + glm::vec3(-he.x,-he.y,-he.z);
		geometry[20] = center + glm::vec3( he.x, he.y, he.z);
		geometry[21] = center + glm::vec3( he.x,-he.y, he.z);
		geometry[22] = center + glm::vec3( he.x, he.y,-he.z);
		geometry[23] = center + glm::vec3( he.x,-he.y,-he.z);
		
		vao.writeData(G2Core::Semantics::POSITION, geometry);

		aabbVis->addDrawCall(G2::DrawCall()
			.setDrawMode(G2Core::DrawMode::LINES)
			.setEnabled(true)
			.setAABBCalculationMode(MANUAL)
			.setVaoIndex(i)
			.setModelSpaceAABB(aabb)
			);
	}
	aabbVis->setEffect(
		mSolidFx
	);
}

void
EditorGeometryManager::_onRenderFrame(G2::FrameInfo const& frameInfo) 
{
	
}

void
EditorGeometryManager::_releaseResources() 
{
	mSelectedEntityHighlight = G2::Entity();
}

void
G2::Editor::EditorGeometryManager::_initEditorGrid()
{
	auto* grid = mEditorGrid.addComponent<G2::RenderComponent>(G2Core::RenderLayer::LAYER_30);
	mEditorGrid.addComponent<EditorComponent>();

	grid->allocateVertexArrays(1);

	std::vector<glm::vec3> gridVertices;


	int numLinesPerPlane = 401; 
	int numExtends = (numLinesPerPlane - 1) / 2;
	
	for (int start = 0 - numExtends; start <= numExtends; ++start)
	{
		// line in Z-Direction
		gridVertices.push_back(glm::vec3(start, 0.f, numExtends));
		gridVertices.push_back(glm::vec3(start, 0.f, -numExtends));
		// line in X-Direction
		gridVertices.push_back(glm::vec3(numExtends, 0.f, start));
		gridVertices.push_back(glm::vec3(-numExtends, 0.f, start));
	}
	auto& gridVao = grid->getVertexArray(0);
	gridVao.resizeElementCount((int)gridVertices.size());
	gridVao.writeData(G2Core::Semantics::POSITION, &gridVertices[0], (int)gridVertices.size());

	grid->addDrawCall(DrawCall().setDrawMode(G2Core::DrawMode::LINES).setVaoIndex(0));
	grid->setEffect(
		mSolidFx
	);

	grid->material.setAmbient(glm::vec4(0.3f,0.3f,0.4f,1.f));
}

void
G2::Editor::EditorGeometryManager::setEditorGeometryVisibility(bool mode)
{
	// change visibility for all Entity object, which have a EditorComponent attached
	mEditorSystem->scheduleVisibilityChange(mode);
}