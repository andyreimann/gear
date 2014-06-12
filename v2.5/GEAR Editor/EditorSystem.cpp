#include "EditorSystem.h"
#include "EditorComponent.h"
#include "Editor.h"

#include <G2/RenderComponent.h>
#include <G2/CameraComponent.h>

#include <G2Core/EventDistributer.h>

using namespace G2::Editor;

EditorSystem::EditorSystem() 
	: mEditor(nullptr)
{
	G2::EventDistributer::onRenderFrame.hook(this, &EditorSystem::_onRenderFrame);
}

EditorSystem::~EditorSystem() 
{
	mCameraFrustumEntities.clear();
	G2::EventDistributer::onRenderFrame.unHook(this, &EditorSystem::_onRenderFrame);
}

void
EditorSystem::runPhase(std::string const& name, G2::FrameInfo const& frameInfo) 
{
	if(mEditor == nullptr)
	{
		return;
	}
	if(name == "update")
	{
		auto* cameraSystem = G2::ECSManager::getShared().getSystem<G2::CameraSystem,G2::CameraComponent>();
		//cameraSystem->lock();
		auto const& cameras = cameraSystem->getComponents();

		for(int i = 0; i < cameras.size(); ++i)
		{
			if(cameras[i].getEntityId() != mEditor->mEditorCamera.getComponent<G2::CameraComponent>()->getEntityId())
			{
				if(!mCameraFrustumEntities[cameras[i].getEntityId()].hasComponent<EditorComponent>())
				{
					// schedule for construction later
					//mCameraFrustumEntities[cameras[i].getEntityId()] = Entity();
				}
			}
		}
		//cameraSystem->unlock();
	}
}

void
EditorSystem::_onRenderFrame(G2::FrameInfo const& frameInfo) 
{
	for(auto it = std::begin(mCameraFrustumEntities); it != std::end(mCameraFrustumEntities); ++it)
	{

		if(!it->second.hasComponent<EditorComponent>())
		{
			it->second.addComponent<EditorComponent>();
			// try get camera
			auto* cameraSystem = G2::ECSManager::getShared().getSystem<G2::CameraSystem,G2::CameraComponent>();
			auto* camera = cameraSystem->get(it->first);
			if(camera != nullptr)
			{
				// setup frustum RenderComponent
				glm::vec4 const* points = camera->getFrustum().getCornerPoints();
				auto* renderSystem = G2::ECSManager::getShared().getSystem<G2::RenderSystem,G2::RenderComponent>();
				
				auto* renderComponent = it->second.addComponent<G2::RenderComponent>();
				renderComponent->allocateVertexArrays(1);
				renderComponent->drawMode = GL_TRIANGLES;
				// prepare vao
				G2::VertexArrayObject& vao = renderComponent->getVertexArray(0);

				vao.resize(24);

				glm::vec4 geometry[24];
				// right frustum plane
				geometry[0] = points[0];
				geometry[1] = points[4];
				geometry[2] = points[5];
				geometry[3] = points[0];
				geometry[4] = points[5];
				geometry[5] = points[1];
				// left frustum plane
				geometry[6] = points[3];
				geometry[7] = points[6];
				geometry[8] = points[7];
				geometry[9] = points[3];
				geometry[10] = points[2];
				geometry[11] = points[6];
				// top frustum plane
				geometry[12] = points[1];
				geometry[13] = points[2];
				geometry[14] = points[6];
				geometry[15] = points[1];
				geometry[16] = points[6];
				geometry[17] = points[5];
				// bottom frustum plane
				geometry[18] = points[0];
				geometry[19] = points[7];
				geometry[20] = points[3];
				geometry[21] = points[0];
				geometry[22] = points[4];
				geometry[23] = points[7];

				vao.writeData(G2::Semantics::POSITION, geometry);
			}
		}
	}
}

void
EditorSystem::_setRootEditor(RootEditor* editor) 
{
	mEditor = editor;
	if(mEditor == nullptr)
	{
		// free all editor content
		//lock();
		//components.clear();
		//unlock();
	}
}