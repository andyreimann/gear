#include "EditorGeometryManager.h"
#include "GEARStudioEvents.h"

#include <G2Core/ECSManager.h>

G2Core::RenderLayer::Name EditorGeometryManager::gHandleLayer = G2Core::RenderLayer::LAYER_30;

G2Core::RenderLayer::RenderLayerMask EditorGeometryManager::gEditorGeometryLayers = EditorGeometryManager::gHandleLayer;

EditorGeometryManager::EditorGeometryManager() :
	mActiveGrid(nullptr),
	isTranslatingOnX(false)
{
	mRenderSystem = G2::ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>();
	mCameraSystem = G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>();
	mTransformSystem = G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>();

	mSolidFx = mEffectImporter.import("shader/Solid.g2fx");

	// create different resolutions of the grid
	mGrids.push_back(EditorGrid(mSolidFx, 0.01f));
	mGrids.push_back(EditorGrid(mSolidFx, 0.05f));
	mGrids.push_back(EditorGrid(mSolidFx, 0.1f));
	mGrids.push_back(EditorGrid(mSolidFx, 0.5f));
	mGrids.push_back(EditorGrid(mSolidFx, 1.f));
	mGrids.push_back(EditorGrid(mSolidFx, 5.0));
	mGrids.push_back(EditorGrid(mSolidFx, 10.0));
	mGrids.push_back(EditorGrid(mSolidFx, 50.0));
	mGrids.push_back(EditorGrid(mSolidFx, 100.0));

	// register to GEAR events
	G2::EventDistributer::onFrameRendered.hook(this, &EditorGeometryManager::_updateEditorGrid);

	// register to Editor events
	G2S::onManagedEntitySelected.hook(this, &EditorGeometryManager::_onManagedEntitySelected);

	auto* tc = mTransAnchor.addComponent<G2::TransformComponent>();
	_initTranslationHandles();
	_initScaleHandles();
	_initRotationHandles();
}

EditorGeometryManager::~EditorGeometryManager() 
{
	// unhook from GEAR events
	G2::EventDistributer::onFrameRendered.unHookAll(this);
	G2::EventDistributer::onMouseUp.unHookAll(this);
	G2::EventDistributer::onMouseDown.unHookAll(this);
	G2::EventDistributer::onMouseMove.unHookAll(this);

	// unhook from Editor events
	G2S::onManagedEntitySelected.unHookAll(this);
	G2S::onEditorHandleSelected.unHookAll(this);
}

void EditorGeometryManager::_initTranslationHandles()
{
	mTransZMesh = mFbxImporter.import("meshes/translate.fbx");
	auto* rc = mTransZMesh.getComponent<G2::RenderComponent>();
	auto* tc = mTransZMesh.addComponent<G2::TransformComponent>();
	_initHandleSettings(rc, tc, glm::vec4(0.f, 0.f, 1.f, 1.f), 0.f, glm::vec3(0.f, 0.f, 1.f));

	mTransYMesh = mFbxImporter.import("meshes/translate.fbx");
	rc = mTransYMesh.getComponent<G2::RenderComponent>();
	tc = mTransYMesh.addComponent<G2::TransformComponent>();
	_initHandleSettings(rc, tc, glm::vec4(0.f, 1.f, 0.f, 1.f), -90.f, glm::vec3(1.f, 0.f, 0.f));

	mTransXMesh = mFbxImporter.import("meshes/translate.fbx");
	rc = mTransXMesh.getComponent<G2::RenderComponent>();
	tc = mTransXMesh.addComponent<G2::TransformComponent>();
	_initHandleSettings(rc, tc, glm::vec4(1.f, 0.f, 0.f, 1.f), 90.f, glm::vec3(0.f, 1.f, 0.f));
}

void EditorGeometryManager::_initScaleHandles()
{
	mScaleZMesh = mFbxImporter.import("meshes/scale.fbx");
	auto* rc = mScaleZMesh.getComponent<G2::RenderComponent>();
	auto* tc = mScaleZMesh.addComponent<G2::TransformComponent>();
	_initHandleSettings(rc, tc, glm::vec4(0.f, 0.f, 1.f, 1.f), 0.f, glm::vec3(0.f, 0.f, 1.f));

	mScaleYMesh = mFbxImporter.import("meshes/scale.fbx");
	rc = mScaleYMesh.getComponent<G2::RenderComponent>();
	tc = mScaleYMesh.addComponent<G2::TransformComponent>();
	_initHandleSettings(rc, tc, glm::vec4(0.f, 1.f, 0.f, 1.f), -90.f, glm::vec3(1.f, 0.f, 0.f));

	mScaleXMesh = mFbxImporter.import("meshes/scale.fbx");
	rc = mScaleXMesh.getComponent<G2::RenderComponent>();
	tc = mScaleXMesh.addComponent<G2::TransformComponent>();
	_initHandleSettings(rc, tc, glm::vec4(1.f, 0.f, 0.f, 1.f), 90.f, glm::vec3(0.f, 1.f, 0.f));
}

void EditorGeometryManager::_initRotationHandles()
{
	mRotateZMesh = mFbxImporter.import("meshes/rotate.fbx");
	auto* rc = mRotateZMesh.getComponent<G2::RenderComponent>();
	auto* tc = mRotateZMesh.addComponent<G2::TransformComponent>();
	_initHandleSettings(rc, tc, glm::vec4(0.f, 0.f, 1.f, 1.f), 0.f, glm::vec3(0.f, 0.f, 1.f));

	mRotateYMesh = mFbxImporter.import("meshes/rotate.fbx");
	rc = mRotateYMesh.getComponent<G2::RenderComponent>();
	tc = mRotateYMesh.addComponent<G2::TransformComponent>();
	_initHandleSettings(rc, tc, glm::vec4(0.f, 1.f, 0.f, 1.f), -90.f, glm::vec3(1.f, 0.f, 0.f));

	mRotateXMesh = mFbxImporter.import("meshes/rotate.fbx");
	rc = mRotateXMesh.getComponent<G2::RenderComponent>();
	tc = mRotateXMesh.addComponent<G2::TransformComponent>();
	_initHandleSettings(rc, tc, glm::vec4(1.f, 0.f, 0.f, 1.f), 90.f, glm::vec3(0.f, 1.f, 0.f));
}

void EditorGeometryManager::_initHandleSettings(G2::RenderComponent* rc, G2::TransformComponent* tc, glm::vec4 const& ambientColor, float degrees, glm::vec3 const& axis)
{
	rc->setRenderLayerMask(gHandleLayer);
	rc->setEffect(mSolidFx);
	rc->material.setAmbient(ambientColor);
	rc->setRenderDepth(false);
	rc->setDepthFunc(G2Core::DepthFunc::ALWAYS);
	rc->setDrawcallEnabled(false);
	tc->rotateAxis(degrees, axis);
	tc->setParent(mTransAnchor.getComponent<G2::TransformComponent>());
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
			mActiveGrid = &(*it);
			_updateGridPosition();
			return;
		}
	}
	((G2::Entity*)(*mGrids.begin()))->getComponent<G2::RenderComponent>()->enable();
	mActiveGrid = &(*mGrids.begin());
	_updateGridPosition();
}

void EditorGeometryManager::_updateGridPosition()
{
	auto* renderCamera = mCameraSystem->getRenderCamera();
	auto* cameraTransformation = mTransformSystem->get(renderCamera->getEntityId());

	glm::vec3 const& camPos = cameraTransformation->getPosition();

	// slot grid on xz plane based on it's unit size
	float gridPosX = (float)(int)(-camPos.x / mActiveGrid->getUnitSize()) * mActiveGrid->getUnitSize();
	float gridPosZ = (float)(int)(-camPos.z / mActiveGrid->getUnitSize()) * mActiveGrid->getUnitSize();
	((G2::Entity*)(*mActiveGrid))->addComponent<G2::TransformComponent>()->setPosition(glm::vec3(gridPosX, 0.f, gridPosZ));

	// DEBUG

	// sync position - add transform component if not already there
	//auto* tc = mSelectedEntityAABB.addComponent<G2::TransformComponent>();
	//mTransAnchor.getComponent<G2::TransformComponent>()->setPosition(glm::vec3(tc->getWorldSpaceMatrix() * glm::vec4(0.f,0.f,0.f,1.f)));
}

void EditorGeometryManager::_onManagedEntitySelected(ManagedEntity* entity)
{
	if (entity != nullptr)
	{
		std::stringstream log;
		log << "Changed Entity-Context to " << entity->getName() << "[" << entity->getId() << "]";
		G2S::onLog(INFO, log.str());
	}
	mSelectedEntityAABB = G2::Entity();
	if (entity != nullptr && entity->hasComponent<G2::RenderComponent>())
	{
		auto* renderComp = entity->getComponent<G2::RenderComponent>();
		// connect to transformation of model
		auto* transformComp = entity->addComponent<G2::TransformComponent>();

		auto* aabbVisTransform = mSelectedEntityAABB.addComponent<G2::TransformComponent>();
		// fetch it newly, because the pointer might already be invalid!
		aabbVisTransform->setParent(entity->getComponent<G2::TransformComponent>());
		
		// visualize model space AABBs
		auto* aabbVis = mSelectedEntityAABB.addComponent<G2::RenderComponent>();
		aabbVis->allocateVertexArrays((unsigned int)renderComp->getNumDrawCalls());
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