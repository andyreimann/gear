#include "EditorGeometryManager.h"
#include "GEARStudioEvents.h"

#include <G2Core/ECSManager.h>

EditorGeometryManager::EditorGeometryManager()
{
	mRenderSystem = G2::ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>();
	mCameraSystem = G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>();
	mTransformSystem = G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>();

	mSolidFx = mEffectImporter.import("shader/Solid.g2fx");

	// create different resolutions of the grid
	mGrids.push_back(EditorGrid(mSolidFx, 0.01f));
	mGrids.push_back(EditorGrid(mSolidFx, 0.1f));
	mGrids.push_back(EditorGrid(mSolidFx, 1.f));
	mGrids.push_back(EditorGrid(mSolidFx, 10.0));
	mGrids.push_back(EditorGrid(mSolidFx, 100.0));

	G2::EventDistributer::onFrameRendered.hook(this, &EditorGeometryManager::_updateEditorGrid);
	GEARStudioEvents::onManagedEntitySelected.hook(this, &EditorGeometryManager::_onManagedEntitySelected);

}

EditorGeometryManager::~EditorGeometryManager() 
{
	G2::EventDistributer::onFrameRendered.unHookAll(this);
	GEARStudioEvents::onManagedEntitySelected.unHookAll(this);
}

void EditorGeometryManager::_updateEditorGrid(G2::FrameInfo const& frame)
{
	auto* camera = G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>()->getRenderCamera();

	glm::vec4 pos(0.f, 0.f, 0.f, 1.f);
	pos = glm::inverse(G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>()->get(camera->getEntityId())->getWorldSpaceMatrix()) * pos;

	float posY = std::abs(pos.y);

	for (auto it = mGrids.begin(); it != mGrids.end(); ++it)
	{
		((G2::Entity*)(*it))->getComponent<G2::RenderComponent>()->disable();
	}

	for (auto it = mGrids.begin(); it != mGrids.end(); ++it)
	{
		float maxY = it->getUnitSize() * 25.f;
		if (maxY > posY)
		{
			// found the closest grid match
			((G2::Entity*)(*it))->getComponent<G2::RenderComponent>()->enable();
			return;
		}
	}
	((G2::Entity*)(*mGrids.begin()))->getComponent<G2::RenderComponent>()->enable();
}

void EditorGeometryManager::_onManagedEntitySelected(ManagedEntity* entity)
{
	mSelectedEntityAABB = G2::Entity();
	if (entity != nullptr && entity->hasComponent<G2::RenderComponent>())
	{
		auto* renderComp = entity->getComponent<G2::RenderComponent>();
		// connect to transformation of model
		auto* transformComp = entity->getComponent<G2::TransformComponent>();
		if (transformComp)
		{
			auto* aabbVisTransform = mSelectedEntityAABB.addComponent<G2::TransformComponent>();
			// fetch it newly, because the pointer might already be invalid!
			aabbVisTransform->setParent(entity->getComponent<G2::TransformComponent>());
		}
		// visualize model space AABBs
		auto* aabbVis = mSelectedEntityAABB.addComponent<G2::RenderComponent>();
		aabbVis->allocateVertexArrays((unsigned int)renderComp->getNumDrawCalls());
		//aabbVis->drawMode = GL_LINES;
		aabbVis->material.setAmbient(glm::vec4(0.f, 1.f, 0.f, 1.f));
		for (unsigned int i = 0; i < renderComp->getNumDrawCalls(); ++i)
		{
			G2::AABB const& aabb = renderComp->getDrawCall(i).getModelSpaceAABB();
			G2::VertexArrayObject& vao = aabbVis->getVertexArray(i);
			vao.resizeElementCount(24);

			glm::vec3 const& center = aabb.getCenter();
			glm::vec3 const& he = aabb.getHalfExtends();

			glm::vec3 geometry[32];
			// bottom plane
			geometry[0] = center + glm::vec3(-he.x, -he.y, he.z);
			geometry[1] = center + glm::vec3(-he.x, -he.y, -he.z);
			geometry[2] = center + glm::vec3(-he.x, -he.y, -he.z);
			geometry[3] = center + glm::vec3(he.x, -he.y, -he.z);
			geometry[4] = center + glm::vec3(he.x, -he.y, -he.z);
			geometry[5] = center + glm::vec3(he.x, -he.y, he.z);
			geometry[6] = center + glm::vec3(he.x, -he.y, he.z);
			geometry[7] = center + glm::vec3(-he.x, -he.y, he.z);
			// top plane
			geometry[8] = center + glm::vec3(-he.x, he.y, he.z);
			geometry[9] = center + glm::vec3(-he.x, he.y, -he.z);
			geometry[10] = center + glm::vec3(-he.x, he.y, -he.z);
			geometry[11] = center + glm::vec3(he.x, he.y, -he.z);
			geometry[12] = center + glm::vec3(he.x, he.y, -he.z);
			geometry[13] = center + glm::vec3(he.x, he.y, he.z);
			geometry[14] = center + glm::vec3(he.x, he.y, he.z);
			geometry[15] = center + glm::vec3(-he.x, he.y, he.z);
			// roundings
			geometry[16] = center + glm::vec3(-he.x, he.y, he.z);
			geometry[17] = center + glm::vec3(-he.x, -he.y, he.z);
			geometry[18] = center + glm::vec3(-he.x, he.y, -he.z);
			geometry[19] = center + glm::vec3(-he.x, -he.y, -he.z);
			geometry[20] = center + glm::vec3(he.x, he.y, he.z);
			geometry[21] = center + glm::vec3(he.x, -he.y, he.z);
			geometry[22] = center + glm::vec3(he.x, he.y, -he.z);
			geometry[23] = center + glm::vec3(he.x, -he.y, -he.z);

			vao.writeData(G2Core::Semantics::POSITION, geometry);

			aabbVis->addDrawCall(G2::DrawCall()
				.setDrawMode(G2Core::DrawMode::LINES)
				.setEnabled(true)
				.setAABBCalculationMode(G2::MANUAL)
				.setVaoIndex(i)
				.setModelSpaceAABB(aabb)
				);
		}
		aabbVis->setEffect(
			mSolidFx
		);
	}
}