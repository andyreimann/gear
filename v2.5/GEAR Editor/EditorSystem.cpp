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
		// reset all seen render components
		for(auto it = mRenderComponentsProcessed.begin(); it != mRenderComponentsProcessed.end(); ++it)
		{
			it->second = false;
		}
		auto* cameraSystem = G2::ECSManager::getShared().getSystem<G2::CameraSystem,G2::CameraComponent>();
		auto* renderSystem = G2::ECSManager::getShared().getSystem<G2::RenderSystem,G2::RenderComponent>();
		
		auto const& renderComponents = renderSystem->getComponents();
		
		for(int i = 0; i < renderComponents.size(); ++i)
		{
			if(get(renderComponents[i].getEntityId()) != nullptr)
			{
				// rendercomponent has EditorComponent -> ignore
				continue;
			}
			auto it = mRenderComponentsProcessed.find(renderComponents[i].getEntityId());
			if(it != mRenderComponentsProcessed.end())
			{
				// already previously seen
				it->second = true;
			}
			else 
			{
				// newly added
				mRenderComponentsProcessed.insert(std::make_pair(renderComponents[i].getEntityId(), true));
				// fire event
				onRenderComponentAdded(renderComponents[i].getEntityId());
			}
		}
		for(auto it = mRenderComponentsProcessed.begin(); it != mRenderComponentsProcessed.end();)
		{
			if(it->second == false)
			{
				// not there anymore
				onRenderComponentRemoved(it->first);
				it = mRenderComponentsProcessed.erase(it);
				continue;
			}
			++it;
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
