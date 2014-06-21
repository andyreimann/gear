#include "EditorUI.h"
#include "Editor.h"
#include "EditorComponent.h"

#include <G2/Logger.h>
#include <G2/NameComponent.h>
#include <G2/RenderComponent.h>
#include <G2Core/ECSManager.h>

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
	// create (load) the TaharezLook scheme file
	// (this auto-loads the TaharezLook looknfeel and imageset files)
	CEGUI::SchemeManager::getSingleton().createFromFile( "TaharezLook.scheme" );
	CEGUI::SchemeManager::getSingleton().createFromFile( "G2EditorProperties.scheme" );

	CEGUI::System::getSingleton().getDefaultGUIContext().
	getMouseCursor().setDefaultImage( "TaharezLook/MouseArrow" );

	CEGUI::System::getSingleton().getDefaultGUIContext().
	setDefaultTooltipType( "TaharezLook/Tooltip" );

	auto* ceguiComponent = addComponent<G2::UI::CeguiComponent>("G2Editor.layout");
	ceguiComponent->attachToRootWindow();

	mComponentList = static_cast<CEGUI::MultiColumnList*>(ceguiComponent->getWindow()->getChild("G2EditorComponents/ComponentsList"));
	mComponentList->addColumn("ID", 0, CEGUI::UDim(0.2f,0.f));
	mComponentList->addColumn("Name", 1, CEGUI::UDim(0.8f,0.f));

	// Set the users selection ability
	mComponentList->setSelectionMode(CEGUI::MultiColumnList::SelectionMode::RowMultiple);

	mComponentList->subscribeEvent(
		CEGUI::MultiColumnList::EventSelectionChanged,
		CEGUI::Event::Subscriber(
			&EditorUI::_onComponentSelectionChanged,
			this
		)
	);
	mPropertiesMainList = static_cast<CEGUI::ItemListbox*>(ceguiComponent->getWindow()->getChild("G2EditorProperties/PropertiesMainList"));

	CEGUI::GridLayoutContainer* glc = ( CEGUI::GridLayoutContainer *)CEGUI::WindowManager::getSingletonPtr()->createWindow( 
		"GridLayoutContainer", "GLC" );
	glc->setGridDimensions(2,2);
	CEGUI::Window* lable = CEGUI::WindowManager::getSingletonPtr()->createWindow( 
		"TaharezLook/Label", "TestLable" );
	lable->setText("Wireframe");
	glc->addChild(lable);

	mWireframeModeToggle = static_cast<CEGUI::ToggleButton*>(CEGUI::WindowManager::getSingletonPtr()->createWindow( 
		"TaharezLook/Checkbox", "WireframeMode" ));
	
	mWireframeModeToggle->subscribeEvent(
		CEGUI::ToggleButton::EventSelectStateChanged,
		CEGUI::Event::Subscriber(
			&EditorUI::_onWireframeModeChanged,
			this
		)
	);

	glc->addChild(mWireframeModeToggle);

	lable = CEGUI::WindowManager::getSingletonPtr()->createWindow( 
		"TaharezLook/Label", "TestLable3" );
	lable->setText("My Test 3");
	glc->addChild(lable);

	lable = CEGUI::WindowManager::getSingletonPtr()->createWindow( 
		"TaharezLook/Label", "TestLable4" );
	lable->setText("My Test 4");
	glc->addChild(lable);

	mPropertiesMainList->addChild(glc);

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
			renderComp->setPolygonDrawMode(G2::PolygonDrawMode::LINE);
		}
		else
		{
			renderComp->setPolygonDrawMode(G2::PolygonDrawMode::FILL);
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
		mWireframeModeToggle->setSelected(renderComp->getPolygonDrawMode() == G2::PolygonDrawMode::LINE);
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