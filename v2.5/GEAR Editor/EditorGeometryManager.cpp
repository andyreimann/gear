#include "EditorGeometryManager.h"
#include "EditorComponent.h"
#include "Editor.h"

#include <G2/RenderComponent.h>
#include <G2/CameraComponent.h>

#include <G2Core/EventDistributer.h>

using namespace G2::Editor;


EditorGeometryManager::EditorGeometryManager(RootEditor* editor) 
	: mEditor(editor)
{
	mRenderSystem = G2::ECSManager::getShared().getSystem<G2::RenderSystem,G2::RenderComponent>();
	mCameraSystem = G2::ECSManager::getShared().getSystem<G2::CameraSystem,G2::CameraComponent>();
	mEditorSystem = G2::ECSManager::getShared().getSystem<EditorSystem,EditorComponent>();
	
	G2::EventDistributer::onRenderFrame.hook(this, &EditorGeometryManager::_onRenderFrame);
	
	mEditor->getUI()->onComponentSelectionChanged.hook(this, &EditorGeometryManager::_setAsSelected);
}

EditorGeometryManager::~EditorGeometryManager() 
{
	G2::EventDistributer::onRenderFrame.unHookAll(this);
}

void
EditorGeometryManager::_setAsSelected(unsigned int entityId) 
{
	auto* renderSystem = G2::ECSManager::getShared().getSystem<G2::RenderSystem,G2::RenderComponent>();
	auto* renderComp = renderSystem->get(entityId);
	if(renderComp == nullptr)
	{
		return;
	}
	mSelectedRCVis = G2::Entity();
	// visualize world space AABBs
	auto* aabbVis = mSelectedRCVis.addComponent<G2::RenderComponent>();
	mSelectedRCVis.addComponent<EditorComponent>();
	aabbVis->allocateVertexArrays((unsigned int)renderComp->getNumDrawCalls());
	//aabbVis->drawMode = GL_LINES;
	aabbVis->material.setAmbient(glm::vec4(0.f,1.f,0.f,1.f));

	for(unsigned int i = 0; i < renderComp->getNumDrawCalls(); ++i)
	{
		G2::AABB const& aabb = renderComp->getDrawCall(i).getWorldSpaceAABB();
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
		mEffectImporter.import(mEditor->getEditorAssetsFolder() + "shader/Solid.g2fx")
	);
}

void
EditorGeometryManager::_onRenderFrame(G2::FrameInfo const& frameInfo) 
{
	
	auto const& cameras = mCameraSystem->getComponents();

	for(int i = 0; i < cameras.size(); ++i)
	{
		if(cameras[i].getEntityId() != mEditor->getCamera()->getComponent<G2::CameraComponent>()->getEntityId())
		{
			if(!mCameraFrustumEntities[cameras[i].getEntityId()].hasComponent<EditorComponent>())
			{
				mCameraFrustumEntities[cameras[i].getEntityId()].addComponent<EditorComponent>();
				// try get camera
				auto* cameraSystem = G2::ECSManager::getShared().getSystem<G2::CameraSystem,G2::CameraComponent>();

				// setup frustum RenderComponent
				glm::vec4 const* points = cameras[i].getFrustum().getCornerPoints();
				auto* renderSystem = G2::ECSManager::getShared().getSystem<G2::RenderSystem,G2::RenderComponent>();
				
				auto* renderComponent = mCameraFrustumEntities[cameras[i].getEntityId()].addComponent<G2::RenderComponent>();
				renderComponent->allocateVertexArrays(1);
				renderComponent->material.setAmbient(glm::vec4(1.f,0.8f,0.f,1.f));
				// prepare vao
				G2::VertexArrayObject& vao = renderComponent->getVertexArray(0);

				vao.resizeElementCount(32);

				glm::vec4 geometry[32];
				// right frustum plane
				geometry[0] = points[0];
				geometry[1] = points[4];
				geometry[2] = points[4];
				geometry[3] = points[5];
				geometry[4] = points[5];
				geometry[5] = points[1];
				geometry[6] = points[1];
				geometry[7] = points[0];
				// left frustum plane
				geometry[8] = points[3];
				geometry[9] = points[7];
				geometry[10] = points[7];
				geometry[11] = points[6];
				geometry[12] = points[6];
				geometry[13] = points[2];
				geometry[14] = points[2];
				geometry[15] = points[3];
				// top frustum plane
				geometry[16] = points[1];
				geometry[17] = points[5];
				geometry[18] = points[5];
				geometry[19] = points[6];
				geometry[20] = points[6];
				geometry[21] = points[2];
				geometry[22] = points[2];
				geometry[23] = points[1];
				// bottom frustum plane
				geometry[24] = points[0];
				geometry[25] = points[4];
				geometry[26] = points[4];
				geometry[27] = points[7];
				geometry[28] = points[7];
				geometry[29] = points[3];
				geometry[30] = points[3];
				geometry[31] = points[0];

				vao.writeData(G2Core::Semantics::POSITION, geometry);

				
				renderComponent->addDrawCall(G2::DrawCall()
					.setDrawMode(G2Core::DrawMode::LINES)
					.setEnabled(true)
					.setAABBCalculationMode(AUTOMATIC)
					.setVaoIndex(0)
					);

				renderComponent->setEffect(
					mEffectImporter.import(mEditor->getEditorAssetsFolder() + "shader/Solid.g2fx")
				);
			}
		}
	}
}

void
EditorGeometryManager::_releaseResources() 
{
	mSelectedRCVis = G2::Entity();
	mCameraFrustumEntities.clear();
}