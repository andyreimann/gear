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

#include <CEGUI/RendererModules/OpenGL/RendererBase.h>



using namespace G2::Editor;

EditorUI::EditorUI(RootEditor* editor) 
	: mEditor(editor),
	mSelectedEntityId(-1)
{
	initCeguiResources();	
}

EditorUI::~EditorUI() 
{
	G2::ECSManager::getShared().getSystem<EditorSystem,EditorComponent>()->onRenderComponentAdded.unHookAll(this);
	G2::ECSManager::getShared().getSystem<EditorSystem,EditorComponent>()->onRenderComponentRemoved.unHookAll(this);
}

void
EditorUI::setup() 
{
	G2::ECSManager::getShared().getSystem<EditorSystem,EditorComponent>()->onRenderComponentAdded.hook(this, &EditorUI::_onRenderComponentAdded);
	G2::ECSManager::getShared().getSystem<EditorSystem,EditorComponent>()->onRenderComponentRemoved.hook(this, &EditorUI::_onRenderComponentRemoved);
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

struct ComponentEntryMetaData
{
	ComponentEntryMetaData(unsigned int entityId, unsigned int rowId) : entityId(entityId), rowId(rowId) {}
	unsigned int entityId;
	unsigned int rowId;
};

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
	mSelectedEntityId = ((ComponentEntryMetaData*)selected->getUserData())->entityId;

	// update properties window
	_updateProperties(mSelectedEntityId);

	onComponentSelectionChanged(mSelectedEntityId); // fire for other observers
	return true;
}

bool
EditorUI::_onWireframeModeChanged(const CEGUI::EventArgs &e) 
{
	if(mSelectedEntityId == -1)
	{
		mWireframeModeToggle->setSelected(false);
	}
	RenderComponent* renderComp = ECSManager::getShared().getSystem<G2::RenderSystem,G2::RenderComponent>()->get(mSelectedEntityId);
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
EditorUI::_updateProperties(unsigned int entityId) 
{
	// initialize all the properties to the state of the currently selected entity
	RenderComponent* renderComp = ECSManager::getShared().getSystem<G2::RenderSystem,G2::RenderComponent>()->get(mSelectedEntityId);
	if(renderComp != nullptr)
	{
		mWireframeModeToggle->setSelected(renderComp->getPolygonDrawMode() == G2Core::PolygonDrawMode::LINE);
	}
	else
	{
		mWireframeModeToggle->setSelected(false);
	}
}

void
EditorUI::_onRenderComponentAdded(unsigned int entityId) 
{
	std::stringstream str;
	str << entityId;
	unsigned int rowId = mComponentList->addRow(); // create empty
	// fill with data
	mComponentList->setItem(new CEGUI::ListboxTextItem(str.str(), 0, new ComponentEntryMetaData(entityId, rowId)), 0, rowId);

	auto* nameComponent = G2::ECSManager::getShared().getSystem<G2::NameSystem,G2::NameComponent>()->get(entityId);
	if(nameComponent != nullptr)
	{
		mComponentList->setItem(new CEGUI::ListboxTextItem(nameComponent->name), 1, rowId);
	}
	else 
	{
		mComponentList->setItem(new CEGUI::ListboxTextItem("G2::RenderComponent"), 1, rowId);
	}
}

void
EditorUI::_onRenderComponentRemoved(unsigned int entityId) 
{
	for(unsigned int i = 0; i < mComponentList->getChildCount(); ++i)
	{
		ComponentEntryMetaData* userData = ((ComponentEntryMetaData*)mComponentList->getChildAtIdx(i)->getUserData());
		if(userData != nullptr && userData->entityId == entityId)
		{
			mComponentList->removeRow(mComponentList->getRowWithID(userData->rowId));
			return;
		}
	}
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
EditorUI::initCeguiResources() 
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