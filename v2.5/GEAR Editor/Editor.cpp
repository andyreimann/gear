#include "Editor.h"
#include "EditorComponent.h"

#include <G2/AbstractWindow.h>
#include <G2/RenderComponent.h>
#include <G2/CameraComponent.h>
#include <G2Core/EventDistributer.h>
#include <G2Cegui/CeguiComponent.h>

using namespace G2::Editor;

RootEditor::RootEditor(G2::AbstractWindow* window, std::string const& editorAssetsFolder)
	: mWindow(window),
	mEditorAssetsFolder(editorAssetsFolder),
	mEditorCamera(window),
	mRenderSystem(G2::ECSManager::getShared().getSystem<G2::RenderSystem,G2::RenderComponent>()),
	mCameraSystem(G2::ECSManager::getShared().getSystem<G2::CameraSystem,G2::CameraComponent>()),
	mEditorUI(this),
	mEditorGeometryManager(this)
{
	G2::EventDistributer::onViewportResize.hook(this, &RootEditor::_onViewportResize);
	mEditorCamera.setInternals(70.f, 0.1f, 500.f);
	mEditorCamera.pause();
	mEditorUI.setup();
	mEditorUI.hide();
}

RootEditor::~RootEditor() 
{
	G2::EventDistributer::onViewportResize.unHook(this, &RootEditor::_onViewportResize);
	ECSManager::getShared().getSystem<EditorSystem,EditorComponent>()->_releaseResources();
}

void
RootEditor::start() 
{
	// safe state
	mPreviousMainCameraId = mCameraSystem->getRenderCamera()->getEntityId();
	// enable rendering from editor view
	mEditorCamera.getComponent<G2::CameraComponent>()->setAsRenderCamera();
	mEditorCamera.unpause();
	ECSManager::getShared().getSystem<EditorSystem,EditorComponent>()->_setRootEditor(this);
	mEditorUI.show();
	G2::ECSManager::getShared().updateInvokeOrder<G2::RenderSystem,G2::RenderComponent,G2::UI::CeguiSystem,G2::UI::CeguiComponent>();
}

void
RootEditor::stop() 
{
	// restore previous state
	mCameraSystem->get(mPreviousMainCameraId)->setAsRenderCamera();
	mEditorCamera.pause();
	ECSManager::getShared().getSystem<EditorSystem,EditorComponent>()->_releaseResources();
	mEditorGeometryManager._releaseResources();
	mEditorUI.hide();
}

void
RootEditor::_onViewportResize(int width, int height) 
{
	mEditorCamera.setViewport(width, height);
}