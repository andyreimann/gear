#include "EditorUI.h"
#include "Editor.h"
#include "EditorComponent.h"

#include <G2/Logger.h>
#include <G2/NameComponent.h>
#include <G2/RenderComponent.h>

#include <G2/TextureImporter.h>
#include <G2/Texture.h>

#include <G2Core/ECSManager.h>
#include <G2Cegui/CeguiComponent.h>

#include <G2/RenderComponent.h>
#include <G2/CameraComponent.h>
#include <G2/SplineAnimation.h>
#include <G2/TransformComponent.h>

#include <CEGUI/RendererModules/OpenGL/RendererBase.h>



using namespace G2::Editor;

EditorUI::EditorUI(RootEditor* editor) 
	: mEditor(editor),
	mSelectedEntry(nullptr)
{

	_init();

	G2::ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>()
		->onComponentAddedEvent.hook(this, &EditorUI::_onRenderComponentAdded);
	G2::ECSManager::getShared().getSystem<G2::SplineAnimationSystem, G2::SplineAnimation>()
		->onComponentAddedEvent.hook(this, &EditorUI::_onSplineAnimationAdded);
	G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>()
		->onComponentAddedEvent.hook(this, &EditorUI::_onCameraComponentAdded);
	G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>()
		->onComponentAddedEvent.hook(this, &EditorUI::_onTransformComponentAdded);
	G2::ECSManager::getShared().getSystem<EditorSystem, EditorComponent>()
		->onComponentAddedEvent.hook(this, &EditorUI::_onEditorComponentAdded);

	G2::ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>()
		->onComponentRemovedEvent.hook(this, &EditorUI::_onRenderComponentRemoved);
	G2::ECSManager::getShared().getSystem<G2::SplineAnimationSystem, G2::SplineAnimation>()
		->onComponentRemovedEvent.hook(this, &EditorUI::_onSplineAnimationRemoved);
	G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>()
		->onComponentRemovedEvent.hook(this, &EditorUI::_onCameraComponentRemoved);
	G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>()
		->onComponentRemovedEvent.hook(this, &EditorUI::_onTransformComponentRemoved);
	G2::ECSManager::getShared().getSystem<EditorSystem, EditorComponent>()
		->onComponentRemovedEvent.hook(this, &EditorUI::_onEditorComponentRemoved);
}

EditorUI::~EditorUI() 
{
	G2::ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>()
		->onComponentAddedEvent.unHookAll(this);
	G2::ECSManager::getShared().getSystem<G2::SplineAnimationSystem, G2::SplineAnimation>()
		->onComponentAddedEvent.unHookAll(this);
	G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>()
		->onComponentAddedEvent.unHookAll(this);
	G2::ECSManager::getShared().getSystem<EditorSystem, EditorComponent>()
		->onComponentAddedEvent.unHookAll(this);
	G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>()
		->onComponentAddedEvent.unHookAll(this);

	G2::ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>()
		->onComponentRemovedEvent.unHookAll(this);
	G2::ECSManager::getShared().getSystem<G2::SplineAnimationSystem, G2::SplineAnimation>()
		->onComponentRemovedEvent.unHookAll(this);
	G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>()
		->onComponentRemovedEvent.unHookAll(this);
	G2::ECSManager::getShared().getSystem<EditorSystem, EditorComponent>()
		->onComponentRemovedEvent.unHookAll(this);
}

void
EditorUI::_init() 
{
	_initCeguiResources();
	// create (load) the scheme file
	// (this auto-loads the looknfeel and imageset files automatically)
	CEGUI::SchemeManager::getSingleton().createFromFile( "TaharezLook.scheme" );
	CEGUI::SchemeManager::getSingleton().createFromFile( "G2EditorProperties.scheme" );

	// set the mouse arrow
	CEGUI::System::getSingleton().getDefaultGUIContext().
	getMouseCursor().setDefaultImage( "TaharezLook/MouseArrow" );


	// set the default tooltip type
	CEGUI::System::getSingleton().getDefaultGUIContext().
	setDefaultTooltipType( "TaharezLook/Tooltip" );

	// DEBUG START
	auto* ceguiComponent = addComponent<G2::UI::CeguiComponent>("CEEDTest.layout");
	ceguiComponent->attachToRootWindow();
	// DEBUG END
	//auto* ceguiComponent = addComponent<G2::UI::CeguiComponent>("G2Editor.layout");
	//ceguiComponent->attachToRootWindow();

	mComponentList = static_cast<CEGUI::MultiColumnList*>(ceguiComponent->getWindow()->getChild("SceneWindow/ComponentsList"));
	mComponentList->addColumn("ID", 0, CEGUI::UDim(0.2f,0.f));
	mComponentList->addColumn("Name", 1, CEGUI::UDim(0.8f,-16.f));
	// Set the users selection ability
	//mComponentList->setSelectionMode(CEGUI::MultiColumnList::SelectionMode::RowMultiple);

	mComponentList->subscribeEvent(
		CEGUI::MultiColumnList::EventSelectionChanged,
		CEGUI::Event::Subscriber(
			&EditorUI::_onComponentSelectionChanged,
			this
		)
	);
	
	mWireframeModeToggle = static_cast<CEGUI::ToggleButton*>(ceguiComponent->getWindow()->getChild("PropertiesWindow/Rendering/CBWireframe"));
	
	mWireframeModeToggle->subscribeEvent(
		CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(
			&EditorUI::_onWireframeModeChanged,
			this
		)
	);

	mNumTrianglesValue = static_cast<CEGUI::DefaultWindow*>(ceguiComponent->getWindow()->getChild("PropertiesWindow/Rendering/TrianglesBox/Value"));
	mNumDrawCallsValue = static_cast<CEGUI::DefaultWindow*>(ceguiComponent->getWindow()->getChild("PropertiesWindow/Rendering/DrawCallsBox/Value"));
	mNumVaosValue = static_cast<CEGUI::DefaultWindow*>(ceguiComponent->getWindow()->getChild("PropertiesWindow/Rendering/VaosBox/Value"));
	mNumIaosValue = static_cast<CEGUI::DefaultWindow*>(ceguiComponent->getWindow()->getChild("PropertiesWindow/Rendering/IaosBox/Value"));
	mEffectFileValue = static_cast<CEGUI::Editbox*>(ceguiComponent->getWindow()->getChild("PropertiesWindow/Rendering/EffectFileBox/Value"));

	/*
	dTestTex = G2::TextureImporter().import(mEditor->getEditorAssetsFolder() + "cegui/imagesets/TaharezLook.png", G2::LINEAR, G2::LINEAR);

	// We create a CEGUI Texture using the renderer you use:
	CEGUI::Texture& texture = G2::ECSManager::getShared().getSystem<G2::UI::CeguiSystem,G2::UI::CeguiComponent>()->getRenderer().createTexture("Tex", dTestTex->getId(), CEGUI::Sizef(dTestTex->getWidth(), dTestTex->getHeight()));	

	CEGUI::BasicImage* image = static_cast<CEGUI::BasicImage*>(&CEGUI::ImageManager::getSingleton().create("BasicImage", "MyImageGroup/MyImageName"));
	image->setTexture(&texture);
	image->setArea(CEGUI::Rectf(0, 0, dTestTex->getWidth(), dTestTex->getHeight()));
	image->setAutoScaled(CEGUI::AutoScaledMode::ASM_Both);

	
	ceguiComponent->getWindow()->getChild("G2EditorProperties/TestImage")->setProperty("Image", "MyImageGroup/MyImageName");
	((CEGUI::ProgressBar*)ceguiComponent->getWindow()->getChild("G2EditorProperties/TestAlternateProgressBar"))->setProgress(0.75f);
	((CEGUI::ProgressBar*)ceguiComponent->getWindow()->getChild("G2EditorProperties/TestProgressBar"))->setProgress(0.25f);
	CEGUI::TabControl* tabControl = ((CEGUI::TabControl*)ceguiComponent->getWindow()->getChild("G2EditorProperties/TestTabControl"));
	CEGUI::Combobox* combobox = ((CEGUI::Combobox*)ceguiComponent->getWindow()->getChild("G2EditorProperties/TestCombobox"));
	
	CEGUI::ListboxTextItem* itemCombobox = new CEGUI::ListboxTextItem("Value 1", 0);
	//itemCombobox->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("Value 2", 1);
	//itemCombobox->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
	combobox->addItem(itemCombobox);
	*/

}

bool
EditorUI::_onComponentSelectionChanged(const CEGUI::EventArgs &e) 
{
	const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);
	CEGUI::MultiColumnList* componentsList = static_cast<CEGUI::MultiColumnList*>(args->window);

	CEGUI::ListboxItem* selected = componentsList->getFirstSelectedItem();
	//selected->setSelectionColours(CEGUI::Colour (1, 0, 0));
	//selected->setSelectionBrushImage();

	// get user data to map to the ECS
	if(selected->getUserData() == nullptr)
	{
		return false;
	}

	// update properties window
	mSelectedEntry = (ComponentEntryMetaData*)selected->getUserData();
	_updateProperties();

	onComponentSelectionChanged(mSelectedEntry->entityId); // fire for other observers
	return true;
}

bool
EditorUI::_onWireframeModeChanged(const CEGUI::EventArgs &e) 
{
	if (mSelectedEntry == nullptr)
	{
		mWireframeModeToggle->setSelected(false);
	}
	RenderComponent* renderComp = ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>()->get(mSelectedEntry->entityId);
	if(renderComp != nullptr)
	{
		if(mWireframeModeToggle->isSelected())
		{
			renderComp->setPolygonDrawMode(G2Core::PolygonDrawMode::LINE);
		}
		else
		{
			renderComp->setPolygonDrawMode(G2Core::PolygonDrawMode::FILL);
		}
	}
	return true;
}

void
EditorUI::_updateProperties()
{
	if (mSelectedEntry == nullptr)
	{
		return; // TODO reset
	}
	if ((mSelectedEntry->existingComponents & ComponentFlag::RENDER_COMPONENT) == ComponentFlag::RENDER_COMPONENT)
	{
		// initialize all the properties to the state of the currently selected entity
		RenderComponent* renderComp = ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>()->get(mSelectedEntry->entityId);

		mWireframeModeToggle->setEnabled(true);

		if (renderComp != nullptr)
		{
			std::stringstream str;
			str << renderComp->getNumTriangles();
			mNumTrianglesValue->setText(str.str());
			str.str("");
			str << renderComp->getNumDrawCalls();
			mNumDrawCallsValue->setText(str.str());
			str.str("");
			str << renderComp->getNumVertexArrays();
			mNumVaosValue->setText(str.str());
			str.str("");
			str << renderComp->getNumIndexArrays();
			mNumIaosValue->setText(str.str());

			if (renderComp->getEffect().get() != nullptr)
			{
				mEffectFileValue->setText(renderComp->getEffect()->getSetting("SourceFile", "default").value);
			}
			else
			{
				mEffectFileValue->setText("");
			}

			mWireframeModeToggle->setSelected(renderComp->getPolygonDrawMode() == G2Core::PolygonDrawMode::LINE);
		}
		else
		{
			mNumTrianglesValue->setText("0");
			mNumDrawCallsValue->setText("0");
			mNumVaosValue->setText("0");
			mNumIaosValue->setText("0");
			mEffectFileValue->setText("");
			mWireframeModeToggle->setSelected(false);
		}
	}
	else
	{
		mNumTrianglesValue->setText("0");
		mNumDrawCallsValue->setText("0");
		mNumVaosValue->setText("0");
		mNumIaosValue->setText("0");
		mEffectFileValue->setText("");
		mWireframeModeToggle->setEnabled(false);
	}
}

void
EditorUI::_onRenderComponentAdded(G2::RenderSystem* system, unsigned int entityId)
{
	if (mEntityIdsToIgnore.find(entityId) != mEntityIdsToIgnore.end() ||
		_tryMergeEntry(entityId, G2::Editor::ComponentFlag::RENDER_COMPONENT, "MESH"))
	{
		return;
	}
	_createEntry(entityId, ComponentFlag::RENDER_COMPONENT, "MESH");
}

void
EditorUI::_onSplineAnimationAdded(G2::SplineAnimationSystem* system, unsigned int entityId)
{
	if (mEntityIdsToIgnore.find(entityId) != mEntityIdsToIgnore.end() || 
		_tryMergeEntry(entityId, G2::Editor::ComponentFlag::SPLINE_ANIMATION, "SPL"))
	{
		return;
	}
	_createEntry(entityId, ComponentFlag::SPLINE_ANIMATION, "SPL");
}

void
EditorUI::_onCameraComponentAdded(G2::CameraSystem* system, unsigned int entityId)
{
	if (mEntityIdsToIgnore.find(entityId) != mEntityIdsToIgnore.end() ||
		_tryMergeEntry(entityId, G2::Editor::ComponentFlag::CAMERA_COMPONENT, "CAM"))
	{
		return;
	}
	_createEntry(entityId, ComponentFlag::CAMERA_COMPONENT, "CAM");
}

void
EditorUI::_onTransformComponentAdded(G2::TransformSystem* system, unsigned int entityId)
{
	if (mEntityIdsToIgnore.find(entityId) != mEntityIdsToIgnore.end() ||
		_tryMergeEntry(entityId, G2::Editor::ComponentFlag::TRANSFORM_COMPONENT, "TRANS"))
	{
		return;
	}
	_createEntry(entityId, ComponentFlag::TRANSFORM_COMPONENT, "TRANS");
}

void
G2::Editor::EditorUI::_onEditorComponentAdded(EditorSystem* system, unsigned int entityId)
{
	mEntityIdsToIgnore.insert(entityId);
}

void
EditorUI::_onRenderComponentRemoved(G2::RenderSystem* system, unsigned int entityId)
{
	_removeEntry(entityId, ComponentFlag::RENDER_COMPONENT);
}

void
EditorUI::_onSplineAnimationRemoved(G2::SplineAnimationSystem* system, unsigned int entityId)
{
	_removeEntry(entityId, ComponentFlag::SPLINE_ANIMATION);
}

void
EditorUI::_onCameraComponentRemoved(G2::CameraSystem* system, unsigned int entityId)
{
	_removeEntry(entityId, ComponentFlag::CAMERA_COMPONENT);
}

void
EditorUI::_onTransformComponentRemoved(G2::TransformSystem* system, unsigned int entityId)
{
	_removeEntry(entityId, ComponentFlag::TRANSFORM_COMPONENT);
}

void
G2::Editor::EditorUI::_onEditorComponentRemoved(EditorSystem* system, unsigned int entityId)
{
	mEntityIdsToIgnore.erase(entityId);
}

bool
G2::Editor::EditorUI::_tryMergeEntry(unsigned int entityId, G2::Editor::ComponentFlag::Name entryFlag, std::string const& displayName)
{
	for (unsigned int i = 0; i < mComponentList->getRowCount(); ++i)
	{
		CEGUI::uint lRowId = mComponentList->getRowID(i);
		CEGUI::uint lCol1 = mComponentList->getColumnWithID(0);
		CEGUI::uint lCol2 = mComponentList->getColumnWithID(1);
		 
		ComponentEntryMetaData* userData = (ComponentEntryMetaData*)(mComponentList->getItemAtGridReference(CEGUI::MCLGridRef(i, lCol1))->getUserData());
		if (userData != nullptr && userData->entityId == entityId)
		{
			userData->existingComponents |= entryFlag;
			auto* listEntry = mComponentList->getItemAtGridReference(CEGUI::MCLGridRef(i, lCol2));
			listEntry->setText(listEntry->getText() + " " + displayName);
			return true;
		}
	}
	return false;
}

void
G2::Editor::EditorUI::_createEntry(unsigned int entityId, G2::Editor::ComponentFlag::Name entryFlag, std::string const& displayName)
{
	unsigned int rowId = mComponentList->addRow(); // create empty row
	// create first column of row
	{
		std::stringstream str;
		str << entityId;

		// fill with data
		mComponentList->setItem(new CEGUI::ListboxTextItem(str.str(), 0, new ComponentEntryMetaData(entityId, rowId, entryFlag)), 0, rowId);
	}
	// create second column of row
	{
		std::stringstream str;
		str << "Entity ";
		auto* nameComponent = G2::ECSManager::getShared().getSystem<G2::NameSystem, G2::NameComponent>()->get(entityId);
		if (nameComponent != nullptr)
		{
			str << "[" << nameComponent->name << "] " << displayName;
			mComponentList->setItem(new CEGUI::ListboxTextItem(str.str()), 1, rowId);
		}
		else
		{
			str << " " << displayName;
			mComponentList->setItem(new CEGUI::ListboxTextItem(str.str()), 1, rowId);
		}
	}
}

bool
G2::Editor::EditorUI::_removeEntry(unsigned int entityId, G2::Editor::ComponentFlag::Name entryFlag)
{
	for (unsigned int i = 0; i < mComponentList->getChildCount(); ++i)
	{
		ComponentEntryMetaData* userData = ((ComponentEntryMetaData*)mComponentList->getChildAtIdx(i)->getUserData());
		if (userData != nullptr && userData->entityId == entityId)
		{
			userData->existingComponents &= ~entryFlag;
			return true;
		}
	}
	return false;
}

void
EditorUI::show() 
{
	getComponent<G2::UI::CeguiComponent>()->getWindow()->show();
	//mEditor->getCamera()->pause();
}

void
EditorUI::hide() 
{
	getComponent<G2::UI::CeguiComponent>()->getWindow()->hide();
	//mEditor->getCamera()->unpause();
}

void
EditorUI::_initCeguiResources() 
{
	// init the CeguiSystem of gear to bootstrap the OpenGL renderer
	G2::ECSManager::getShared().getSystem<G2::UI::CeguiSystem,G2::UI::CeguiComponent>();

	/****************************************
	 * INIT CEGUI RESOURCE LOADING SYSTEM	*
	 ****************************************/
	// initialise the required dirs for the DefaultResourceProvider
	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
		(CEGUI::System::getSingleton().getResourceProvider());
	rp->setResourceGroupDirectory("schemes", mEditor->getEditorAssetsFolder() + "cegui/schemes/");
	rp->setResourceGroupDirectory("imagesets", mEditor->getEditorAssetsFolder() + "cegui/imagesets/");
	rp->setResourceGroupDirectory("fonts", mEditor->getEditorAssetsFolder() + "cegui/fonts/");
	rp->setResourceGroupDirectory("layouts", mEditor->getEditorAssetsFolder() + "cegui/layouts/");
	rp->setResourceGroupDirectory("looknfeels", mEditor->getEditorAssetsFolder() + "cegui/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", mEditor->getEditorAssetsFolder() + "cegui/lua_scripts/");

	// set the default resource groups to be used
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
}