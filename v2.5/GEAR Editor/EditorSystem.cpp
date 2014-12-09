#include "EditorSystem.h"
#include "EditorComponent.h"
#include "Editor.h"

#include <G2/RenderComponent.h>
#include <G2/CameraComponent.h>
#include <G2/SplineAnimation.h>

#include <G2Core/EventDistributer.h>

using namespace G2::Editor;

EditorSystem::EditorSystem() 
	: mEditor(nullptr),
	mPendingVisibilityChange(false),
	mVisibilityMode(false)
{
}

EditorSystem::~EditorSystem() 
{
}

void
EditorSystem::runPhase(std::string const& name, G2::FrameInfo const& frameInfo) 
{
	if(name == "update")
	{
		auto* renderSystem = G2::ECSManager::getShared().getSystem<G2::RenderSystem,G2::RenderComponent>();
		
		if (mPendingVisibilityChange)
		{
			for (int i = 0; i < components.size(); ++i)
			{
				auto& editorComp = components[i];
				auto* renderComp = renderSystem->get(editorComp.getEntityId());
				if (renderComp)
				{
					renderComp->setDrawcallEnabled(mVisibilityMode);
				}
			}
			mPendingVisibilityChange = false;
		}
	}
}

void
EditorSystem::_onRenderFrame(G2::FrameInfo const& frameInfo) 
{

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

void
EditorSystem::_releaseResources() 
{
	mEditor = nullptr;
}

void
G2::Editor::EditorSystem::scheduleVisibilityChange(bool mode)
{
	mPendingVisibilityChange = true;
	mVisibilityMode = mode;
}

G2::Entity
G2::Editor::EditorSystem::createEditorEntity()
{
	G2::Entity entity;
	entity.addComponent<EditorComponent>();
	return std::move(entity);
}