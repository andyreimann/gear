#include "EditorGeometryManager.h"

#include <G2/RenderComponent.h>
#include <G2/CameraComponent.h>
#include <G2/TransformComponent.h>
#include <G2Core/ECSManager.h>

EditorGeometryManager::EditorGeometryManager()
{
	mSolidFx = mEffectImporter.import("shader/Solid.g2fx");

	// create different resolutions of the grid
	mGrids.push_back(EditorGrid(mSolidFx, 0.01f));
	mGrids.push_back(EditorGrid(mSolidFx, 0.1f));
	mGrids.push_back(EditorGrid(mSolidFx, 1.f));
	mGrids.push_back(EditorGrid(mSolidFx, 10.0));
	mGrids.push_back(EditorGrid(mSolidFx, 100.0));

	G2::EventDistributer::onFrameRendered.hook(this, &EditorGeometryManager::_updateEditorGrid);

}

EditorGeometryManager::~EditorGeometryManager() 
{
	G2::EventDistributer::onFrameRendered.unHookAll(this);
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